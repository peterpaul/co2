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
  below), zero regressions. (An initial "330 → 69" warning count reported here was measured from a
  contaminated intermediate rebuild during the two-pass bootstrap debugging below — see Category
  E's note for the corrected, reproducible baseline: 330 → 116.)
  - **Bootstrap gotcha hit while verifying this**: `make`'s `%.c: %.co2` rule uses a `CARBON`
    variable hardcoded to an absolute path at `./configure` time (`carbon/src/Makefile:392`), not
    whatever `carbon` is first on `$PATH`. So after building a new `carbon` binary containing a
    compiler source fix, seeing that fix's effect on *other* `.co2` files requires either
    `make CARBON=/path/to/the/new/binary` or manually invoking that binary — plain `make` silently
    keeps using the old pinned compiler and the fix appears to do nothing.
  - **Also found, and FIXED**: `carbon/test/run_tests.sh` (the standalone, non-`make check` driver
    documented as the normal way to run this suite) passed already-absolute test paths (from
    `find ${TESTDIR}/${TYPE}`) into `run_pass_test.sh`/`run_fail_test.sh`, whose `run_test_base.sh`
    then prefixed them with `$SRCDIR` again — double-prefixing the path into something nonexistent
    whenever `$SRCDIR` is set to anything other than literally empty, which is exactly what `make
    check`'s own `TESTS_ENVIRONMENT` does (`SRCDIR="$(srcdir)"`). The result was either a silent
    "file not found"-turned-crash (segfault) for `fail/` tests (which still counts as "correctly
    failed to compile" so the bug hid), or an outright missing-file build failure for `pass/`
    tests. Pre-existing, not caused by this session's earlier changes. Root cause was actually one
    level deeper than the path join itself: unlike `run_pass_test.sh`/`run_fail_test.sh` (which
    already derive `BASEDIR` from `$(dirname "$0")`, location-independent), `run_tests.sh` derived
    `BASEDIR` from bare `` `pwd` ``, i.e. it silently assumed it was always invoked with the test
    directory as the caller's CWD. **Fixed**: `BASEDIR` now resolves via `$0` like its sibling
    scripts, and the script `cd`s into it once up front — so `find ${TYPE}` (relative) now always
    searches the right directory regardless of caller CWD, `$TEST` comes out relative like
    `pass/foo.test` (matching what `run_pass_test.sh`/`run_fail_test.sh` expect when invoked
    directly by a human, and what `run_test_base.sh`'s `$SRCDIR` join logic assumes in both its
    "unset" (`.`, now correctly resolving against the `cd`'d-to test dir) and "set, made absolute"
    branches), and the whole script is now callable from any CWD via an absolute or relative path
    to it. Verified: bare `./run_tests.sh` and `SRCDIR=... ./run_tests.sh` (mimicking
    `make check`'s `TESTS_ENVIRONMENT`) both now run every `fail/` test to a clean pass/fail
    verdict with no segfaults. Remaining `run_tests.sh` failures after the fix are two distinct,
    unrelated, pre-existing gaps, not path bugs: (1) `testignore` (`scope_override_declaration_order`,
    `cast_expression`, etc.) isn't consulted by `run_tests.sh` the way it is by
    `generate-testcases.sh`'s generated per-test `.sh` scripts, so deliberately-excluded tests still
    run and "fail"; (2) several `pass/` tests (`class_decl_inheritance`, `dependencies`, `import`,
    etc.) depend on a companion `.co2` helper file in the same directory (e.g. `InheritB.co2` →
    `InheritB.h`) that `run_tests.sh` never compiles, since it only iterates `*.test` files.
- **Category C — silent numeric narrowing — investigated, confirmed to be a non-issue for this
  goal.** `PrimitiveType.is_compatible()` warns on numeric narrowing but still returns `true`, so
  no cast is ever inserted — but checked directly against a full clean rebuild's warning log: there
  are **zero** GCC int/float-narrowing warnings at `-Wall -Wextra` (the handful of `-Wint-conversion`
  hits present are all the pre-existing, unrelated `Interface.h` ternary macro issue noted below,
  not numeric narrowing). Confirms the original plan's own caveat: this is carbon's *own*
  compile-time diagnostic, invisible to GCC by default, so it's not a blocker for dropping the
  flags. Not implemented — would only improve carbon's own diagnostic noise, not GCC's.
- **Category E — `.class` literal arguments mistyped as `void[]` — FIXED (highest-volume of the
  remainder: 89 of the 116 baseline warnings).** Found while measuring Category D's result and
  re-verifying the true baseline (see below). `SomeClass.class` (parsed by the grammar as
  `_TYPE_IDENTIFIER '.' _CLASS` → a bare `TokenExpression` wrapping the `_TYPE_IDENTIFIER` token —
  the *only* place this grammar shape is produced) generates as `SomeClass ()`, calling the
  `O_OBJECT`-generated accessor that returns `struct SomeClassClass *` — a synthetic, per-class
  struct type with no representation anywhere in the `Type` hierarchy. Instead of modeling that,
  `TokenExpression.type_check()`'s `_TYPE_IDENTIFIER` case gives the expression a generic
  `ArrayType(PrimitiveType(void))`, so it never matches Category B/D's `ObjectType`-vs-`ObjectType`
  or `FunctionType`-vs-`FunctionType` branches in `expression_generate_casted()` — the argument
  reaches the callee (typically `Context.find(Class klass)` or `IScope.find_type(Token, Class
  type)`) completely uncast, and `struct SomeClassClass *` vs `struct Class *` is a real GCC
  warning. **Fixed**: added a third branch to `expression_generate_casted()` (`Expression.co2`)
  that detects the AST shape directly — `arg is_of TokenExpression && arg.token.type ==
  _TYPE_IDENTIFIER` — rather than relying on `arg.type` (which stays wrong), and casts
  unconditionally to whatever `ObjectType` the target parameter declares, same "always cast, no
  need to check whether it's needed" reasoning as Category D (every `O_OBJECT`-generated
  `XClass` struct starts with the common `Class` fields via its own `Class()`-accessor
  initialization chain, so the upcast is always safe). Did **not** fix the underlying
  `ArrayType(void)` mistyping itself — that's a real characteristic of the type checker (used
  nowhere except this one grammar production, confirmed by grepping the grammar), left alone to
  keep this fix scoped to codegen, matching how Categories B/D were scoped.
  - **Corrected baseline**: while verifying this, re-measured from a genuinely fresh, fully clean
    rebuild (all generated `.c`/`.h`/`.d` deleted, proper two-pass bootstrap) and got **116** total
    warnings before this fix, not the 69 previously logged under Category D — that 69 was measured
    from an intermediate rebuild contaminated by this session's own debugging (stale files left
    over from chasing the `CARBON`-pinning gotcha below). 116 is the reproducible number: full
    clean two-pass rebuild, `make check` 90/90, then count. This fix takes it to **27**.
  - Verified: full two-pass clean rebuild (`make` with the old installed `carbon`, then `make
    CARBON=<newly-built local binary>` to force every `.co2` file to be re-translated by the fixed
    compiler — see the `CARBON`-pinning gotcha noted under Category D, hit again here), `make
    check` 90/90, zero regressions, warnings 116 → 27. Installed the fixed compiler to
    `~/local-repo` afterward so it's the new baseline for future bootstrap iterations.
  - **Remaining 27 warnings**, for whoever picks this up next: 5 are the pre-existing `Interface.h`
    ternary macro issue (unrelated to codegen); most of the rest are `assignment to`/`returning ...
    from a function with incompatible type` in `Scope.co2`/`CompositeScope.co2`/`RefList.co2`/
    `ClassDeclaration.co2`/`FunctionDeclaration.co2`/`StructDeclaration.co2` — i.e. constructor
    field assignments and `return` statements, not call *arguments*, so Category B/D/E's
    `expression_generate_casted` machinery (only wired into `FunctionCallExpression`/
    `NewExpression`/`SuperExpression`) doesn't reach them at all; a handful are still-uncast
    `passing argument` sites (`set_scope`, `is_compatible`, `visitObjectType`) where the target
    parameter is an interface type — needs checking why Category B's `ObjectType`-vs-`ObjectType`
    branch doesn't already catch those. This last stretch is exactly Category C's originally
    envisioned mechanism (extend casting to `VariableDeclaration.generate()`/
    `ReturnStatement.generate()`), just needed for interface/subtype mismatches rather than numeric
    narrowing.

**Next step to actually drop the flags**: 27 warnings left (down from a 368 original baseline — see
the corrected-baseline note above for why the intermediate "195"/"69" figures logged earlier don't
match a from-scratch rebuild). What's left is assignment/return-statement casts (not call
arguments) plus a few interface-parameter call sites, and Phase 4 (the actual attempt to relax
`-std=gnu89 -fcommon`). `-fcommon` itself may already be droppable independently — worth a quick
check. Full mechanism/history for all of this preserved at
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
