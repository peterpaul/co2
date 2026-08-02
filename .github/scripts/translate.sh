#!/usr/bin/env bash
# Stage 1 of the CI pipeline ("translate"): bootstrap a carbon release, then
# use it to two-pass-translate HEAD's own co2-base and carbon .co2 sources
# into portable C, and package the results as dist tarballs. Stage 2
# (build-and-test.sh) downloads those tarballs and builds+tests co2-base and
# carbon on every platform in the matrix with a plain C toolchain -- no
# bootstrap-carbon dance repeated per platform, since translation only ever
# happens here, once, on Linux. See TODO.md item #1 for the full rationale.
#
# The bootstrap releases below (BOOTSTRAP_*_TAG) should always point at the
# *most recently cut* release of each project, not a historical pin -- as
# long as that's true, the bootstrap sources already reflect every fix in
# this repo's history (case-insensitive-filesystem renames, MinGW
# portability fixes, compiler-side codegen fixes, all of it), so no patching
# step is needed before building them, unlike when this bootstrapped from the
# 2015-2018 releases (`libco2-0.3.0`/`libco2-base-0.3.0`/`carbon-0.3.1`) --
# see `git log -- .github/scripts/patch-bootstrap.sh` if that patching logic
# is ever needed again (e.g. bootstrapping a long-stale fork). Bump these tags
# whenever a new release is cut, to keep the bootstrap chain (and CI runtime)
# as fast and simple as possible: libco2-base and carbon's release tarballs
# are already `make dist` output (pre-generated configure/Makefile.in/.c/.h,
# no autoreconf needed); libco2 has no equivalent dist tarball asset yet, so
# it uses GitHub's automatic per-tag source archive instead (a plain git
# checkout at that tag, needing ./autogen.sh, but with every rename/fix from
# this repo's history already applied in the checked-in source itself).
BOOTSTRAP_LIBCO2_TAG=libco2-0.3.2
BOOTSTRAP_LIBCO2_BASE_TAG=libco2-base-0.3.2
BOOTSTRAP_CARBON_TAG=carbon-0.3.4
#
# Two-pass rebuild of co2-base and carbon (see "PASS 2" below) is still
# required: the bootstrap carbon predates *this* commit's own .co2 source
# changes, if any. Anything the *bootstrap* carbon translates only reflects
# compiler-side fixes as of the bootstrap release, not necessarily anything
# newer at HEAD; only after HEAD's own carbon has been built once (pass 1)
# and installed does translating everything *again* (pass 2) actually
# reflect HEAD's current state -- and pass 2's output is what gets packaged.
#
# Unlike the old single-stage build-and-test.sh, this script never runs
# `make check` -- testing is entirely Stage 2's job (including for Linux
# itself, as one of Stage 2's own matrix legs), so this script stays focused
# on producing correct, portable dist tarballs as fast as possible.
#
# Required environment variables (set by the workflow):
#   CC       - C compiler to use throughout (a real GCC; see TODO.md item #3
#              for why Clang/MSVC can't build this -- GNU nested functions)
#   WORKDIR  - scratch directory for downloading/extracting/building the
#              bootstrap tarballs and installing everything into
#   OUTDIR   - directory to copy the two dist tarballs into, for the
#              workflow to upload as artifacts
#
# Must be run from the repo root.
set -euo pipefail

: "${CC:?CC must be set}"
: "${WORKDIR:?WORKDIR must be set}"
: "${OUTDIR:?OUTDIR must be set}"

REPO_ROOT="$(pwd)"
BOOTSTRAP_PREFIX="$WORKDIR/bootstrap-prefix"
HEAD_PREFIX="$WORKDIR/head-prefix"
CFLAGS_COMMON="-std=gnu89 -g -O2"

# TODO.md item #6: this repo has hit three separate case-insensitive-
# filesystem naming collisions (macOS/Windows) across its history --
# exception.h/Exception.h, Grammar.h/grammar.h, IncludeStack.co2's own
# File.h reference. Cheap, fast, fails before any of the expensive
# bootstrap/build work below: every .co2-consuming project's own src/co2/
# directory is where all three actually happened, so check each one for two
# tracked filenames that only differ by case. Not a perfect check (a
# collision could in principle span two *different* directories that both
# end up on the same -I search path), but matches every incident so far
# and catches new ones within a single project before they cause a
# hard-to-debug platform-specific build failure.
for dir in co2 co2-base carbon; do
    dupes=$(git -C "$REPO_ROOT" ls-files "$dir/src/co2/*.co2" "$dir/src/co2/*.h" \
        | xargs -n1 basename | awk '{print tolower($0)}' | sort | uniq -d)
    if [ -n "$dupes" ]; then
        echo "::error::Case-insensitive filename collision(s) in $dir/src/co2/ (breaks macOS/Windows builds): $dupes" >&2
        exit 1
    fi
done

mkdir -p "$WORKDIR" "$BOOTSTRAP_PREFIX" "$HEAD_PREFIX" "$OUTDIR"
cd "$WORKDIR"

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

echo "::group::Download bootstrap releases"
# libco2 has no `make dist`-style release asset yet (see the header comment)
# -- GitHub's automatic per-tag source archive gives the same source at a
# small extra cost (needs ./autogen.sh below, since it's a raw checkout
# without pre-generated configure/Makefile.in).
curl -sL -o libco2-src.tar.gz "https://github.com/peterpaul/co2/archive/refs/tags/${BOOTSTRAP_LIBCO2_TAG}.tar.gz"
curl -sL -o libco2-base.tar.gz "https://github.com/peterpaul/co2/releases/download/${BOOTSTRAP_LIBCO2_BASE_TAG}/${BOOTSTRAP_LIBCO2_BASE_TAG}.tar.gz"
curl -sL -o carbon.tar.gz "https://github.com/peterpaul/co2/releases/download/${BOOTSTRAP_CARBON_TAG}/${BOOTSTRAP_CARBON_TAG}.tar.gz"
tar xzf libco2-src.tar.gz
tar xzf libco2-base.tar.gz
tar xzf carbon.tar.gz
# GitHub's archive names its top-level directory <repo>-<tag>, not <tag> --
# capture it rather than assume, since it doesn't match this repo's own
# release-asset naming convention.
LIBCO2_DIR="$(find "$WORKDIR" -maxdepth 1 -type d -name "co2-${BOOTSTRAP_LIBCO2_TAG}")"
echo "::endgroup::"

echo "::group::Build bootstrap libco2"
cd "$LIBCO2_DIR/co2"
./autogen.sh
./configure --prefix="$BOOTSTRAP_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON"
make
make install
echo "::endgroup::"

echo "::group::Build bootstrap libco2-base"
cd "$WORKDIR/$BOOTSTRAP_LIBCO2_BASE_TAG"
export PKG_CONFIG_PATH="$BOOTSTRAP_PREFIX/lib/pkgconfig"
./configure --prefix="$BOOTSTRAP_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$BOOTSTRAP_PREFIX/include/co2-1.0"
make
make install
echo "::endgroup::"

echo "::group::Build bootstrap carbon"
cd "$WORKDIR/$BOOTSTRAP_CARBON_TAG"
./configure --prefix="$BOOTSTRAP_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON"
make
make install
"$BOOTSTRAP_PREFIX/bin/carbon" --help > /dev/null
echo "::endgroup::"

# From here on, the bootstrap carbon (now on PATH) translates HEAD's own
# .co2 sources.
export PATH="$BOOTSTRAP_PREFIX/bin:$PATH"
unset PKG_CONFIG_PATH

# co2-base and carbon's own `configure` steps below need libco2-1.0 findable
# via pkg-config -- libco2 itself needs no .co2 translation (pure C), it just
# needs to exist in HEAD_PREFIX so those configure checks pass. Stage 2 still
# builds it fresh from the checkout rather than this dist tarball (no
# translation dependency to short-circuit there), but packaging it here
# closes TODO.md item #1's noted asymmetry: a future libco2 release now gets
# a real `make dist` tarball as a release asset like the other two projects,
# instead of only GitHub's raw per-tag source archive -- release.yml's
# artifact upload/publish steps already glob `*.tar.gz` generically, so
# nothing else needs to change to pick this up.
echo "::group::Build & install HEAD libco2 (no translation, no test -- just a build dependency for co2-base/carbon below -- but packaged via make dist)"
cd "$REPO_ROOT/co2"
./autogen.sh
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON"
make
make install
make dist
cp *.tar.gz "$OUTDIR/"
echo "::endgroup::"

echo "::group::Build HEAD co2-base (pass 1, bootstrap carbon -- not tested yet)"
regen_co2 "$REPO_ROOT/co2-base"
cd "$REPO_ROOT/co2-base"
export PKG_CONFIG_PATH="$HEAD_PREFIX/lib/pkgconfig"
./autogen.sh
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$HEAD_PREFIX/include/co2-1.0"
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

echo "::group::Rebuild & package HEAD co2-base (pass 2, HEAD carbon translating -- self-consistent, ready to package)"
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
# Stage 2's carbon (pass 2, below) links against co2-base via pkg-config, so
# it needs to be installed here even though nothing in *this* script runs
# co2-base's tests.
make install
make dist
cp *.tar.gz "$OUTDIR/"
echo "::endgroup::"

echo "::group::Rebuild & package HEAD carbon (pass 2, self-consistent)"
regen_co2 "$REPO_ROOT/carbon"
cd "$REPO_ROOT/carbon"
./configure --prefix="$HEAD_PREFIX" CC="$CC" CFLAGS="$CFLAGS_COMMON -I$HEAD_PREFIX/include/co2-1.0 -I$HEAD_PREFIX/include/co2-base-1.0"
make
make dist
cp *.tar.gz "$OUTDIR/"
echo "::endgroup::"

echo "Translation complete. Dist tarballs ready for Stage 2:"
ls -la "$OUTDIR"
