# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.3.2] - 2026-07-30

### Added

- `CHANGELOG.md`, in [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) format.

### Fixed

- `README.md` and `CHANGELOG.md` are now actually included in `make dist` tarballs
  (previously silently missing — only the plain `README`/`ChangeLog` were).

## [0.3.1] - 2026-07-28

### Fixed

- Vtable slot assignment mismatch: `O_OBJECT_METHOD`/`O_OBJECT_IF_METHOD*` now cast against
  the base-declared vtable slot type, not the override's own type (TODO #2, Category A).
- `Interface.h`'s `O_BRANCH_CALL_IF` ternary mixing a pointer operand with integer `true`.
- macOS build portability (shared across `co2`, `co2-base`, and `carbon`).
- Windows/MinGW build: `__STRING` fallback (MinGW has no `<sys/cdefs.h>`), `ASSERT_FAIL`'s
  non-glibc branch.

### Added

- `README.md`.

## [0.3.0] - 2015-12-27

### Fixed

- Portability fixes.

## [0.2.1] - 2012-12-17

### Changed

- Add/update license and copyrights.

## [0.2.0] - 2012-12-15

### Changed

- Refactor: `libco2` is now the minimal runtime library implemented in C, with objects that
  cannot be implemented in Carbon.

## [0.1.3] - 2012-05-21

### Added

- `O_IS_IMPLEMENTATION` macro to check whether an Object implements an interface.

## [0.1.2] - 2011-05-29

### Added

- `o_branch_cast_interface` function, and macros `O_CAST_INTERFACE` and
  `O_BRANCH_CAST_INTERFACE`.
- `O_GET_ARG_IF` and `O_BRANCH_GET_ARG_IF` macros to obtain interface arguments from `va_arg`.

### Changed

- Extracted `O_IMPLEMENT_IF_BINDING` macro from `O_IMPLEMENT_IF`.
- More verifications in `O_CALL_IF` and `O_BRANCH_CALL_IF` when `O_DEBUG` is defined.

## [0.1.1] - 2011-05-15

### Added

- `O_BRANCH_CALL_IF` macro.

## [0.1.0] - 2011-05-09

### Added

- Initial release.

[Unreleased]: https://github.com/peterpaul/co2/compare/libco2-0.3.2...HEAD
[0.3.2]: https://github.com/peterpaul/co2/releases/tag/libco2-0.3.2
[0.3.1]: https://github.com/peterpaul/co2/releases/tag/libco2-0.3.1
[0.3.0]: https://github.com/peterpaul/co2/releases/tag/libco2-0.3.0
[0.2.1]: https://github.com/peterpaul/co2/releases/tag/libco2-0.2.1
[0.2.0]: https://github.com/peterpaul/co2/releases/tag/libco2-0.2.0
[0.1.3]: https://github.com/peterpaul/co2/releases/tag/libco2-0.1.3
[0.1.2]: https://github.com/peterpaul/co2/releases/tag/libco2-0.1.2
[0.1.1]: https://github.com/peterpaul/co2/releases/tag/libco2-0.1.1
[0.1.0]: https://github.com/peterpaul/co2/releases/tag/libco2-0.1.0
