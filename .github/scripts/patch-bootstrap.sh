#!/usr/bin/env bash
# Patches the old bootstrap release tarballs (libco2-0.3.0, libco2-base-0.3.0,
# carbon-0.3.1 -- see TODO.md item #1) so they build on a modern toolchain.
#
# These tarballs predate every fix that's already applied at HEAD in this
# repo. Once this bootstrap carbon is built and installed, building HEAD's
# own co2/co2-base/carbon needs none of this -- that's the whole point.
#
# All of these patches are semantically neutral (renames, extern additions,
# an angle-bracket include, a Makefile.am dependency listing) and are applied
# unconditionally on every OS: most of them exist because of case-insensitive
# filesystem collisions (macOS/Windows), but applying the same rename/fix on
# a case-sensitive filesystem (Linux) is harmless -- there's no reason to
# special-case by OS here.
#
# Usage: patch-bootstrap.sh <libco2-dir> <libco2-base-dir> <carbon-dir>
set -euo pipefail

LIBCO2_DIR="$1"
LIBCO2_BASE_DIR="$2"
CARBON_DIR="$3"

sedi() {
    # Portable in-place sed: works with both BSD sed (macOS) and GNU sed
    # (Linux/MSYS2), unlike `sed -i ''` (BSD-only) or bare `sed -i` (GNU-only
    # without a following argument).
    local pattern=$1
    shift
    for f in "$@"; do
        sed -i.bak -e "$pattern" "$f"
        rm -f "$f.bak"
    done
}

echo "=== Patching libco2 (bootstrap) ==="
cd "$LIBCO2_DIR"
# Case-insensitive collision: libco2's own co2/exception.h shadows co2-base's
# co2/Exception.h once both are on the include path. Rename to a name that
# can't collide either way.
mv src/co2/exception.c src/co2/co2_exception.c
mv src/co2/exception.h src/co2/co2_exception.h
sedi 's|co2/exception\.h|co2/co2_exception.h|' src/co2/Object.h src/co2/co2_exception.c
sedi 's|co2/exception\.c|co2/co2_exception.c|' src/Makefile.am

# MinGW's headers don't provide BSD/glibc's __STRING(x) macro (used to
# stringify O_CALL's method-name argument), so it's left unexpanded and the
# compiler tries to call a nonexistent function named __STRING with a bare
# identifier like `ctor`/`dtor` as its argument -- which fails to compile
# since those are only ever valid as struct member names, never standalone
# variables. Same fix as this repo's own HEAD utils.h.
sed -i.bak '/#define __ASSERT_VOID_CAST (void)/a\
#endif\
\
#ifndef __STRING\
#define __STRING(x) #x' src/co2/utils.h
rm -f src/co2/utils.h.bak

# ASSERT_FAIL's non-glibc branch calls Darwin/BSD's __assert_rtn, which
# doesn't exist on MinGW either -- message() has already printed the failure
# text by the time ASSERT_FAIL runs, so all it needs to do elsewhere is
# actually terminate the program. Same fix as this repo's own HEAD utils.h.
perl -0777 -pi -e 's/#else\n#define ASSERT_FAIL\(p,file,line,function\)     \\\n  __assert_rtn\(function,file,line,p\)\n#endif/#elif defined(__APPLE__)\n#define ASSERT_FAIL(p,file,line,function)     \\\n  __assert_rtn(function,file,line,p)\n#else\n#define ASSERT_FAIL(p,file,line,function)     \\\n  abort()\n#endif/' src/co2/utils.h
sedi 's|#include <assert.h>|#include <assert.h>\n#include <stdlib.h>|' src/co2/utils.h

autoreconf -fi

echo "=== Patching libco2-base (bootstrap) ==="
cd "$LIBCO2_BASE_DIR"
# Case-insensitive collision: BaseObject.h/.c quote-include "string.h" (the
# system header), which resolves to co2-base's own generated co2/String.h
# instead once "-I ." is in effect. Angle brackets skip the local directory.
sedi 's|#include "string.h"|#include <string.h>|' src/co2/BaseObject.h src/co2/BaseObject.c
# Match libco2's exception.h -> co2_exception.h rename above.
sedi 's|co2/exception\.h|co2/co2_exception.h|' src/co2/BaseObject.h src/co2/BaseObject.c

# random() is a POSIX extension that MinGW's C library doesn't provide --
# it's declared (implicitly, no prototype) and used in a few places but
# never actually defined, so linking fails with "undefined reference to
# `random'" the moment a test binary that calls it gets built. No carbon
# binary exists yet at this bootstrap stage to retranslate the .co2 sources
# (that's what pass 1/2 in build-and-test.sh are for), so patch the
# tarball's bundled pre-generated .c files directly. rand() is standard C89,
# available everywhere, and plenty adequate for what these tests need
# (alternating/pseudo-random test data).
sedi 's|random ()|rand ()|' test/co2/TestRefObject.c test/co2/TestMap.c

# localtime_r is a POSIX reentrant variant MinGW's C library doesn't provide
# either (only plain, non-reentrant localtime()); same undefined-reference-
# at-link-time failure as random() above. logging one record at a time,
# non-reentrant is an acceptable trade for portability here.
sedi 's|localtime_r (\& record->time.tv_sec, \& tm)|localtime (\& record->time.tv_sec)|' src/co2/SimpleFormatter.c

echo "=== Patching carbon (bootstrap) ==="
cd "$CARBON_DIR/src"
# This dist tarball bundles stale, wrongly-located pre-generated grammar.c/
# grammar.h/lex.c at top level -- leftover from before grammar.y/lex.l moved
# into co2/. Makefile.am's own source list (co2/grammar.y, co2/lex.l) and
# clean-local rule (which rm's top-level grammar.c/grammar.h/lex.c) both
# confirm this; the stale duplicates cause a "duplicate symbol '_yyerror'"
# link error once the real co2/grammar.c gets built alongside them.
rm -f grammar.c grammar.h lex.c

# ArgumentDeclaration.c uses _VA_ARG (one of the token constants pulled in by
# co2/Grammar.h) but is missing the include that every other user of these
# constants already has -- a pre-existing gap in this tarball's pre-generated
# sources, unrelated to the rename below. Add it before the batch rename so
# the sed pass a few lines down picks up this occurrence too.
sed -i.bak '1a\
#include "co2/Grammar.h"
' co2/ArgumentDeclaration.c
rm -f co2/ArgumentDeclaration.c.bak

# Case-insensitive collision: co2/Grammar.co2 (a small wrapper that pulls in
# bison's token constants) generates co2/Grammar.h, which collides with
# bison's own co2/../grammar.h -- and separately, co2/Grammar.c collides with
# the real bison-generated co2/grammar.c. Rename the carbon-side wrapper
# instead of bison's output.
mv co2/Grammar.co2 co2/GrammarTokens.co2
mv co2/Grammar.h co2/GrammarTokens.h
sedi 's|Grammar_H|GrammarTokens_H|g' co2/GrammarTokens.h
rm -f co2/Grammar.c
# No carbon binary exists yet at this bootstrap stage to retranslate
# GrammarTokens.co2 -> GrammarTokens.c, so recreate the (trivial) generated
# output by hand -- it only ever wrapped the two includes below.
cat > co2/GrammarTokens.c <<'EOF'
#include "GrammarTokens.h"
#include "grammar.h"
EOF
grep -rl 'include "co2/Grammar.h"' co2/*.c co2/*.h 2>/dev/null | while read -r f; do
    sedi 's|include "co2/Grammar.h"|include "co2/GrammarTokens.h"|' "$f"
done
sedi 's|co2/Grammar\.co2|co2/GrammarTokens.co2|' Makefile.am

# grammar.h (bison's -d output, generated at top level despite co2/grammar.y
# living in a subdirectory -- ylwrap's own naming convention) isn't listed as
# a BUILT_SOURCES dependency, so files needing it can be compiled before it
# exists.
sedi 's|^BUILT_SOURCES=\\|BUILT_SOURCES=\\\ngrammar.h\\|' Makefile.am

# macOS's ld64 doesn't merge tentative definitions across translation units
# the way traditional linkers do; lex.l's own un-`extern`'d `path` collides
# with io.c's real definition. `extern` here is correct and harmless on every
# platform regardless of linker behavior.
sedi 's|^struct ArrayList \* path;|extern struct ArrayList * path;|' co2/lex.l

# realpath() is POSIX; MinGW's C library doesn't provide it. _fullpath's
# NULL-buffer convention matches realpath's -- it malloc's the result, which
# the caller already frees either way. Same fix as this repo's own HEAD
# lex.l.
sed -i.bak 's|\tchar \* _real_path = realpath (_absolute_path->data, NULL);|#ifdef __MINGW32__\n\tchar * _real_path = _fullpath (NULL, _absolute_path->data, 0);\n#else\n\tchar * _real_path = realpath (_absolute_path->data, NULL);\n#endif|' co2/lex.l
rm -f co2/lex.l.bak

cd "$CARBON_DIR"
autoreconf -fi

echo "=== Bootstrap patches applied ==="
