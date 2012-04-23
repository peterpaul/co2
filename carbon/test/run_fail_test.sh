#!/bin/bash -u
# Copyright (C) 2011 Peterpaul Taekele Klein Haneveld

LD=gcc

BASEDIR=$(dirname $(readlink -f $(which $0)))
COMPILER=${BASEDIR}/../carbon/carbon

TARGET=${BASEDIR}/target

LOGFILE=${TARGET}/${!#}.log
if [[ -e ${LOGFILE} ]]
then
    rm -f ${LOGFILE}
fi

if [[ "${SRCDIR-x}" == "x" ]]
 then
    echo "SRCDIR not defined"
    SRCDIR=.
fi

mkdir -p ${TARGET}/fail
find ${SRCDIR}/fail -name "*.h" -exec cp -u -t ${TARGET}/fail {} \;

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
    echo "Command: ${COMPILER} -I ${SRCDIR}/fail ${SRCDIR}/${TEST} ${TARGETNAME}.c" >> ${LOGFILE}
    ${COMPILER} -I ${SRCDIR}/fail ${SRCDIR}/${TEST} ${TARGETNAME}.c >> ${LOGFILE} 2>&1
    if [[ "$?" == "0" ]]
    then
	return 1
    fi
    return 0
}

run_fail_test ${!#}
exit_code=$?
if [[ "${exit_code}" == "0" ]]
then
    echo "passed" >> ${LOGFILE}
else
    echo "failed" >> ${LOGFILE}
fi
exit $exit_code
