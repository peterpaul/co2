#!/usr/bin/env bash
# Stage 2 of the CI pipeline ("build-and-test"): consume Stage 1's pre-
# translated dist tarballs (libco2-base-*.tar.gz, carbon-*.tar.gz -- see
# .github/scripts/translate.sh) and build + test everything with this
# platform's own native C toolchain. No carbon binary is needed anywhere in
# this script -- translation already happened once, centrally, on Linux --
# so adding a new platform/architecture to the CI matrix means adding a new
# `os:` entry that runs this same script, nothing more. See TODO.md item #1
# for the full two-stage rationale.
#
# Required environment variables (set by the workflow per-OS):
#   CC       - C compiler to use throughout (a real GCC; see TODO.md item #3
#              for why Clang/MSVC can't build this -- GNU nested functions)
#   WORKDIR  - scratch directory for extracting/building/installing into
#   DISTDIR  - directory containing the two dist tarballs downloaded from
#              Stage 1 (libco2-base-*.tar.gz, carbon-*.tar.gz)
#
# Must be run from the repo root.
set -euo pipefail

: "${CC:?CC must be set}"
: "${WORKDIR:?WORKDIR must be set}"
: "${DISTDIR:?DISTDIR must be set}"

REPO_ROOT="$(pwd)"
HEAD_PREFIX="$WORKDIR/head-prefix"
CFLAGS_COMMON="-std=gnu89 -g -O2"

mkdir -p "$WORKDIR" "$HEAD_PREFIX"

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
export LD_LIBRARY_PATH="$HEAD_PREFIX/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

echo "::group::Build & test co2 (libco2, from git checkout -- pure C, no translation needed)"
cd "$REPO_ROOT/co2"
./autogen.sh
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON"
make
check_with_logs
make install
echo "::endgroup::"

echo "::group::Build & test co2-base (from Stage 1's pre-translated dist tarball -- no carbon needed)"
tar xzf "$DISTDIR"/libco2-base-*.tar.gz -C "$WORKDIR"
CO2BASE_DIR="$(find "$WORKDIR" -maxdepth 1 -type d -name 'libco2-base-*')"
cd "$CO2BASE_DIR"
export PKG_CONFIG_PATH="$HEAD_PREFIX/lib/pkgconfig"
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$HEAD_PREFIX/include/co2-1.0"
make
check_with_logs
make install
echo "::endgroup::"

echo "::group::Build & test carbon (from Stage 1's pre-translated dist tarball -- no carbon needed)"
tar xzf "$DISTDIR"/carbon-*.tar.gz -C "$WORKDIR"
CARBON_DIR="$(find "$WORKDIR" -maxdepth 1 -type d -name 'carbon-*')"
cd "$CARBON_DIR"
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$HEAD_PREFIX/include/co2-1.0 -I$HEAD_PREFIX/include/co2-base-1.0"
make
check_with_logs
echo "::endgroup::"

echo "All builds and tests passed."
