# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository overview

This repo is a bootstrap chain of three GNU Autotools C projects, built and released independently but tightly coupled by a self-hosting dependency cycle:

1. **`co2/`** — `libco2` (LGPL): base object system for plain C (classes, interfaces, singletons, exceptions). Pure C, no code generation. This is the foundation everything else builds on.
2. **`co2-base/`** — `libco2-base` (LGPL): collections/utility library (`ArrayList`, `HashMap`, `String`, logging, exceptions, etc.) written in the **Carbon language** (`.co2` files under `src/co2/`), compiled to C by the `carbon` compiler.
3. **`carbon/`** — the Carbon compiler and language itself (GPL). Confusingly, the compiler's own sources (`src/co2/*.co2`, e.g. `Compiler.co2`, `Grammar.co2`) are *also* written in Carbon, and it links against `libco2` and `libco2-base`.

`examples/my-object` and `examples/my-object-carbon` are minimal sample projects showing how to consume `libco2` from plain C vs. from Carbon.

### The bootstrap cycle

`carbon` requires `libco2-base` to build (via `pkg-config libco2-base-1.0`), but `libco2-base`'s `.co2` sources require an already-installed `carbon` binary to compile to `.c`/`.h` (see the `%.c: %.co2` rules in `co2-base/src/Makefile.am` and `carbon/src/Makefile.am`, gated by the `CARBON_EXISTS` automake conditional). In practice this means:

- A working `carbon` binary must already be installed (from a previous release) before `co2-base` or `carbon` itself can be rebuilt from `.co2` sources.
- Generated `.c`/`.h` files from `.co2` sources are `BUILT_SOURCES`, not checked into git — do not add them to version control.
- When bumping the Carbon language/compiler, the usual sequence is: install current `carbon` → rebuild `co2-base` → rebuild `carbon` against it → reinstall → repeat if the language itself changed.
- Release commits alternate between `carbon` and `libco2-base` version bumps (see git log) precisely because of this cycle.

## Build commands

Each of `co2/`, `co2-base/`, `carbon/`, and the `examples/*` projects is a standalone autotools project (own `configure.ac`, `Makefile.am`). From inside a project directory:

```bash
./autogen.sh && ./configure && make
```

Install to `~/local` is the convention used by the test scripts (`--prefix=$HOME/local`); `pkg-config` must be able to find `libco2-1.0` / `libco2-base-1.0` (set `PKG_CONFIG_PATH` accordingly if installing outside the default prefix).

`co2-base/src/build.sh` and `carbon/src/build.sh` are convenience wrappers around `make -k` (run twice, to get past the first pass generating `.co2`-derived sources) for iterating inside `src/` without a full `configure`. `build.sh clean` removes generated sources.

## Running tests

- **`co2/test`**: standard automake `TESTS` (`TestObject`), run via `make check` from `co2/test`.
- **`carbon/test`**: custom shell-driven suite, not wired into `make check`.
  ```bash
  cd carbon/test
  ./run_tests.sh
  ```
  This runs every `*.test` file under `carbon/test/pass/` (must compile and run successfully) and `carbon/test/fail/` (must fail to compile) against the freshly built `../src/carbon` compiler binary, writing per-test logs to `carbon/test/target/`. A single test can be run directly with `./run_pass_test.sh <path>.test` or `./run_fail_test.sh <path>.test` (both source `run_test_base.sh` for `SRCDIR`/`BUILDDIR` setup). `carbon/test/generate-testcases.sh` / `generate-makefile-am.sh` regenerate the test list when `.test` files are added/removed.
- **`co2-base/test`**: automake-driven; add cases via `Makefile.am`.

## Working with `.co2` source files

Files under any `src/co2/` directory are Carbon language source, not C — despite the `.co2` extension resembling data. Each compiles to a matching `.c`/`.h` pair. When adding a new class to `co2-base` or `carbon`, add the `.co2` file to the corresponding `*_CO2SOURCES` list in that project's `src/Makefile.am` (the `.c`/`.h`/`.d` derivations are computed automatically via substitution).
