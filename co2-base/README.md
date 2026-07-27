# libco2-base

Collections and utility library for [libco2](../co2): array lists, double
linked lists, hash maps, strings, logging, exceptions, and more. Written
in the **Carbon language** (`.co2` files under `src/co2/`) and compiled
to plain C by the `carbon` compiler.

Licensed under the LGPL.

## Bootstrap note

Building this project from `.co2` sources requires an already-installed
`carbon` compiler (found via `PATH`/`configure`). See the top-level
[repo README](../README.md) and `CLAUDE.md` for the full bootstrap-cycle
explanation: `carbon` itself depends on `libco2-base` to build, so a
prior release of `carbon` must be installed first.

## Features

Selected classes from `src/co2/` (each `X.co2` compiles to `X.c`/`X.h`):

- Collections: `Array`, `ArrayList`, `ArrayIterator`, `DoubleLinkedList`,
  `DoubleLinkedListIterator`, `DoubleLinkedItem`, `HashMap`, `RefList`
- Interfaces: `Collection`, `Comparable`, `Iterable`, `List`
- Core types: `RefObject`, `BaseObject`, `String`, `Enum`
- Exceptions: `Exception`, `ClassCastException`
- Logging: `Logger`, `ConsoleHandler`

## Installation

```bash
./autogen.sh
./configure --prefix=$HOME/local
make
make install
```

`configure` requires `libco2-1.0` to be discoverable via `pkg-config`
(set `PKG_CONFIG_PATH` if `libco2` is installed to a non-default
prefix), and requires a `carbon` binary on `PATH` to regenerate the
`.c`/`.h` files from `.co2` sources if they are not already present.
Installing produces a `libco2-base-1.0.pc` pkg-config file for
downstream projects.

For quick iteration inside `src/` without a full `configure`, use:

```bash
cd src
./build.sh          # runs make -k twice, past the .co2-generation pass
./build.sh clean     # removes generated .c/.h sources
```

## Usage example

`ArrayList` written in Carbon (`src/co2/ArrayList.co2`, abbreviated):

```
include co2/Array
include co2/List

class ArrayList : Array, List {
	unsigned length;

	ArrayList () {
		super (128);
		length = 0;
	}

	RefObject add(RefObject item) {
		ensure (length + 1);
		set (length, item);
		length += 1;
		return item;
	}

	Iterator iterator () {
		return new ArrayIterator (self);
	}
}
```

Using the compiled result from C:

```c
#include "co2/ArrayList.h"
#include "co2/String.h"

void *list = new(ArrayList());
add(list, new(String(), "hello"));
add(list, new(String(), "world"));

void *it = iterator(list);
while (hasNext(it)) {
	void *item = next(it);
	printf("%s\n", c_str(item));
}
delete(it);
delete(list);
```

Compile against both libraries:

```bash
gcc $(pkg-config --cflags --libs libco2-1.0 libco2-base-1.0) main.c -o myapp
```

## Tests

```bash
cd test
make check
```
