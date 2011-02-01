#!/bin/bash -u

BASEDIR=`pwd`
COMPILER=${BASEDIR}/../src/carbon

TESTDIR=${BASEDIR}
TARGET=${TESTDIR}/target

mkdir -p ${TARGET}/success
find success -name "*.h" -exec cp -t ${TARGET}/success {} \;

OBJECTS=

function compile_library_test {
    local TEST=$1

    local BASENAME=`basename ${TEST} .test`
    local CO2=$( echo ${BASENAME} | grep ".co2" | wc -l )
    if [[ $CO2 == 1 ]]
    then
	local BASENAME=`basename ${TEST} .co2`
    fi
    local TARGETNAME=${TARGET}/success/${BASENAME}
    # Compile the testcase
    ${COMPILER} ${TEST} ${TARGETNAME}.c > ${TARGETNAME}.err 2>&1
    if [[ "$?" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: Compiler error" >> ${TARGETNAME}.err
	echo "Command: ${COMPILER} ${TEST} ${TARGETNAME}.c" >> ${TARGETNAME}.err
	return 1
    fi
    # Compile the generated code with gcc
    pushd `dirname ${TARGETNAME}.bin` > /dev/null 2>&1
    gcc -g3 -c ${TARGETNAME}.c -o `basename ${TARGETNAME}.o` ${CFLAGS} > ${TARGETNAME}.err 2>&1
    local GCC_STATUS=$?
    popd > /dev/null 2>&1
    if [[ "${GCC_STATUS}" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: GCC error" >> ${TARGETNAME}.err
	echo "Command: gcc -g3 -c ${TARGETNAME}.c -o `basename ${TARGETNAME}.o` ${CFLAGS}" >> ${TARGETNAME}.err
	return 1
    else
	rm ${TARGETNAME}.err
	OBJECTS="${OBJECTS} $( basename ${TARGETNAME}.o )"
    fi
    return 0
}

function run_succes_test {
    local TEST=$1

    local BASENAME=`basename ${TEST} .test`
    local CO2=$( echo ${BASENAME} | grep ".co2" | wc -l )
    if [[ $CO2 == 1 ]]
    then
	local BASENAME=`basename ${TEST} .co2`
    fi
    local TARGETNAME=${TARGET}/success/${BASENAME}
    # Compile the testcase
    ${COMPILER} ${TEST} ${TARGETNAME}.c > ${TARGETNAME}.err 2>&1
    if [[ "$?" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: Compiler error" >> ${TARGETNAME}.err
	echo "Command: ${COMPILER} ${TEST} ${TARGETNAME}.c" >> ${TARGETNAME}.err
	return 1
    fi
    # Compile the generated code with gcc
    pushd `dirname ${TARGETNAME}.bin` > /dev/null 2>&1
    gcc -g3 ${TARGETNAME}.c ${OBJECTS} -o `basename ${TARGETNAME}.bin` ${CFLAGS} ${LDFLAGS} -lm > ${TARGETNAME}.err 2>&1
    local GCC_STATUS=$?
    popd > /dev/null 2>&1
    if [[ "${GCC_STATUS}" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: GCC error" >> ${TARGETNAME}.err
	echo "Command: gcc -g3 ${TARGETNAME}.c -o `basename ${TARGETNAME}.bin` ${CFLAGS} ${LDFLAGS} -lm" >> ${TARGETNAME}.err
	return 1
    fi
    # When no input and output exists, create empty in/output.
    local TESTINPUT=${TESTDIR}/success/${BASENAME}.in
    local TESTOUTPUT=${TESTDIR}/success/${BASENAME}.out
    if [[ ! -f ${TESTINPUT} ]]; then touch ${TESTINPUT}; fi
    if [[ ! -f ${TESTOUTPUT} ]]; then touch ${TESTOUTPUT}; fi
    # Run program with input, and compare the output with the expected output.
    cat  ${TESTINPUT} | ${TARGETNAME}.bin > ${TARGETNAME}.out
    diff ${TESTOUTPUT} ${TARGETNAME}.out
    if [[ "$?" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: output error" >> ${TARGETNAME}.err
	return 1
    else
	rm ${TARGETNAME}.err
    fi
    return 0
}

if [[ $# > 1 ]]
then
    while [[ $# > 1 ]];
    do
	compile_library_test $1
	shift 1
    done
    run_succes_test $1
else
    run_succes_test $1
fi
