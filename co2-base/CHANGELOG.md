# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Fixed

- `BaseObject.getClass()` now explicitly casts its return value to `Class`. Every generated class
  struct redeclares its own `class` field with a more specific covariant type (`struct
  BaseObjectClass *`, not the base `struct Class *`) as a deliberate codegen convenience, so
  returning the field bare (`return class;`) from a method declared to return the base type is a
  real return-type mismatch — harmless under `-std=gnu89`, but a hard build error under newer C
  standards (part of an ongoing audit into what blocks moving off `gnu89`).

### Removed

- `BaseObject.co2`'s `int true, false;` declaration. `true`/`false` are recognized natively by the
  compiler as of carbon 0.3.4 (see its `bool` type, TODO #9); this declaration only existed so the
  old compiler would accept them as ordinary identifiers.

## [0.3.2] - 2026-07-30

### Added

- `CHANGELOG.md`, in [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) format.

### Fixed

- `README.md` and `CHANGELOG.md` are now actually included in `make dist` tarballs
  (previously silently missing — only the plain `README`/`ChangeLog` were).

## [0.3.1] - 2026-07-28

### Fixed

- macOS build portability (shared across `co2`, `co2-base`, and `carbon`).
- Windows/MinGW build: `random()` -> `rand()`, `localtime_r()` -> `localtime()`, and a
  pointer-punning crash from MinGW-w64's 32-bit `struct timeval.tv_sec` vs. 64-bit `time_t`
  (new `time_compat.c`/`.h` shim).
- `time_compat.c`/`.h` being caught by the generated-files `.gitignore` rule.

### Changed

- Marked `TestLogger` as an expected failure (XFAIL) on MinGW/Windows only.

### Added

- `README.md`.

## [0.3.0] - 2015-12-27

### Fixed

- Portability fixes.

## [0.1.1] - 2012-12-17

### Added

- License/copyrights.

### Fixed

- Do not distribute `.d` files.

## [0.1.0] - 2012-12-15

### Added

- Initial release.

[Unreleased]: https://github.com/peterpaul/co2/compare/libco2-base-0.3.2...HEAD
[0.3.2]: https://github.com/peterpaul/co2/compare/libco2-base-0.3.1...libco2-base-0.3.2
[0.3.1]: https://github.com/peterpaul/co2/compare/libco2-base-0.3.0...libco2-base-0.3.1
[0.3.0]: https://github.com/peterpaul/co2/compare/libco2-base-0.1.1...libco2-base-0.3.0
[0.1.1]: https://github.com/peterpaul/co2/compare/libco2-base-0.1.0...libco2-base-0.1.1
[0.1.0]: https://github.com/peterpaul/co2/releases/tag/libco2-base-0.1.0
