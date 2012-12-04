#!/bin/bash

function absolute_path {
    local file=$1
    if [[ "${file:0:1}" == "." ]]
    then
	echo ${PWD}/${file}
    else
	readlink -e ${file}
    fi
}

if [[ "${SRCDIR-x}" == "x" ]]
then
    echo "SRCDIR not defined"
    SRCDIR=.
else
    SRCDIR=$(absolute_path ${SRCDIR})
fi

if [[ "${TOP_SRCDIR-x}" == "x" ]]
then
    echo "TOP_SRCDIR not defined"
    TOP_SRCDIR=..
else
    TOP_SRCDIR=$(absolute_path ${TOP_SRCDIR})
fi

if [[ "${BUILDDIR-x}" == "x" ]]
then
    echo "BUILDDIR not defined"
    BUILDDIR=.
else
    BUILDDIR=$(absolute_path ${BUILDDIR})
fi

if [[ "${TOP_BUILDDIR-x}" == "x" ]]
then
    echo "TOP_BUILDDIR not defined"
    TOP_BUILDDIR=..
else
    TOP_BUILDDIR=$(absolute_path ${TOP_BUILDDIR})
fi

COMPILER=${TOP_BUILDDIR}/src/carbon

TARGET=${BUILDDIR}/target

LOGFILE=$(readlink -m ${TARGET}/${!#}.log)
if [[ -e ${LOGFILE} ]]
then
    rm -f ${LOGFILE}
fi
