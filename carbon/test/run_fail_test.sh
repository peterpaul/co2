#!/bin/bash -u

BASEDIR=`pwd`
COMPILER=${BASEDIR}/../src/carbon

TESTDIR=${BASEDIR}
TARGET=${TESTDIR}/target

mkdir -p ${TARGET}/fail
find fail -name "*.h" -exec cp -t ${TARGET}/fail {} \;

function run_fail_test {
    local TEST=$1
    local BASENAME=`basename ${TEST} .test`
    local TARGETNAME=${TARGET}/fail/${BASENAME}
    ${COMPILER} ${TEST} ${TARGETNAME}.c > ${TARGETNAME}.err 2>&1
    if [[ "$?" == "0" ]]
    then
	echo "Command: ${COMPILER} ${TEST} ${TARGETNAME}.c" >> ${TARGETNAME}.err
	return 1
    fi
    return 0
}

run_fail_test $1
