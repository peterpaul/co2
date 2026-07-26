#!/usr/bin/env bash
# Full CI build: bootstrap an old carbon release, then use it to build and
# test HEAD's own co2/co2-base/carbon. See TODO.md item #1 for the plan this
# implements, and .github/scripts/patch-bootstrap.sh for why the bootstrap
# tarballs need patching first.
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
cd "$REPO_ROOT/co2"
./autogen.sh
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON"
make
check_with_logs
make install
echo "::endgroup::"

echo "::group::Build & test HEAD libco2-base"
cd "$REPO_ROOT/co2-base"
export PKG_CONFIG_PATH="$HEAD_PREFIX/lib/pkgconfig"
./autogen.sh
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$HEAD_PREFIX/include/co2-1.0"
make
check_with_logs
make install
echo "::endgroup::"

echo "::group::Build & test HEAD carbon"
cd "$REPO_ROOT/carbon"
./autogen.sh
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$HEAD_PREFIX/include/co2-1.0 -I$HEAD_PREFIX/include/co2-base-1.0"
make
check_with_logs
echo "::endgroup::"

echo "All builds and tests passed."
