#!/bin/bash -u
# Copyright (C) 2011 Peterpaul Taekele Klein Haneveld

LD=gcc

BASEDIR=$(dirname $(readlink -f $(which $0)))

. ${BASEDIR}/run_test_base.sh

mkdir -p ${TARGET}/pass
find ${SRCDIR}/pass -name "*.h" -exec cp -u -t ${TARGET}/pass {} \;

OBJECTS=

echo "LOGFILE=${LOGFILE}" >> ${LOGFILE}
echo "SRCDIR=${SRCDIR}" >> ${LOGFILE}
echo "TOP_SRCDIR=${TOP_SRCDIR}" >> ${LOGFILE}
echo "BUILDDIR=${BUILDDIR}" >> ${LOGFILE}
echo "TOP_BUILDDIR=${TOP_BUILDDIR}" >> ${LOGFILE}
echo "TARGET=${TARGET}" >> ${LOGFILE}
echo "CC=${CC}" >> ${LOGFILE}
echo "CFLAGS=${CFLAGS}" >> ${LOGFILE}
echo "LDFLAGS=${LDFLAGS}" >> ${LOGFILE}
echo "LD=${LD}" >> ${LOGFILE}

CFLAGS="${CFLAGS} -DO_DEBUG -O0 -g3 -Wall -I${TOP_SRCDIR}/pass -I${TARGET}/pass"

function compile_library_test {
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
    echo "Command: ${COMPILER} -I ${SRCDIR}/pass ${SRCDIR}/${TEST} ${TARGETNAME}.c" >> ${LOGFILE}
    ${COMPILER} -I ${SRCDIR}/pass ${SRCDIR}/${TEST} ${TARGETNAME}.c >> ${LOGFILE} 2>&1
    if [[ "$?" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: Compiler error" >> ${LOGFILE}
	return 1
    fi
    return 0
}

function compile_library_gcc {
    local TEST=$1

    local BASENAME=$( basename ${TEST} .test )
    local DIRNAME=$( dirname ${TEST} )
    local CO2=$( echo ${BASENAME} | grep ".co2" | wc -l )
    if [[ $CO2 == 1 ]]
    then
	local BASENAME=$( basename ${TEST} .co2 )
    fi
    local TARGETNAME=${TARGET}/${DIRNAME}/${BASENAME}
    # Compile the generated code with ${CC}
    echo "Command: ${CC} -g3 -c ${TARGETNAME}.c -o ${TARGETNAME}.o ${CFLAGS}" >> ${LOGFILE}
    ${CC} -g3 -c ${TARGETNAME}.c -o ${TARGETNAME}.o ${CFLAGS} >> ${LOGFILE} 2>&1
    local CC_STATUS=$?
    if [[ "${CC_STATUS}" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: CC error" >> ${LOGFILE}
	return 1
    else
	OBJECTS="${OBJECTS} ${TARGETNAME}.o"
    fi
    return 0
}

function run_pass_test {
    local TEST=$1

    local BASENAME=$( basename ${TEST} .test )
    local DIRNAME=$( dirname ${TEST} )
    local CO2=$( echo ${BASENAME} | grep ".co2" | wc -l )
    if [[ $CO2 == 1 ]]
    then
	local BASENAME=$( basename ${TEST} .co2 )
    fi
    local TARGETNAME=${TARGET}/${DIRNAME}/${BASENAME}
    # Compile the generated code with ${CC}
    echo "Command: ${LD} ${OBJECTS} -o ${TARGETNAME}.bin ${LDFLAGS} -lc -lm" >> ${LOGFILE}
    ${LD} ${OBJECTS} -o ${TARGETNAME}.bin ${LDFLAGS} -lc -lm >> ${LOGFILE} 2>&1
    local LD_STATUS=$?
    if [[ "${LD_STATUS}" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: LD error" >> ${LOGFILE}
	return 1
    fi
    # When no input and output exists, create empty in/output.
    local TESTINPUT=${SRCDIR}/pass/${BASENAME}.in
    local TESTOUTPUT=${SRCDIR}/pass/${BASENAME}.out
    if [[ ! -f ${TESTINPUT} ]]; then touch ${TESTINPUT}; fi
    if [[ ! -f ${TESTOUTPUT} ]]; then touch ${TESTOUTPUT}; fi
    # Run program with input, and compare the output with the expected output.
    cat  ${TESTINPUT} | ${TARGETNAME}.bin > ${TARGETNAME}.out
    # if [[ "$?" != "0" ]]
    # then
    # 	echo "ERROR: ${TEST} failed: execution error" >> ${LOGFILE}
    # 	return 1
    # fi
    diff ${TESTOUTPUT} ${TARGETNAME}.out
    if [[ "$?" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: output error" >> ${LOGFILE}
	return 1
    fi
    return 0
}

MAIN=${!#}
FILES=""
while [[ $# > 0 ]]
do
    FILES="${FILES} $1"
    shift 1
done

for FILE in ${FILES}
do
    compile_library_test ${FILE}
    if [[ $? != 0 ]]
    then
	exit 1
    fi
done

for FILE in ${FILES}
do
    compile_library_gcc ${FILE}
    if [[ $? != 0 ]]
    then
	exit 1
    fi
done

run_pass_test ${MAIN}
exit_code=$?
if [[ "${exit_code}" == "0" ]]
then
    echo "passed" >> ${LOGFILE}
else
    echo "failed" >> ${LOGFILE}
fi
exit $exit_code
