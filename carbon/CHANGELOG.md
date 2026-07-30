# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

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

[Unreleased]: https://github.com/peterpaul/co2/compare/carbon-0.3.3...HEAD
[0.3.3]: https://github.com/peterpaul/co2/releases/tag/carbon-0.3.3
[0.3.2]: https://github.com/peterpaul/co2/releases/tag/carbon-0.3.2
[0.3.1]: https://github.com/peterpaul/co2/releases/tag/carbon-0.3.1
[0.3.0]: https://github.com/peterpaul/co2/releases/tag/carbon-0.3.0
[0.2.1]: https://github.com/peterpaul/co2/releases/tag/carbon-0.2.1
[0.2.0]: https://github.com/peterpaul/co2/releases/tag/carbon-0.2.0
[0.1.5]: https://github.com/peterpaul/co2/releases/tag/carbon-0.1.5
[0.1.4]: https://github.com/peterpaul/co2/releases/tag/carbon-0.1.4
[0.1.3]: https://github.com/peterpaul/co2/releases/tag/carbon-0.1.3
[0.1.2]: https://github.com/peterpaul/co2/releases/tag/carbon-0.1.2
[0.1.1]: https://github.com/peterpaul/co2/releases/tag/carbon-0.1.1
[0.1.0]: https://github.com/peterpaul/co2/releases/tag/carbon-0.1.0
