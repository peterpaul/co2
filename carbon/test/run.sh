#!/bin/bash -u
# Copyright (C) 2011 Peterpaul Taekele Klein Haneveld

export CC="gcc"
export CFLAGS="-g3 -O0 -I/home/peterpaul/local/include/co2-1.0"
export LDFLAGS="-L/home/peterpaul/local/lib -lco2-1.0"
export LD="gcc"
export SRCDIR="."
$@
