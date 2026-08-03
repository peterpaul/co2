# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- `configure.ac` now defaults `CFLAGS` to `-g -O2 -std=gnu17` when the caller doesn't supply one, so
  a plain `autogen.sh`/`configure`/`make` run gets a known-working modern C standard instead of
  whatever the local compiler happens to default to (`gnu89` on some systems, as new as `gnu23` on
  others — which is exactly how the fixes below were found in the first place). CI's own explicit
  `CFLAGS` (currently `-std=gnu89`, for bootstrap-tarball compatibility) is unaffected.

### Fixed

- `parse()`, `yylex()`, and `error()` are now properly declared before use in the compiler's own
  generated C (`grammar.y`'s prologue for the latter two; a small `Makefile.am` rule injecting
  `parse()`'s prototype into bison's generated `grammar.h`, since old bison has no portable way to
  put arbitrary declarations there). Relied on implicit-function-declaration being merely a warning
  under `-std=gnu89`; a hard error under newer C standards (part of an ongoing audit into what
  blocks moving off `gnu89`).
- `expression_generate_casted()` now also recognizes `self.class`/bare `class` field access
  (`TokenExpression` wrapping a `_CLASS` token with a resolved `class_decl`) as needing a cast to
  its target type, the same way it already did for `SomeClass.class` literals. Every class's
  generated struct redeclares its own `class` field with a more specific covariant type (`struct
  XClass *`, not the base `struct Class *`) as a deliberate codegen convenience; type_check()
  already verifies compatibility with the declared type, so the cast is always safe. Fixes
  `co2-base`'s `BaseObject.getClass()` (and any other bare `return class;`-style code) without
  needing a source-level workaround there.
- `ConditionalExpression.generate()` now casts both `?:` branches to the common type `type_check()`
  already resolved for them (either branch's own type when directly compatible, or their nearest
  common base class otherwise), instead of emitting each branch's code completely uncast. Harmless
  under `-std=gnu89`; a hard error under newer C standards when the two branches are different
  subclasses (`struct B *`/`struct C *` assigned to a `struct A *` common base).
- `GenerateSourceIncludesVisitor` now emits `#include <math.h>` whenever a `%`/`^` (or their
  compound-assign forms) binary expression on `float` operands is used anywhere in a file.
  `BinaryExpression.generate()` codegens these as calls to `fmodf()`/`powf()`, but unlike every
  other external declaration in this codebase they were never modeled with a `Declaration`/
  `include_file` for the existing include-collection mechanism to find - so `math.h` was never
  added, relying on implicit-function-declaration to paper over the missing prototypes.

### Changed

- Migrated this codebase's own compiler sources off the `typedef Bool = int;` convention onto the
  real `bool` type added in 0.3.4 (TODO #9), now that a release with `bool` can be used as
  `translate.sh`'s bootstrap seed. Renamed `Bool` to `bool` throughout `carbon/src/co2/`.

### Removed

- The `typedef Bool = int;` declaration itself (`CompileObject.co2`), now that every usage has
  migrated to the real `bool` type.

## [0.3.4] - 2026-08-01

### Added

- Native `bool` type, parsed like `int`/`char`/`float` and generated as plain `int` (no native bool
  in `-std=gnu89`). Comparisons, `&&`/`||`, and unary `!` now produce a `Bool`-typed result. `true`/
  `false` are recognized by name rather than as reserved keywords, and `Bool` is compatible with
  `int`/`unsigned` rather than a strictly separate type — both deliberate bootstrap-safety choices,
  see TODO #9. This codebase's own `typedef Bool = int;` isn't removed yet; that's a deliberate
  follow-up once a release built from this becomes the bootstrap seed.

### Fixed

- `continue` no longer crashes the compiler. `ContinueStatement` was never wired into
  `BaseCompileObjectVisitor`'s dispatch at all; every `continue;` fell through to a generic error
  path that called an abstract, never-overridden `get_token()` (TODO #8).
- Bitwise XOR (`#`/`##`) and compound XOR-assign (`#=`) no longer generate invalid C. Codegen was
  passing these tokens' literal text straight through instead of translating to C's `^`/`^=` (TODO
  #8).
- `Expression.set_scope()`'s overrides in `TokenExpression`/`BinaryExpression` now declare the same
  `Scope` parameter type as the base method, instead of the interface type `IScope`. A long-standing
  (2011/2012) override-signature mismatch that only ever produced a warning; surfaced as a hard
  build error on toolchains that promote `-Wincompatible-function-pointer-types` to an error by
  default.

## [0.3.3] - 2026-07-30

### Added

- `[<header.h>]` angle-bracket include syntax, alongside the existing `["header.h"]`
  (TODO #7). The existing quoted form still normalizes to angle-bracket output for now —
  this repo's own 15 uses can't migrate to the new syntax until a carbon release built
  from this change becomes the bootstrap seed.
- `CHANGELOG.md`, in [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) format.

### Fixed

- `RefList.filter()`'s vtable return-type cast: root-caused and fixed the long-standing
  "incompatible types: Bool and Bool" mystery (TODO #2, Category G's last warning). The
  base-resolved return type from `find_base_function_declaration()` wasn't being
  type-checked before use, leaving its `ObjectType.decl` unresolved and causing spurious
  mismatches against an already-resolved `ObjectType` for the same typedef.
- `README.md` and `CHANGELOG.md` are now actually included in `make dist` tarballs
  (previously silently missing — only the plain `README`/`ChangeLog` were).

## [0.3.2] - 2026-07-28

### Added

- Support for pointer arithmetic (`ptr +/- int`) in the type checker.
- `README.md`.

### Changed

- Replaced qsort-based declaration ordering with dependency-based repair (fixes
  nondeterministic ordering bugs).

### Fixed

- Vtable slot assignment/call mismatches across several categories: base-declared vtable
  slot types now used consistently for assignment casts, call arguments,
  `map()`/`map_args()` callback pointers, `SomeClass.class` literal arguments, and
  assignment/return-statement mismatches (TODO #2, Categories A-F).
- `["header.h"]` external declarations now emit angle-bracket `#include`s.
- `ObjectType.toString()`/`to_string()` naming mismatch (every type-mismatch error message
  was printing a generic "ClassName at 0xADDRESS" instead of the real type name).
- The last 2 flaky carbon test fixtures (90/90 passing).
- macOS build portability (shared across `co2`, `co2-base`, and `carbon`).
- Windows/MinGW build: `realpath()` -> `_fullpath()`, dropped a redundant `-lc` from the
  test harness's linker flags, strip `\r` before diffing pass-test output.
- `IncludeStack.h`'s carbon rule now gated on `CARBON_EXISTS` like every other rule.
- `run_tests.sh` is now location-independent.

## [0.3.1] - 2018-02-07

### Fixed

- Compilation fix.

## [0.3.0] - 2015-12-27

### Fixed

- Portability fixes.

## [0.2.1] - 2012-12-17

### Added

- License/copyrights.

### Fixed

- Do not distribute `.d` files.

## [0.2.0] - 2012-12-15

### Changed

- Rewrote the carbon compiler in Carbon itself.

## [0.1.5] - 2012-05-21

### Added

- Allow identifiers in switch-case statements.
- `<Class>.class` expression.

### Changed

- Operator precedence for `IS_OF`, to parse `!obj is_of class`.
- Allow the use of `self` in interface methods, and raise an error when used incorrectly.
- Use a macro to check for implementation of an interface.
- Generate "interface instances" as `Object`.

### Fixed

- Visitor now visits the whole tree.
- Handle nested functions in class methods properly.
- Look up identifiers upon declaration.

## [0.1.4] - 2011-11-10

### Added

- `-d`/`--depend` option to generate dependency information for Makefiles.
- Mixins: allow interface methods to have an implementation.
- Nested functions.
- Syntax to avoid name clashes when implementing multiple interfaces.

### Changed

- Renamed the `-P` option to `-I`, to align better with existing compilers.

### Fixed

- Use a conditional cast when casting objects or interfaces.

## [0.1.3] - 2011-09-12

### Added

- Switch-case statement.

### Changed

- Syntax for implementing interfaces: `[ ',' INTERFACE ]*`.

### Fixed

- Generate `#include` statements for all externally defined tokens.
- Do not generate a function when the body is undefined.

## [0.1.2] - 2011-05-29

### Fixed

- Raise an error when a function expression is not a function type.
- Set the parent scope of class members before type checking.
- Add a typedef for undeclared types first used in a function prototype.

## [0.1.1] - 2011-05-15

### Added

- `ConditionalBinaryExpression`.
- Allow an expression for the class in an `IS_OF` expression.

## [0.1.0] - 2011-05-09

### Added

- Initial release.

[Unreleased]: https://github.com/peterpaul/co2/compare/carbon-0.3.4...HEAD
[0.3.4]: https://github.com/peterpaul/co2/compare/carbon-0.3.3...carbon-0.3.4
[0.3.3]: https://github.com/peterpaul/co2/compare/carbon-0.3.2...carbon-0.3.3
[0.3.2]: https://github.com/peterpaul/co2/compare/carbon-0.3.1...carbon-0.3.2
[0.3.1]: https://github.com/peterpaul/co2/compare/carbon-0.3.0...carbon-0.3.1
[0.3.0]: https://github.com/peterpaul/co2/compare/carbon-0.2.1...carbon-0.3.0
[0.2.1]: https://github.com/peterpaul/co2/compare/carbon-0.2.0...carbon-0.2.1
[0.2.0]: https://github.com/peterpaul/co2/compare/carbon-0.1.5...carbon-0.2.0
[0.1.5]: https://github.com/peterpaul/co2/compare/carbon-0.1.4...carbon-0.1.5
[0.1.4]: https://github.com/peterpaul/co2/compare/carbon-0.1.3...carbon-0.1.4
[0.1.3]: https://github.com/peterpaul/co2/compare/carbon-0.1.2...carbon-0.1.3
[0.1.2]: https://github.com/peterpaul/co2/compare/carbon-0.1.1...carbon-0.1.2
[0.1.1]: https://github.com/peterpaul/co2/compare/carbon-0.1.0...carbon-0.1.1
[0.1.0]: https://github.com/peterpaul/co2/releases/tag/carbon-0.1.0
