# TODO / Known Issues

Running list of outstanding work and known problems found while getting `co2`/`co2-base`/`carbon`
building cleanly on macOS (and preparing for cross-platform CI). Recent commits already fixed a lot
of this — see git log for `Fix macOS build portability...`, `Replace carbon's qsort-based...`,
`Fix the last 2 flaky carbon test fixtures`, and `Emit angle-bracket includes for external...`.
This file only tracks what's still open.

## 1. GitHub Actions CI not yet added

The original ask. Plan already decided, not yet implemented:

- **Bootstrap problem**: `carbon` needs a working `carbon` binary to build itself, and `co2-base`
  needs `carbon` to translate `.co2` → `.c`. A clean checkout can't self-host. Fix: this repo's
  GitHub Releases already host `make dist` tarballs with pre-generated sources
  (`libco2-0.3.0.tar.gz`, `libco2-base-0.3.0.tar.gz`, `carbon-0.3.1.tar.gz` at
  `github.com/peterpaul/co2/releases`) — download these, build them first as a bootstrap
  toolchain, then use that `carbon` binary to build HEAD's actual `co2`/`co2-base`/`carbon`.
- **Important**: those bootstrap tarballs are old releases and predate every fix in this file and
  in the recent commits. Building *them* still needs the full old patch set applied by hand
  (bool/C23 guard, `exception.h`→`co2_exception.h` rename, `Grammar.co2`→`GrammarTokens.co2`
  rename, `lex.l`'s `path` made `extern`, `echo -n`→`printf` in `Makefile.am`, `BUILT_SOURCES`
  fix, and the `string.h`/`IncludeStack.h` angle-bracket fixes since those tarballs' *shipped*
  generated files predate the compiler fix that makes them unnecessary at HEAD). Once bootstrap
  is built, building HEAD's own three projects needs none of that anymore — that's the whole
  point of the work done so far.
- **Matrix**: `ubuntu-latest` (system GCC is real GCC, no special setup needed),
  `macos-latest` (`brew install gcc`, then find the versioned `gcc-N` binary — formula version
  isn't stable, resolve it via `ls $(brew --prefix gcc)/bin | grep -E '^gcc-[0-9]+$' | sort -V | tail -1`),
  `windows-latest` via `msys2/setup-msys2` action in `MINGW64` mode (`mingw-w64-x86_64-gcc`,
  `autoconf`, `automake`, `libtool`, `bison`, `flex`, `pkgconf`) — decided over a Clang-only or
  Windows-skipped approach specifically because carbon's generated code needs GNU nested
  functions (see #3 below).
- All three OS's need `CFLAGS="-std=gnu89 -fcommon -g -O2"` (see #2) until that's fixed properly.

## 2. `-std=gnu89 -fcommon` masks real bugs in carbon's generated C — PARTIALLY FIXED

Original framing here was wrong: `TypeCheckVisitor.co2` is actually a dead stub (never
instantiated, zero references anywhere) — type-checking happens via a `type_check()` method on
every AST node, not a visitor. Two rounds of investigation this session found the real warnings
come from three *independent* root causes, not one:

- **Category A — vtable slot assignment mismatch.** `O_OBJECT_METHOD`/`O_OBJECT_IF_METHOD*`
  (`co2/src/co2/Object.h`, `Interface.h`) do a bare `self->method = _Class_method;` with no cast,
  but each override independently regenerates its own function-pointer typedef from its own
  `.co2` parameter annotations, with zero cross-check against the base class's declared slot type.
  **FIXED**: added a `(typeof(self->method))` cast to all three macros (`typeof` was already used
  elsewhere in the same files, so this isn't a new dependency). Verified: eliminated all 36
  baseline occurrences of this specific warning shape (`assignment to ... from incompatible
  pointer type`) in a full clean carbon self-host rebuild, confirmed by inspecting exactly which
  generated functions the remaining ~13 "assignment to" warnings come from (all ordinary
  constructor field assignments — Category B/C territory, not this one). Zero regressions,
  90/90 carbon tests, 5/5 co2-base, 1/1 co2.
- **Considered and explicitly reverted: a defensive override-signature check.** Tried adding a
  check to `FunctionDeclaration.type_check()` warning when an override's parameter/return types
  aren't a valid contravariant/covariant widening of the base declaration's (to catch genuinely
  broken overrides now that the `typeof` cast above silently accepts *any* mismatch). Built,
  worked correctly on a synthetic bad-override repro — then flagged ~26 sites across carbon's own
  source when run for real. Investigated each: **all 26 are false positives**, all the same
  pattern — this codebase's entire visitor system (`BaseCompileObjectVisitor` and every
  `Generate*Visitor`/`FixScopeVisitor` subclass) depends on overrides *narrowing* their parameter
  type (base declares `visitClassDeclaration(Declaration decl)`, every override declares
  `visitClassDeclaration(ClassDeclaration decl)`), safe only because the call site always does an
  `is_of` check immediately before dispatching — an invariant no static per-signature check can
  verify without real control-flow analysis. Reverted; not worth the noise. If this is
  revisited, it needs to understand call-site dispatch context, not just compare two
  declarations in isolation.
- **Category B — call-site argument casts — PARTIALLY FIXED (highest volume, was ~330 of ~368
  baseline warnings).** `FunctionCallExpression.generate()` emitted arguments with no cast even
  where `type_check_arguments()` already validated (and approved, via subtype/interface
  compatibility) a type that still needs an explicit cast in C, since C has no struct-pointer
  subtyping. **FIXED for ordinary expression arguments**: added `expression_generate_casted`/
  `expression_generate_actual_arguments_casted`/`expression_generate_ctor_arguments_casted`
  (`Expression.co2`) and wired them into `FunctionCallExpression.generate()` (all 5 call-emission
  sites, using a resolved `FunctionType` fetched once at the top of `generate()` — named
  `resolved_function_type` specifically to avoid the existing, confusingly-named local
  `function_type` that's actually the *receiver's* `ObjectType` a few lines below),
  `NewExpression.generate()` (needed a new retained `resolved_ctor` field, since the
  `ConstructorDeclaration` was previously only resolved transiently inside
  `newexpression_type_check_arguments()` during `type_check()` and never carried forward —
  changed that function to return the resolved decl instead of `void`), and
  `SuperExpression.generate()` (same pattern, new `resolved_super_target` field, both the
  `super(...)` ctor-call and `super.method(...)` branches). Verified: eliminated 135 of the 330
  baseline "passing argument ... incompatible pointer type" warnings (330 → 195) in a full clean
  carbon self-host rebuild, zero regressions (libco2 1/1, co2-base 5/5, carbon 90/90, end-to-end
  smoke test).
- **Category D — callback function-pointer casts — FIXED.** The bulk of the remaining ~195
  call-site warnings were a *different* shape than Category B covers: `.map()`/`.map_args()` calls
  (e.g. `list.map_args(some_callback, ...)`) passing a specific callback *function pointer* as an
  argument, where the callback's real C signature (e.g.
  `void (*)(struct SomeSpecificType*, va_list*)`) doesn't match the generic parameter type
  `map`/`map_args` itself declares (`void (*)(struct RefObject*, va_list*)`). type_check() already
  accepts this — every caller only ever invokes the callback with instances of the narrower type,
  by construction of the list it's mapped over — but C requires the two distinct function pointer
  types to match exactly, so the mismatch surfaces as a GCC warning with no cast ever emitted.
  **Fixed**: added `FunctionType.generate_cast()` (`FunctionType.co2`, mirrors the existing
  `generate_named()` but omits the declarator name, producing a bare cast-target string like
  `void (*)(struct RefObject *, va_list *)`), and a new `FunctionType`-vs-`FunctionType` branch in
  `expression_generate_casted()` (`Expression.co2`) that always emits the cast when both the
  target and argument types are function-pointer types — no need to first check whether the two
  signatures actually differ, since a cast to an identical type is a harmless no-op in C. No new
  wiring needed at any call site: `expression_generate_casted` is already invoked for every
  argument via `FunctionCallExpression.generate()`'s existing Category B machinery, so this
  automatically covers `.map()`/`.map_args()` calls (and any other function-pointer-typed
  argument) everywhere, including `NewExpression`/`SuperExpression`'s ctor-argument paths.
  Verified via full clean self-hosted rebuild (two bootstrap passes — see note below):
  `carbon self-hosted 90/90` via `make check` (the properly-wired per-test `.sh` suite, not the
  standalone `run_tests.sh`, which has its own pre-existing, unrelated path-handling bug — see
  below), zero regressions, and the `incompatible pointer type`/`passing argument ... from
  incompatible pointer type` warning count dropped from the ~195 baseline to **69** in a from-raw
  full rebuild.
  - **Bootstrap gotcha hit while verifying this**: `make`'s `%.c: %.co2` rule uses a `CARBON`
    variable hardcoded to an absolute path at `./configure` time (`carbon/src/Makefile:392`), not
    whatever `carbon` is first on `$PATH`. So after building a new `carbon` binary containing a
    compiler source fix, seeing that fix's effect on *other* `.co2` files requires either
    `make CARBON=/path/to/the/new/binary` or manually invoking that binary — plain `make` silently
    keeps using the old pinned compiler and the fix appears to do nothing.
  - **Also found, unrelated, not fixed**: `carbon/test/run_tests.sh` (the standalone, non-`make
    check` driver documented as the normal way to run this suite) passes already-absolute test
    paths (from `find $TESTDIR/...`) into `run_pass_test.sh`/`run_fail_test.sh`, whose
    `run_test_base.sh` then prefixes them with `$SRCDIR` again — this double-prefixes the path
    into something nonexistent whenever `$SRCDIR` is set to anything other than literally empty,
    which is exactly what `make check`'s own `TESTS_ENVIRONMENT` does (`SRCDIR="$(srcdir)"`). The
    result is either a silent "file not found"-turned-crash (segfault) for `fail/` tests (which
    still counts as "correctly failed to compile" so the bug hides), or an outright missing-file
    build failure for `pass/` tests. Pre-existing, not caused by this session's changes; not fixed
    here since `make check`'s per-test `.sh` scripts are unaffected and give a reliable 90/90.
    Worth fixing `run_tests.sh`/`run_test_base.sh`'s path-joining before relying on it again.
- **Category C — silent numeric narrowing (lower priority, doesn't move the GCC-warning-count
  needle).** `PrimitiveType.is_compatible()` warns on numeric narrowing but still returns `true`,
  so no cast is ever inserted — but this is carbon's *own* compile-time diagnostic, not a
  GCC/Clang warning at default `-Wall -Wextra`, so it isn't a blocker for dropping the flags
  either. Not attempted. The plan file below has the mechanism (extend
  `expression_generate_casted` with a `PrimitiveType` branch) if picked up.

**Next step to actually drop the flags**: only Category C (69 warnings left, down from 368
baseline — numeric narrowing, doesn't move the GCC-warning-count needle anyway) and Phase 4 (the
actual attempt to relax `-std=gnu89 -fcommon`) remain. `-fcommon` itself may already be droppable
independently — worth a quick check. Full mechanism/history for all of this preserved at
`~/.claude/plans/abundant-percolating-brooks.md`.

## 3. GNU nested functions force real GCC (excludes Clang, MSVC)

Carbon's own codegen visitors emit GNU nested C functions for closures (`map_args` callback
patterns throughout the compiler's own source). Apple Clang has never supported this GNU
extension, which is why building on macOS needs Homebrew's `gcc` rather than the system
`cc`/`clang`, and why Windows CI needs MSYS2/MinGW's real GCC rather than MSVC.

**Direction**: closure-conversion — lift each nested function to a top-level function and pass
its captured variables through an explicit context struct parameter, the standard technique used
by every "closures compiled to portable C" system. Would remove the GCC dependency entirely.
Sizable rewrite of however callback generation works across the compiler's own source; not
attempted yet.

## 4. `path` global's fragile sharing pattern

Fixed the acute symptom (added `extern` to `lex.l`'s declaration so it doesn't create a second
tentative definition that collides with the real one in `io.c`/`Compiler.c`), but the pattern
itself — a bare global shared by name across a hand-written file and generated files, relying on
both sides declaring compatible types — is fragile by construction and was the one place `-fcommon`
was actually load-bearing.

**Direction**: this codebase already has the right shape sitting right next to it —
`file_hash_map`/`get_hash_map()` in `File.co2` is a lazily-initialized singleton behind a getter,
not a raw shared global. Convert `path` (the include search path list) to the same shape. If this
is genuinely the only place relying on old-style tentative-definition merging, `-fcommon` can
likely be dropped from `CFLAGS` entirely afterward — worth verifying nothing else needs it before
removing it.

## 5. ~~carbon has no pointer arithmetic support (`ptr + int`)~~ — FIXED

Found while fixing `GenerateHeaderIncludesVisitor.co2`/`GenerateSourceIncludesVisitor.co2`
(needed `&quoted[1]` instead of `quoted + 1` to skip a leading character). Carbon's grammar
accepted `char* + int` syntactically but the type checker rejected it as "incompatible types:
char* and int", because `BinaryExpression.type_check()`'s default case ran every non-condition
binary operator through `Type.assert_compatible()` — a general compatibility check (same class of
Type on both sides) with no special case for pointer arithmetic, which is a real, valid exception
in C for `+`/`-` specifically.

**Fixed** in `carbon/src/co2/BinaryExpression.co2`: added `is_integer_type()` and
`is_pointer_arithmetic()` (checks `+`/`-` between an `ArrayType` and an int/unsigned/char
`PrimitiveType`, either operand order for `+`, pointer-first only for `-`, matching real C rules)
and special-cased it in `type_check()` ahead of the general `assert_compatible` fallback, so it's
additive — doesn't touch `ArrayType.is_compatible`, which is also used for assignments where a
bare int must still be rejected (verified `char *x = 5;`-style mismatches still error correctly).

Also found and fixed the **secondary bug** blamed for the error's line misattribution:
`ArrayType.get_token()` was missing its `return` (`base_type.get_token();` instead of
`return base_type.get_token();`) — a real bug, one more instance of the "forgot the `return`
keyword" pattern already found twice in `File.co2`'s old comparators. Fixed. Note: even with that
fixed, `assert_compatible`-driven errors still report a type's *declaration* site rather than the
expression where the mismatch was detected (confirmed this is pre-existing/general behavior
unrelated to pointer arithmetic — a plain `int x; x = someFloat;` mismatch shows the same
declaration-site attribution) — that's a separate, broader design characteristic of how
`assert_compatible`/`get_token()` are used for error reporting across the whole compiler, not
something introduced by or in scope for this fix.

Verified with fully clean rebuilds of `co2-base` and `carbon` (all generated sources deleted,
rebuilt from raw `.co2`): co2-base 5/5, carbon self-hosted 90/90, no regressions.

## 6. Naming-collision risk on case-insensitive filesystems (process, not a bug)

Already hit and fixed three instances this session (`exception.h`/`Exception.h`,
`Grammar.h`/`grammar.h` at the object-file layer, `IncludeStack.co2`'s `["File.h"]` referencing
its own `File.h`). No fourth instance is currently known, but nothing stops a new one from being
introduced as classes get added, since carbon has no awareness of case-insensitive filesystems at
all.

**Direction**: no code fix needed, just a convention — before adding a new class/header name,
grep for existing names that are a case-insensitive match. Could be automated as a cheap CI lint
step (`find . -iname '*.co2' -o -iname '*.h' | tr '[:upper:]' '[:lower:]' | sort | uniq -d`-style
check) if this becomes a recurring problem.

## 7. `["header.h"]` doesn't distinguish quoted vs. angle-bracket includes

The angle-bracket fix (see `Emit angle-bracket includes for external...` commit) made
`["header.h"]` *always* emit `#include <header.h>`, since every current use in this codebase is a
real system header (or, after fixing `IncludeStack.co2`, a project header reachable via the
already-present `-I.` root). But that's a narrower fix than the language really deserves: carbon is
a general-purpose language meant for real programs, not just a bootstrap tool for this repo (it's
been used for actual applications — e.g. a live ray-traced "slime volleyball" game, since lost).
A real C-embedding language should let the author choose quoted-vs-angle-bracket the same way C
itself does, since a from-scratch program built with carbon could plausibly need a header findable
only by same-directory adjacency, not through the compiler's `-I` search path.

**Direction**: extend the grammar to accept both `["quoted.h"]` and `[<bracket.h>]`, preserving
whichever form the source used through to the generated `#include` line, instead of normalizing
everything to one form. Needs: a context-sensitive lexer state entered right after `[` (so `<`/`>`
don't collide with their normal use as relational operators elsewhere in the grammar — same trick
already used for the `<INCLUDE>` start-condition), a way for the resulting token to carry which
form was written, and simplifying `GenerateHeaderIncludesVisitor`/`GenerateSourceIncludesVisitor`
back to just passing that token through unchanged rather than transforming it. Real front-end
surface (lexer/grammar/AST), in the most bootstrap-fragile part of the codebase to iterate on —
worth doing, not urgent.
