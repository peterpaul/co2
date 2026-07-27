# TODO / Known Issues

Running list of outstanding work and known problems found while getting `co2`/`co2-base`/`carbon`
building cleanly on macOS (and preparing for cross-platform CI). Recent commits already fixed a lot
of this — see git log for `Fix macOS build portability...`, `Replace carbon's qsort-based...`,
`Fix the last 2 flaky carbon test fixtures`, and `Emit angle-bracket includes for external...`.
This file only tracks what's still open.

## 1. GitHub Actions CI — Linux and macOS fully green; Windows has one known open failure

`.github/workflows/ci.yml` + `.github/scripts/{patch-bootstrap.sh,build-and-test.sh}`. Matrix:
`ubuntu-latest`/`macos-latest`/`windows-latest` (MSYS2 MINGW64, since carbon's generated code
needs GNU nested functions — see #3 — which Clang/MSVC don't support). `CFLAGS="-std=gnu89 -g
-O2"` throughout (see #2 — `-fcommon` confirmed unnecessary).

**Linux (`ubuntu-latest`) and macOS (`macos-latest`): fully verified passing via real CI runs.**

- **Bootstrap problem**: `carbon` needs a working `carbon` binary to build itself, and `co2-base`
  needs `carbon` to translate `.co2` → `.c`. A clean checkout can't self-host. Fix:
  `build-and-test.sh` downloads this repo's own old GitHub Release tarballs (`libco2-0.3.0`,
  `libco2-base-0.3.0`, `carbon-0.3.1`), patches and builds them into a scratch bootstrap prefix
  (`patch-bootstrap.sh`), then does a **two-pass** build of HEAD's own `co2`/`co2-base`/`carbon`:
  pass 1 uses the bootstrap carbon to produce HEAD's own carbon binary (untested — this pass's
  compiler-side fixes aren't present in the *old* bootstrap carbon, so it can only be trusted to
  produce a working binary, not correct output); pass 2 re-translates everything with that
  freshly-built HEAD carbon (now reflecting every compiler-side fix in this repo's history) and is
  the pass that's actually tested via `make check`. Both `co2-base` and `carbon` need a `configure`
  re-run between passes — `AC_PATH_PROG(CARBON, carbon, no)` resolves `$CARBON` to an absolute path
  once, at configure time, and doesn't re-resolve it from `PATH` on every `make`.
- Linux needed one additional fix beyond the bootstrap-tarball patches: `LD_LIBRARY_PATH` must
  include both prefixes' `lib/` dirs — Linux shared objects are referenced by bare SONAME and need
  the dynamic linker told where to look outside a system prefix, unlike macOS where libtool embeds
  an absolute install path in each `.dylib` at link time.
- Full list of bootstrap-tarball patches, per-OS fixes, and the two-pass rationale: see the CI
  fix commits themselves (`.github/scripts/build-and-test.sh` and `patch-bootstrap.sh`'s inline
  comments document each one at the point it's applied — the raw reasoning is more useful there
  than duplicated here).

**Windows (`windows-latest`, MSYS2 MINGW64): substantial progress, one test still failing.**

Six separate MinGW/MSYS2 portability gaps were found and fixed (each one only surfaced after
fixing the previous one and getting further into the build — every fix confirmed via a real CI
run before moving to the next):
  - `O_CALL`'s bare `__STRING(msg)` macro-argument stringification relies on BSD/glibc's
    `<sys/cdefs.h>` `__STRING` macro, which MinGW's headers don't provide — added an
    `#ifndef __STRING` fallback in `co2/src/co2/utils.h` (and `patch-bootstrap.sh`'s copy for the
    bootstrap tarball).
  - `ASSERT_FAIL`'s non-glibc branch called Darwin/BSD's `__assert_rtn`, which doesn't exist on
    MinGW either — split into a proper `#elif defined(__APPLE__)` branch plus a generic
    `abort()` fallback (same two files).
  - `runner.temp`'s Windows-style path (`D:\a\_temp`), once passed through MSYS2's path
    conversion, came out mangled (`libtool: error: argument to -rpath is not absolute`) — the
    Windows workflow step now uses a plain MSYS2-native `WORKDIR: /tmp/co2-ci` instead.
  - `random()` isn't provided by MinGW's C library — switched to `rand()` in `BaseObject.co2`'s
    declaration and the two `.co2` test files that called it (plus `patch-bootstrap.sh`'s
    pre-generated bootstrap-tarball `.c` files, which have no carbon binary yet to retranslate
    from source).
  - `localtime_r` (POSIX reentrant variant) isn't provided by MinGW either — switched to plain
    `localtime()` (`LogRecord.co2`, `SimpleFormatter.co2`, `patch-bootstrap.sh`).
  - That `localtime()` swap alone still crashed at runtime on Windows only: MinGW-w64's own
    `struct timeval.tv_sec` is a 32-bit `long`, but `time_t` is 64-bit there, so passing
    `&tv_sec` straight to `localtime()` read 4 bytes of garbage past the field. carbon has no
    `long`/`time_t`-sized type to fix this from `.co2` source alone, so added a small
    hand-written conversion shim (`co2-base/src/co2/time_compat.c`/`.h`, wired into
    `co2-base/src/Makefile.am` as a plain, non-`.co2` source — note the root `.gitignore`'s
    blanket `co2-base/src/co2/*.[cdh]` rule for generated files needed explicit negation
    exceptions for these two, or a fresh clone silently drops them) that takes the seconds count
    *by value* and lets C's own int-to-time_t promotion widen it correctly.
  - `realpath()` (used in carbon's own hand-written `lex.l` for import path resolution) isn't
    provided by MinGW — swapped to `_fullpath(NULL, path, 0)` under `#ifdef __MINGW32__`, matching
    `_fullpath`'s NULL-buffer allocation convention to `realpath`'s.
- **Currently failing**: after all six fixes above, the Windows build now gets all the way
  through both bootstrap passes and HEAD's own two-pass rebuild, but `co2-base`'s own `make check`
  fails on exactly one test — `TestLogger.exe` exits with status 127 and **zero output** (the
  other four co2-base tests — `TestDoubleLinkedList`, `TestRefObject`, `TestMap`, `TestSet` — all
  pass). `TestLogger` is the only one of the five that exercises the `Logger`/`ConsoleHandler`/
  `SimpleFormatter`/`LogRecord` code path at all, so the bug is almost certainly still somewhere in
  there, but the previous `time_compat.c` fix's own cross-compiled object files link cleanly with
  zero warnings, and the import table (checked via `objdump -p`) shows only standard
  `api-ms-win-crt-*.dll`/`KERNEL32.dll` entries — nothing exotic or missing. Diagnosis attempts so
  far: ruled out a missing-DLL theory (clean import table); ruled out `gettimeofday`'s `NULL`
  timezone argument (MinGW-w64's own header comment confirms "the timezone pointer arg is ignored,
  errors are ignored"); attempted to reproduce locally by cross-compiling the exact same sources
  with the MinGW cross-compiler (`brew install mingw-w64`) and running under Wine, but `wine-stable`
  needs `sudo` for its `gstreamer-runtime` dependency, which wasn't available in this environment.
  **Next step, if picked back up**: get a real Windows box or a working local Wine install and
  actually run `TestLogger.exe` directly to see the crash instead of guessing from CI log text —
  every other Windows fix in this list was found this way once local reproduction became possible
  (the MinGW cross-compiler installed via `brew install mingw-w64` is enough to build; only running
  needs Wine or real Windows).

## 2. `-std=gnu89 -fcommon` masks real bugs in carbon's generated C — MOSTLY FIXED

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
- **Category F — assignment/return-statement casts — FIXED (27 → 15).** The remaining warnings
  after Category E were `assignment to`/`returning ... from a function with incompatible type` in
  `Scope.co2`/`CompositeScope.co2`/`RefList.co2`/`ClassDeclaration.co2`/`FunctionDeclaration.co2`/
  `StructDeclaration.co2` — constructor field assignments, plain variable assignments, and `return`
  statements, none of which are call *arguments*, so Category B/D/E's `expression_generate_casted`
  machinery (only wired into `FunctionCallExpression`/`NewExpression`/`SuperExpression`) never
  reached them. This is exactly Category C's originally-envisioned mechanism, just needed for
  interface/subtype mismatches (e.g. `Scope.get_parent()` declares return type `IScope` — matching
  its interface base declaration exactly — but returns `self->parent`, a field concretely typed
  `Scope`) rather than numeric narrowing. **Fixed** in three places: `BinaryExpression.generate()`'s
  default case (covers `=`/`==`/`!=`) now casts the right-hand operand to the left-hand operand's
  resolved type via `expression_generate_casted`, instead of emitting it bare;
  `VariableDeclaration.generate()` now casts its initializer expression to the declared `type`
  instead of calling `expr.generate(fp)` directly; `ReturnStatement.generate()` now casts `expr` to
  `function_type.return_type` in both its plain and try/finally-wrapped forms (the latter also had
  its own separate bug — the `__return_value` temporary was declared using `expr.type` instead of
  the function's actual `return_type`, which could itself silently mismatch the final `return
  __return_value;`).
  - **Bug found and fixed while implementing this**: casting an entire assignment-as-expression
    (`return self->parent = parent_scope;`, i.e. `return X = Y`) by wrapping bare `arg.generate(fp)`
    output in a prefix cast breaks C operator precedence — `(Type) X = Y` parses as `((Type)X) = Y`,
    not `(Type)(X = Y)`, and GCC correctly rejected it (`lvalue required as left operand of
    assignment`). Fixed by having `expression_generate_casted()` wrap the *argument's own output* in
    parens whenever it emits a cast — `(Type) (X = Y)` — via a `casted` flag set in every cast
    branch and checked once at the end, rather than each branch managing its own closing syntax.
    Never triggered before this category because Category B/D/E only ever cast plain call
    arguments (never assignment-shaped expressions).
  - **Second bug found and fixed**: `ObjectType.generate()` emits a plain by-value `struct X` (no
    `*`) when `.decl` is a `StructDeclaration` or `TypeDeclaration` (used for lightweight
    non-object-system value structs, e.g. a `vec` struct in the `ray.test` fixture) — casting an
    expression to a non-pointer aggregate type is illegal C (`conversion to non-scalar type
    requested`), and this newly-added assignment-casting was the first thing to actually exercise
    that path (Category B/D/E never cast a by-value struct field). Fixed by excluding
    `StructDeclaration`/`TypeDeclaration`-backed `ObjectType`s from the cast-decl-mismatch branch —
    only the pointer-shaped forms (plain class, or interface/unresolved → `struct X*`/`struct
    Object*`) get cast.
  - Verified via full two-pass clean rebuild + `make check`: caught a real regression this way
    (`pass/ray.sh` failing to compile from the struct-cast bug above) before it was fixed. Final
    state: 90/90, zero regressions, warnings 27 → 15. Installed to `~/local-repo` as the new
    baseline.
- **Category G — vtable-dispatched calls resolve against the override's own signature, not the
  base-declared vtable slot type — MOSTLY FIXED (4 of 5 warnings eliminated).** The 5
  non-`Interface.h` warnings (`RefList.filter()`'s return via `List.filter()`, `ClassDeclaration
  .is_compatible()`'s argument via `ObjectTypeDeclaration.is_compatible()`, `TokenExpression
  .set_scope()`'s argument via `Expression.set_scope()` (2 call sites), `GenerateSourceIncludesVisitor
  .visitObjectType()`'s argument via its base visitor declaration) all share one root cause: for a
  virtually-dispatched method call (ordinary class-scope `O_CALL`, which is every plain method call
  in this object model — there's no non-virtual variant), the C function pointer's *actual*
  signature is fixed by whichever class/interface *first* declared that method name (same fact
  Category A's `typeof`-cast fix relies on for the *assignment* side) — but the type-checker
  (correctly, for type-checking purposes) resolves `.type` against the *most-derived* override's
  own declared signature, which this session's cast-insertion logic (reasonably) trusted. When an
  override narrows/widens a parameter or return type (the same safe-by-construction pattern noted
  throughout Category A's investigation — e.g. `TokenExpression.set_scope(IScope scope)` overriding
  `Expression.set_scope(Scope scope);`), the two signatures genuinely differ, and casting against
  the override's type doesn't match what the vtable slot actually expects.
  **Fixed**: added `ClassDeclaration.find_base_function_declaration(Token method_name)`
  (`ClassDeclaration.co2`) — walks from a receiver class up through its superclass chain, and at
  each level also checks that level's own directly-declared interfaces (each interface's
  `member_scope` already has all of *its* super-interfaces flattened in via the existing
  diamond-of-death fix, so one level of interface-checking per class is enough — no separate
  recursion needed there), using `Scope.find_type_in_this_scope()` to check only what's declared
  directly at that exact level, not inherited. Returns the most-ancestral `FunctionDeclaration`
  found — the one that actually shaped the vtable slot. Added
  `functioncallexpression_resolve_vtable_type()` (`FunctionCallExpression.co2`) to determine, from
  a call's `function` sub-expression (either bare-name self-calls or explicit `receiver.method()`
  calls — the two shapes `FunctionCallExpression.generate()` already distinguishes), the receiver's
  static class and the method name, then delegates to the new hierarchy walk; wired into
  `generate()` so call-argument casting (`expression_generate_actual_arguments_casted`) uses the
  base signature instead of the resolved one.
  - **Deliberately did NOT touch `type_check()`** (which would have additionally fixed the 5th
    warning, `RefList.filter()`'s return value flowing into a caller's `struct RefList*` variable
    uncast) — tried it twice, in two separate sessions, and both times it broke self-hosted rebuilds
    with a spurious carbon-level "incompatible types: Bool and Bool" error at
    `ClassDeclaration.co2:95` (`is_compatible`'s own declaration line) and a couple of similar spots.
    **Follow-up investigation (this session)**: the error message itself was previously misleading —
    found and fixed a real, separate, unrelated bug along the way: `ObjectType.co2` defined its
    `toString()` override as `to_string()` (snake_case), so `Type.assert_compatible()`'s calls to
    `.toString()` never dispatched to it, always falling back to `BaseObject`'s generic "ClassName at
    0xADDRESS" — meaning *every* `ObjectType` incompatibility error in this compiler's whole history
    printed useless output instead of the actual type name. Fixed (renamed to `toString()`, no other
    callers depended on the old name) — this alone is a real, independent, useful fix, since it makes
    every future type-mismatch error message across the entire compiler actually readable. With that
    fixed, the real message read "incompatible types: Bool and Bool" — and debugging further (temporary
    instrumentation in `ObjectType.is_compatible()`) confirmed both sides resolve to the *same*
    `TypeDeclaration` object for `Bool` (identical pointer, so it's not a canonicalization/identity
    problem as originally suspected), and `TypeDeclaration.is_compatible()` unconditionally `return
    true;` — so by every check performed, this comparison *should* succeed. Yet it still fails, and
    only in the full multi-file `make -k` build: reproducing the *exact* same compiler invocation
    standalone (`make V=1 co2/ClassDeclaration.c`, or `carbon` invoked directly with the identical
    `-I` flags) compiles `ClassDeclaration.co2` cleanly every time, with no error — the failure only
    manifests when many files are retranslated in the same overall build run, which rules out a
    static logic bug in the new casting code itself and points at some state that differs between
    "this file alone" and "this file as file N of ~80 in one build," not yet identified (not
    filesystem parallelism — this Makefile builds serially; the same mystery persisted across
    multiple independent repro attempts). Reverted `type_check()`'s change again rather than ship
    something with an intermittent, unexplained failure mode; call-argument casting alone (kept) is
    a real, safe, independent win that doesn't need it to be correct.
  - **For whoever picks this up next**: the remaining 1 warning (`Object.h:256`, `RefList.filter()`'s
    return case) needs the `type_check()` fix specifically (set `type` from the base-resolved return
    type, not the resolved override's), but *don't* trust a standalone repro — this bug only shows up
    inside the real multi-file `make -k` build, and the fastest way to iterate on it is probably to
    add a print statement identifying which specific `.co2` file's compilation triggers it (not just
    which line the error is attributed to, which may be misleading — check whether it's actually
    `ClassDeclaration.co2` being compiled at that point in the log, e.g. by cross-referencing
    surrounding warning lines' file paths, the same way this session did).
  - Verified via full two-pass clean rebuild + `make check`: 90/90, zero regressions, warnings
    5 → 1. Installed to `~/local-repo` as the new bootstrap baseline.

- **`Interface.h`'s `O_BRANCH_CALL_IF` ternary macro bug — FIXED (10 warnings eliminated).**
  `assertTrue(_tmp ? _if : true, ...)` and `assertTrue(_tmp ? _if->msg : true, ...)` mix a pointer
  operand (`_if`/`_if->msg`) with the integer `true` in a ternary — `true` isn't a null-pointer
  constant, so GCC correctly flags the two branches as incompatible types
  (`-Wint-conversion`). `assertTrue`'s parameter is only ever used for truthiness, so the ternary's
  actual result type never mattered semantically — fixed in both `co2/src/co2/Interface.h` and the
  installed copy by making both branches genuinely boolean: `_tmp ? _if != NULL : true` and
  `_tmp ? _if->msg != NULL : true`. Zero behavior change, purely a type-compatibility fix.
- **A whole previously-uncounted warning bucket in hand-written `grammar.y`/`lex.l` — FOUND and
  FIXED (39 warnings).** Every warning count logged in this file up through Category F (368, 195,
  116, 27, 15) was measured from builds that never actually recompiled `grammar.c`/`lex.c` — these
  are bison/flex output, not `.co2`-translated, so nothing in this session's "clean rebuild"
  procedure (which only deleted `.co2`-derived `.c`/`.h`/`.d`) ever forced them to rebuild; `make`
  correctly saw their `.o`s as up to date (since `grammar.y`/`lex.l` themselves were untouched) and
  silently reused stale objects from early in the session. Found while investigating why dropping
  `-fcommon` appeared to add ~40 new warnings — it didn't; a fully-thorough `make clean` finally
  forced them to recompile for the first time, surfacing warnings that were *always* there,
  completely independent of `-fcommon`. Since `grammar.y`/`lex.l` are hand-written C mixed with
  bison/flex, not code the carbon *compiler* generates, none of Categories A–G's compiler-side
  fixes could ever have reached them — these needed manual casts added directly in the two source
  files, same shapes as the categories above: ~24 `RefList.append(RefObject)` calls passed a more
  specific AST node type (cast to `(struct RefObject*)`); ~6 `List.merge(List)` calls passed a
  concrete `RefList*` (cast to `(struct Object*)`, plus the one call that assigns `merge`'s return
  also needed `(struct RefList*)` on the assignment); 4 `.map_args()` calls passed callbacks with
  narrower first-parameter types (cast to `(void (*)(struct RefObject*, va_list*))`, matching
  Category D's mechanism by hand); 5 plain field-assignment/return-value mismatches (`decl->body =
  O_CALL(...);`-style, matching Category F's mechanism by hand); 4 in `lex.l`'s `Map`/`List`
  interface calls (`get`/`put`/`add` all expect `RefObject*`, got `String*`/`File*`). Verified via
  full clean rebuild + `make check`: 90/90, zero regressions, all 39 gone.
- **`-fcommon` is fully droppable, right now, with everything already fixed above — VERIFIED.**
  Empirically tested rather than guessed at: full clean rebuilds of `co2` (`make check` 1/1),
  `co2-base` (`make check` 5/5), and `carbon` (`make check` 90/90) with plain `CFLAGS="-std=gnu89 -g
  -O2"` (no `-fcommon`) all succeed with the exact same 5 residual warnings (Category G, above) as
  with `-fcommon` present — dropping the flag changes nothing, confirming item #4's own suspicion
  that the `lex.l` `extern`-fix from an earlier session was already sufficient. **Item #4's planned
  `path`-global refactor to a lazy-singleton getter turned out to be unnecessary** — leaving it as
  documented direction below in case a *future* hand-written global runs into the same issue, but
  it's not blocking anything right now.
- **`-std=gnu17` (dropping the `89`, keeping `-fcommon` or not) is a separate, bigger effort — not
  yet attempted successfully.** Tried it directly: the 5 Category G warnings become **hard errors**
  under `gnu17` (`-Wincompatible-pointer-types` is error-by-default in modern C dialects, not just
  a warning) — so Category G is no longer optional once this is attempted, it must be fixed first.
  On top of that, `gnu17`'s stricter K&R-implicit-declaration rules surfaced entirely new errors
  unrelated to any category here: `Compiler.c`'s call to `parse` (bison-generated `yyparse`'s
  wrapper), `grammar.c`'s own call to `yylex`, and `grammar.y`'s call to `error` (the parser's error
  hook) are all missing proper prototypes before use, relying on C89's implicit-declaration
  leniency. Real, separate follow-on work: fix Category G (see its own entry above) *and* add
  proper forward declarations for these three, before `gnu17` can be attempted again.

**Current recommended `CFLAGS`**: `-std=gnu89 -g -O2` (just dropped `-fcommon` — verified above).
Still `-std=gnu89`, not plain `c89`/`c17`, since GNU nested functions/`typeof`/statement-expressions
are used throughout `Object.h`/`Interface.h`/carbon's own codegen (item #3, unaffected by any of
this). **1 warning remains** (`RefList.filter()`'s uncast return value, Category G's last piece —
see above), down from a 368 original baseline, none of them blocking a successful build — just
diagnostic noise pending that fix. Moving to `-std=gnu17` is real, valuable, separate follow-on work
(fix that last warning so Category G's errors-under-`gnu17` concern is moot, + add the three missing
prototypes noted above), not attempted further this session. Full mechanism/history for all of this
preserved at `~/.claude/plans/abundant-percolating-brooks.md`.

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

## 4. `path` global's fragile sharing pattern — no longer blocking anything

Fixed the acute symptom (added `extern` to `lex.l`'s declaration so it doesn't create a second
tentative definition that collides with the real one in `io.c`/`Compiler.c`). That turned out to be
the *whole* fix needed: empirically verified (see item #2) that `-fcommon` is now fully droppable
from `CFLAGS` with zero build or test differences across `co2`/`co2-base`/`carbon` — this was the
only place relying on old-style tentative-definition merging after all.

The underlying pattern itself — a bare global shared by name across a hand-written file and
generated files, relying on both sides declaring compatible types — is still fragile by
construction, just not currently causing any problem. **Direction, if it ever bites again**: this
codebase already has the right shape sitting right next to it — `file_hash_map`/`get_hash_map()` in
`File.co2` is a lazily-initialized singleton behind a getter, not a raw shared global. Convert
`path` (the include search path list) to the same shape.

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
