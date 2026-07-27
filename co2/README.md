# libco2

Object system for plain C. Gives C classes, interfaces, singletons, and
exceptions, no code generation needed — everything is plain macros and
structs. Foundation library for the [co2 project family](../README.md)
(`co2-base` and the `carbon` compiler are both built on top of it).

Licensed under the LGPL.

## Features

- **Classes** with single inheritance and virtual methods (`O_CLASS`,
  `O_METHOD_DEF`, `O_IMPLEMENT`, `O_OBJECT` / `O_OBJECT_END`)
- **Interfaces** or protocols independent of the class hierarchy
- **Singletons**
- **Exceptions** (`co2_exception.h`)
- Small set of general helpers (`utils.h`)

Core headers live in `src/co2/`: `Object.h`, `Interface.h`, `Singleton.h`,
`co2_exception.h`, `utils.h`.

## Installation

Standard GNU Autotools build:

```bash
./autogen.sh
./configure --prefix=$HOME/local
make
make install
```

This installs the library and headers plus a `libco2-1.0.pc` pkg-config
file. If you install outside a default prefix, make sure
`PKG_CONFIG_PATH` includes `$PREFIX/lib/pkgconfig` so other projects
(`co2-base`, `carbon`) can find `libco2-1.0` via `pkg-config`.

## Usage example

A minimal class definition, `MyObject.h`:

```c
#include "co2/Object.h"

O_METHOD_DEF(MyObject, int, getValue, (void *_self));
O_METHOD_DEF(MyObject, int, setValue, (void *_self, int value));

#define MyObjectClass_Attr			\
	ObjectClass_Attr;			\
	O_METHOD(MyObject, getValue);		\
	O_METHOD(MyObject, setValue)

#define MyObject_Attr				\
	Object_Attr;				\
	int value

O_CLASS(MyObject, Object);
```

and `MyObject.c`:

```c
#include "MyObject.h"

#define O_SUPER Object()

O_IMPLEMENT(MyObject, void *, ctor, (void *_self, va_list * argp)) {
	struct MyObject *self = O_CAST(_self, MyObject());
	self->value = va_arg(*argp, int);
	return self;
}

O_IMPLEMENT(MyObject, int, getValue, (void *_self)) {
	struct MyObject *self = O_CAST(_self, MyObject());
	return self->value;
}

O_IMPLEMENT(MyObject, int, setValue, (void *_self, int value)) {
	struct MyObject *self = O_CAST(_self, MyObject());
	self->value = value;
	return value;
}

O_OBJECT(MyObject, Object);
O_OBJECT_METHOD(MyObject, ctor);
O_OBJECT_METHOD(MyObject, getValue);
O_OBJECT_METHOD(MyObject, setValue);
O_OBJECT_END
```

Used like any other C object:

```c
void *obj = new(MyObject(), 42);
int v = getValue(obj);
setValue(obj, v + 1);
delete(obj);
```

Compile against libco2 with pkg-config:

```bash
gcc $(pkg-config --cflags --libs libco2-1.0) MyObject.c main.c -o myapp
```

A complete, buildable version of this example is in
[`examples/my-object`](../examples/my-object).

## Tests

```bash
cd test
make check
```
