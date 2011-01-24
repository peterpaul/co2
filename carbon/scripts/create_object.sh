#!/bin/bash -u

TEMPLATE=`dirname $PWD/$0`/Template

CLASS=$1
SUPER=$2

cat ${TEMPLATE}.h | sed "s/\$1/$CLASS/g" | sed "s/\$2/$SUPER/g" > $CLASS.h
cat ${TEMPLATE}.c | sed "s/\$1/$CLASS/g" | sed "s/\$2/$SUPER/g" > $CLASS.c
