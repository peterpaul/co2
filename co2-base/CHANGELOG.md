# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

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

[Unreleased]: https://github.com/peterpaul/co2/compare/libco2-base-0.3.1...HEAD
[0.3.1]: https://github.com/peterpaul/co2/releases/tag/libco2-base-0.3.1
[0.3.0]: https://github.com/peterpaul/co2/releases/tag/libco2-base-0.3.0
[0.1.1]: https://github.com/peterpaul/co2/releases/tag/libco2-base-0.1.1
[0.1.0]: https://github.com/peterpaul/co2/releases/tag/libco2-base-0.1.0
