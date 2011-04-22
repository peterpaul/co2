#!/bin/bash -u
# Copyright (C) 2011 Peterpaul Taekele Klein Haneveld

BASEDIR=$( pwd )
COMPILER=${BASEDIR}/../src/carbon

TARGET=${BASEDIR}/target

LOGFILE=${TARGET}/${!#}.log
if [[ -e ${LOGFILE} ]]
then
    rm -f ${LOGFILE}
fi

if [ "${SRCDIR-x}" == "x" ]; then
    echo "SRCDIR not defined"
    SRCDIR=.
fi

mkdir -p ${TARGET}/fail
find fail -name "*.h" -exec cp -t ${TARGET}/fail {} \;

function run_fail_test {
    local TEST=$1
    local BASENAME=$( basename ${TEST} .test )
    local DIRNAME=$( dirname ${TEST} )
    local CO2=$( echo ${BASENAME} | grep ".co2" | wc -l )
    if [[ $CO2 == 1 ]]
    then
	local BASENAME=$( basename ${TEST} .co2 )
    fi
    local TARGETNAME=${TARGET}/${DIRNAME}/${BASENAME}
    mkdir -p ${TARGET}/${DIRNAME}
    # Compile the testcase
    echo "Command: ${COMPILER} ${SRCDIR}/${TEST} ${TARGETNAME}.c" >> ${LOGFILE}
    ${COMPILER} ${SRCDIR}/${TEST} ${TARGETNAME}.c >> ${LOGFILE} 2>&1
    if [[ "$?" == "0" ]]
    then
	return 1
    fi
    return 0
}

run_fail_test ${!#}
