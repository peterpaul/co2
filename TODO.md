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

## 2. `-std=gnu89 -fcommon` masks real bugs in carbon's generated C

Currently required because carbon's codegen relies on implicit-int, mismatched pointer types
through the `O_CALL`/`O_CALL_IF` dynamic-dispatch macros, and (until recently) tentative-definition
symbol merging — all things modern C compilers reject or warn hard on by default.

**Direction**: `TypeCheckVisitor.co2` already detects these mismatches — it's *why* you see
`WARNING: incompatible types: ...` / `WARNING: possible data loss ...` spam on every build. It
just warns and moves on instead of inserting the explicit cast it already knows is needed. Fix:
make the type checker (or a new pass) auto-insert those casts in generated code, so the output
compiles clean under a strict modern C dialect and the `-std=gnu89` pin can be dropped (or at
least the reliance on it becomes a choice, not a requirement). Biggest, most valuable, but also
the most invasive fix on this list — touches every codegen path going through the object-dispatch
macros.

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
