# co2

[![CI](https://github.com/peterpaul/co2/actions/workflows/ci.yml/badge.svg)](https://github.com/peterpaul/co2/actions/workflows/ci.yml)
[![Release](https://github.com/peterpaul/co2/actions/workflows/release.yml/badge.svg)](https://github.com/peterpaul/co2/actions/workflows/release.yml)

This repository contains three Open Source projects, built and released independently but tightly
coupled by a self-hosting bootstrap cycle (see below):

| Project | Description | License | Docs |
|---|---|---|---|
| [`co2/`](co2) — **libco2** | Object system for plain C: classes, interfaces, singletons, exceptions. Pure C, no code generation. Foundation everything else builds on. | LGPL | [co2/README.md](co2/README.md) |
| [`co2-base/`](co2-base) — **libco2-base** | Collections/utility library (`ArrayList`, `HashMap`, `String`, logging, exceptions, ...), written in the Carbon language and compiled to C by the `carbon` compiler. | LGPL | [co2-base/README.md](co2-base/README.md) |
| [`carbon/`](carbon) — **carbon** | The Carbon compiler and language itself. Confusingly, its own sources are *also* written in Carbon, and it links against `libco2`/`libco2-base`. Meant as a real general-purpose language, not just this repo's internal build tool. | GPL | [carbon/README.md](carbon/README.md) |

`examples/my-object` and `examples/my-object-carbon` are minimal, standalone, buildable sample
projects showing how to consume `libco2` from plain C vs. from Carbon — see each sub-README's
"Usage" section for the source snippets they're built from.

## The bootstrap cycle

`carbon` needs `libco2-base` to build (via `pkg-config libco2-base-1.0`), but `libco2-base`'s
`.co2` sources need an already-installed `carbon` binary to compile to `.c`/`.h`. In practice:

- A working `carbon` binary must already exist (from a previous release) before `co2-base` or
  `carbon` itself can be rebuilt from `.co2` sources.
- Generated `.c`/`.h` files from `.co2` sources are never checked into git.
- Bumping the Carbon language/compiler follows: install current `carbon` → rebuild `co2-base` →
  rebuild `carbon` against it → reinstall → repeat if the language itself changed.
- Release commits alternate between `carbon` and `libco2-base` version bumps for exactly this
  reason.

CI's own `translate` job (see below) automates this same dance from a clean checkout, using each
project's most recently cut release as the bootstrap seed — see `.github/scripts/translate.sh` and
`TODO.md` item #1 for the full mechanism, and each project's own README for its "Bootstrap note"
section.

**Adding a new compiler (language) feature?** It can't be used in this repo's own `.co2` sources
until a release built from it becomes the bootstrap seed above — the old bootstrap `carbon` simply
can't parse syntax it predates. See `CLAUDE.md`'s "New compiler (language) features..." section for
the concrete rule and a worked example, and `carbon/README.md`'s "Tests" section for why
`carbon/test/`'s own fixtures (not self-compilation) are how such a feature actually gets verified.

## Building

Each of `co2/`, `co2-base/`, `carbon/`, and the `examples/*` projects is a standalone Autotools
project (own `configure.ac`, `Makefile.am`). From inside a project directory:

```bash
./autogen.sh
./configure --prefix=$HOME/local
make
make install
```

`libco2-1.0`/`libco2-base-1.0` must be discoverable via `pkg-config` for `co2-base`/`carbon`
respectively (set `PKG_CONFIG_PATH` if installing outside the default prefix), and `co2-base`/
`carbon` need a `carbon` binary on `PATH` to translate `.co2` sources not already generated. See
each project's own README for exact prerequisites, its `src/build.sh` fast-iteration wrapper, and a
runnable usage example.

## Testing

- `co2/test`, `co2-base/test`: standard Autotools `make check`.
- `carbon/test`: custom shell-driven suite (`./run_tests.sh`), not wired into `make check` — see
  [carbon/README.md](carbon/README.md#tests).

## CI and releases

`.github/workflows/ci.yml` builds and tests all three projects on Linux, macOS, and Windows on
every push/PR, using a two-stage design: a single Linux job translates `.co2` → `.c` once
(`translate.sh`), then a build-and-test matrix job builds+tests each platform from that shared,
pre-translated output with a plain C toolchain — no per-platform bootstrap dance repeated.

`.github/workflows/release.yml` publishes a GitHub Release whenever a tag matching this repo's
`<project>-<version>` convention is pushed (e.g. `carbon-0.3.2`, `libco2-base-0.3.1`,
`libco2-0.3.1`) — each project releases independently. See `TODO.md` item #1 for the full pipeline
design and current status.

## History

These projects started out as an experiment to investigate how to do Object Oriented Programming
(OOP) in plain C. After some experiments and several redesigns I came up with a small library that
evolved into what now is `libco2`.

I used `libco2` for several experiments, including a lexical analyzer, a configuration file parser,
a sudoku solver, and a raytracer.

Then I realized that without the syntactic sugar of the `libco2` library I could develop faster, so
I started building the `carbon` compiler and programming language — created with `libco2`, of
course.

## More information

- [`TODO.md`](TODO.md) — running list of known issues, in-progress work, and design notes across
  all three projects.
- [`CLAUDE.md`](CLAUDE.md) — repository conventions and the full bootstrap-cycle explanation, for
  anyone (human or AI) working on this codebase.
