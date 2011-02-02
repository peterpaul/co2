#!/bin/bash -u

BASEDIR=`pwd`
COMPILER=${BASEDIR}/../src/carbon

TESTDIR=${BASEDIR}
TARGET=${TESTDIR}/target

LOGFILE=${TARGET}/${!#}.log
if [[ -e ${LOGFILE} ]]
then
    rm -f ${LOGFILE}
fi

mkdir -p ${TARGET}/fail
find fail -name "*.h" -exec cp -t ${TARGET}/fail {} \;

function run_fail_test {
    echo "--- ${LOGFILE} ---"
    local TEST=$1
    local BASENAME=`basename ${TEST} .test`
    local TARGETNAME=${TARGET}/fail/${BASENAME}
    echo "Command: ${COMPILER} ${TEST} ${TARGETNAME}.c" >> ${LOGFILE}
    ${COMPILER} ${TEST} ${TARGETNAME}.c >> ${LOGFILE} 2>&1
    if [[ "$?" == "0" ]]
    then
	return 1
    fi
    return 0
}

run_fail_test ${!#}
