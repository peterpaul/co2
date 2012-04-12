#!/bin/bash -u
# Copyright (C) 2011 Peterpaul Taekele Klein Haneveld

export CC="gcc"
export CFLAGS="-g3 -O0 -I${HOME}/local/include/co2-1.0"
export LDFLAGS="-L${HOME}/local/lib -lco2-1.0"
export LD="gcc"
export SRCDIR=$(dirname $(readlink -f $(which $0)))
$@
