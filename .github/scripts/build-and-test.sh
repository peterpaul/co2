#!/usr/bin/env bash
# Full CI build: bootstrap an old carbon release, then use it to build and
# test HEAD's own co2/co2-base/carbon. See TODO.md item #1 for the plan this
# implements, and .github/scripts/patch-bootstrap.sh for why the bootstrap
# tarballs need patching first.
#
# Two-pass rebuild of co2-base and carbon (see "PASS 2" below) is required,
# not optional: the bootstrap carbon is an old (0.3.1) release that predates
# every carbon-*compiler*-side fix in this repo's history (e.g. the
# angle-bracket-include fix, and every call-site/vtable casting fix from
# TODO.md item #2's Categories A-G). Those are changes to what the compiler
# itself generates, not just new source logic -- so anything the *bootstrap*
# carbon translates (co2-base's and carbon's own .co2 sources) comes out at
# old-compiler quality on the first pass, same as this session's own
# multi-pass bootstrap methodology when iterating on carbon locally. Only
# after HEAD's own carbon has been built once (pass 1) and installed does
# translating everything *again* (pass 2) actually reflect those fixes.
#
# Required environment variables (set by the workflow per-OS):
#   CC       - C compiler to use throughout (a real GCC; see TODO.md item #3
#              for why Clang/MSVC can't build this -- GNU nested functions)
#   WORKDIR  - scratch directory for downloading/extracting/building the
#              bootstrap tarballs and installing everything into
#
# Must be run from the repo root.
set -euo pipefail

: "${CC:?CC must be set}"
: "${WORKDIR:?WORKDIR must be set}"

REPO_ROOT="$(pwd)"
BOOTSTRAP_PREFIX="$WORKDIR/bootstrap-prefix"
HEAD_PREFIX="$WORKDIR/head-prefix"
CFLAGS_COMMON="-std=gnu89 -g -O2"

mkdir -p "$WORKDIR" "$BOOTSTRAP_PREFIX" "$HEAD_PREFIX"
cd "$WORKDIR"

# `make check`'s own PASS/FAIL summary never includes *why* a test failed --
# that detail only lands in each test's own .log file. Dump every one of
# those (plus the aggregate test-suite.log) before failing the build, so a
# CI failure is actually debuggable from the workflow log instead of needing
# a second, instrumented run.
check_with_logs() {
    if ! make check; then
        echo "::group::make check failed -- dumping per-test logs"
        find . -name '*.log' -print -exec cat {} \;
        echo "::endgroup::"
        return 1
    fi
}

# Every shared library built in this script lives in a non-system prefix.
# macOS embeds an absolute install path in each .dylib at link time, so it
# needs nothing extra; Linux's shared objects are referenced by bare SONAME
# (e.g. libco2-1.0.so.0) and need the dynamic linker told where to look, or
# every test that executes a linked binary fails with "cannot open shared
# object file". Harmless to set on macOS too.
export LD_LIBRARY_PATH="$HEAD_PREFIX/lib:$BOOTSTRAP_PREFIX/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

# regen_co2 <project-dir> deletes a project's .co2-derived .c/.h/.d files (and
# libtool's build state) so the next `make` retranslates every .co2 source
# from scratch with whatever `carbon` is currently first on PATH, instead of
# silently reusing files a *different* carbon already generated.
regen_co2() {
    local dir=$1
    (
        cd "$dir/src"
        find . -name '*.co2' | while read -r f; do
            base="${f%.co2}"
            rm -f "${base}.c" "${base}.h" "${base}.d"
        done
        rm -rf .libs
        rm -f *.lo *.la
    )
    make -C "$dir" clean >/dev/null 2>&1 || true
}

echo "::group::Download bootstrap release tarballs"
curl -sL -o libco2-0.3.0.tar.gz https://github.com/peterpaul/co2/releases/download/libco2-0.3.0/libco2-0.3.0.tar.gz
curl -sL -o libco2-base-0.3.0.tar.gz https://github.com/peterpaul/co2/releases/download/libco2-base-0.3.0/libco2-base-0.3.0.tar.gz
curl -sL -o carbon-0.3.1.tar.gz https://github.com/peterpaul/co2/releases/download/carbon-0.3.1/carbon-0.3.1.tar.gz
tar xzf libco2-0.3.0.tar.gz
tar xzf libco2-base-0.3.0.tar.gz
tar xzf carbon-0.3.1.tar.gz
echo "::endgroup::"

echo "::group::Patch bootstrap tarballs"
bash "$REPO_ROOT/.github/scripts/patch-bootstrap.sh" \
    "$WORKDIR/libco2-0.3.0" "$WORKDIR/libco2-base-0.3.0" "$WORKDIR/carbon-0.3.1"
echo "::endgroup::"

echo "::group::Build bootstrap libco2"
cd "$WORKDIR/libco2-0.3.0"
./configure --prefix="$BOOTSTRAP_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON"
make
make install
echo "::endgroup::"

echo "::group::Build bootstrap libco2-base"
cd "$WORKDIR/libco2-base-0.3.0"
export PKG_CONFIG_PATH="$BOOTSTRAP_PREFIX/lib/pkgconfig"
./configure --prefix="$BOOTSTRAP_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$BOOTSTRAP_PREFIX/include/co2-1.0"
make
make install
echo "::endgroup::"

echo "::group::Build bootstrap carbon"
cd "$WORKDIR/carbon-0.3.1"
./configure --prefix="$BOOTSTRAP_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON"
make
make install
"$BOOTSTRAP_PREFIX/bin/carbon" --help > /dev/null
echo "::endgroup::"

# From here on, the bootstrap carbon (now on PATH) translates HEAD's own
# .co2 sources -- none of the bootstrap patches above are needed for HEAD,
# since every one of those bugs is already fixed in this repo's history.
export PATH="$BOOTSTRAP_PREFIX/bin:$PATH"
unset PKG_CONFIG_PATH

echo "::group::Build & test HEAD libco2"
# Pure C, no .co2 translation -- no two-pass concern, build once.
cd "$REPO_ROOT/co2"
./autogen.sh
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON"
make
check_with_logs
make install
echo "::endgroup::"

echo "::group::Build HEAD co2-base (pass 1, bootstrap carbon -- not tested yet)"
regen_co2 "$REPO_ROOT/co2-base"
cd "$REPO_ROOT/co2-base"
export PKG_CONFIG_PATH="$HEAD_PREFIX/lib/pkgconfig"
./autogen.sh
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$HEAD_PREFIX/include/co2-1.0"
# The bootstrap carbon predates the angle-bracket-include fix (this repo's
# own compiler-side fix for exactly this), so it translates BaseObject.co2's
# `["string.h"]` as a quoted include, which collides on a case-insensitive
# filesystem with co2-base's own generated String.h. Let it generate the
# (still-broken) output, patch just that, then continue -- pass 2 below
# retranslates this correctly with HEAD's own (fixed) carbon, so this only
# ever affects pass 1.
(cd src && make co2/BaseObject.c) || true
if [ -f src/co2/BaseObject.h ]; then
    sed -i.bak 's|#include "string.h"|#include <string.h>|' src/co2/BaseObject.h src/co2/BaseObject.c
    rm -f src/co2/BaseObject.h.bak src/co2/BaseObject.c.bak
fi
# Same bootstrap-carbon-only codegen gap: LogRecord.co2 declares `typedef
# Timeval = struct timeval` (and uses it in a field) before the bootstrap
# carbon has emitted that typedef in LogRecord.h -- it emits the typedef only
# at the very end of the file instead of at first use. Insert an early,
# include-guarded copy right after the existing #include block; the original
# (later) block is itself guarded by the same #ifndef, so it becomes a
# harmless no-op once this one has already defined it. Pass 2 retranslates
# this correctly with HEAD's own (fixed) carbon, so this only ever affects
# pass 1.
(cd src && make co2/LogRecord.c) || true
if [ -f src/co2/LogRecord.h ]; then
    sed -i.bak '/^#include "co2\/BaseObject.h"$/a\
struct timeval;\
#ifndef TYPEDEF_Timeval\
#define TYPEDEF_Timeval\
typedef struct timeval Timeval;\
#endif /* TYPEDEF_Timeval */' src/co2/LogRecord.h
    rm -f src/co2/LogRecord.h.bak
fi
make
make install
echo "::endgroup::"

echo "::group::Build HEAD carbon (pass 1, bootstrap carbon translating -- produces the real carbon binary)"
regen_co2 "$REPO_ROOT/carbon"
cd "$REPO_ROOT/carbon"
./autogen.sh
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$HEAD_PREFIX/include/co2-1.0 -I$HEAD_PREFIX/include/co2-base-1.0"
make
make install
echo "::endgroup::"

# HEAD's own carbon now exists and is on PATH ahead of the bootstrap one --
# everything translated from here on reflects every compiler-side fix.
export PATH="$HEAD_PREFIX/bin:$PATH"

echo "::group::Rebuild & test HEAD co2-base (pass 2, HEAD carbon translating)"
regen_co2 "$REPO_ROOT/co2-base"
cd "$REPO_ROOT/co2-base"
# configure resolves $CARBON to an absolute path via AC_PATH_PROG at the time
# it runs and bakes that into the Makefile -- it does not re-resolve it from
# PATH on every `make`. Pass 1's configure ran before HEAD's own carbon was
# on PATH, so its Makefile still points at the bootstrap carbon; re-run
# configure now that PATH has switched, or pass 2 silently keeps using
# bootstrap carbon and reproduces pass 1's bugs instead of testing the fix.
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$HEAD_PREFIX/include/co2-1.0"
make
check_with_logs
make install
echo "::endgroup::"

echo "::group::Rebuild & test HEAD carbon (pass 2, self-consistent)"
regen_co2 "$REPO_ROOT/carbon"
cd "$REPO_ROOT/carbon"
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$HEAD_PREFIX/include/co2-1.0 -I$HEAD_PREFIX/include/co2-base-1.0"
make
check_with_logs
echo "::endgroup::"

echo "All builds and tests passed."
