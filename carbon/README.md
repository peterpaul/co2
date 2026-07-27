# carbon

Compiler and general-purpose programming language that compiles to C,
built on top of [libco2](../co2) and [libco2-base](../co2-base). Carbon
adds classes, interfaces, exceptions, and other syntactic sugar over
libco2's C object system, then generates plain C source from `.co2`
files.

Licensed under the GPL.

Note: despite living in this bootstrap repo, Carbon is meant as a real
general-purpose language, not just an internal build tool — see
`TODO` for the language roadmap.

## Bootstrap note

The compiler's own sources (`src/co2/*.co2`, e.g. `Compiler.co2`,
`Grammar.co2`) are themselves written in Carbon, and the compiler links
against `libco2` and `libco2-base`. A working `carbon` binary must
already be installed before `carbon` (or `co2-base`) can be rebuilt
from `.co2` sources — see the top-level [repo README](../README.md) and
`CLAUDE.md` for the full cycle.

## Installation

```bash
./autogen.sh
./configure --prefix=$HOME/local
make
make install
```

`configure` requires `pkg-config` to find `libco2-1.0` and
`libco2-base-1.0` (set `PKG_CONFIG_PATH` if they're installed to a
non-default prefix), plus `lex`/`yacc` (or `flex`/`bison`) to build the
lexer/parser. Installing produces the `carbon` binary and a
`carbon-1.0.pc` pkg-config file.

For quick iteration inside `src/` without a full `configure`:

```bash
cd src
./build.sh          # runs make -k twice, past the .co2-generation pass
./build.sh clean     # removes generated .c/.h sources
```

## Usage

```
carbon [OPTION]... [SOURCE_FILE] [C_OUTPUT_FILE]

ARGUMENTS:
        SOURCE_FILE:    carbon source file
        C_OUTPUT_FILE:  filename of c output

OPTIONS:
        -I INCLUDE_DIR: include directory to add to search path
        -h,--help:      show this help
        -V,--version:   show version
        -d,--depend:    show dependencies
```

Example, `MyObject.co2`:

```
class MyObject {
	int value;

	MyObject(int value) {
		self.value = value;
	}

	int getValue() {
		return value;
	}

	int setValue(int value) {
		return self.value = value;
	}
}
```

Compile it to C, then build against `libco2`:

```bash
carbon MyObject.co2 MyObject.c
gcc $(pkg-config --cflags --libs libco2-1.0) MyObject.c main.c -o myapp
```

A complete, buildable example project is in
[`examples/my-object-carbon`](../examples/my-object-carbon).

## Tests

Custom shell-driven suite (not wired into `make check`):

```bash
cd test
./run_tests.sh
```

Runs every `*.test` file under `test/pass/` (must compile and run
successfully) and `test/fail/` (must fail to compile) against the
freshly built `../src/carbon` binary, logging results to
`test/target/`. Run a single test directly with:

```bash
./run_pass_test.sh path/to/file.test
./run_fail_test.sh path/to/file.test
```

`generate-testcases.sh` / `generate-makefile-am.sh` regenerate the test
list after adding or removing `.test` files.
