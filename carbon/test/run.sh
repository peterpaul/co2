#!/bin/bash -u
# Copyright (C) 2011 Peterpaul Taekele Klein Haneveld

export CC="gcc"
export CFLAGS="-g -O2 -I/home/peterpaul/local/co2-0.0.1/include/co2-1.0"
export LDFLAGS="-L/home/peterpaul/local/co2-0.0.1/lib -lco2-1.0"
export LD="gcc"

$@
