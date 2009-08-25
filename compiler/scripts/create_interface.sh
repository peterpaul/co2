#!/bin/bash

TEMPLATE=`dirname $PWD/$0`/TemplateInterface

INTERFACE=$1
SUPERCLASS=$2

cat ${TEMPLATE}.h | sed "s/\$1/$INTERFACE/g" | sed "s/\$2/$SUPERCLASS/g" > $INTERFACE.h
cat ${TEMPLATE}.c | sed "s/\$1/$INTERFACE/g" | sed "s/\$2/$SUPERCLASS/g" > $INTERFACE.c
