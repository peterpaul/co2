#!/bin/bash
set -x

if [[ ($# > 0) && ("$1" == "clean") ]]
then
    make clean
    rm co2/*.[ch]
else
    make -k
    make -k
fi
