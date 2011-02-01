#!/bin/bash -u
LD=gcc

BASEDIR=`pwd`
COMPILER=${BASEDIR}/../src/carbon

TESTDIR=${BASEDIR}
TARGET=${TESTDIR}/target

LOGFILE=${TARGET}/${!#}.log
if [[ -e ${LOGFILE} ]]
then
    rm -f ${LOGFILE}
fi

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
    echo "Command: ${COMPILER} ${TEST} ${TARGETNAME}.c" >> ${LOGFILE}
    ${COMPILER} ${TEST} ${TARGETNAME}.c >> ${LOGFILE} 2>&1
    if [[ "$?" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: Compiler error" >> ${LOGFILE}
	return 1
    fi
    # Compile the generated code with ${CC}
    pushd `dirname ${TARGETNAME}.bin` >> ${LOGFILE} 2>&1
    echo "Command: ${CC} -g3 -c ${TARGETNAME}.c -o `basename ${TARGETNAME}.o` ${CFLAGS}" >> ${LOGFILE}
    ${CC} -g3 -c ${TARGETNAME}.c -o `basename ${TARGETNAME}.o` ${CFLAGS} >> ${LOGFILE} 2>&1
    local CC_STATUS=$?
    popd >> ${LOGFILE} 2>&1
    if [[ "${CC_STATUS}" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: CC error" >> ${LOGFILE}
	return 1
    else
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
    # Compile the generated code with ${CC}
    pushd `dirname ${TARGETNAME}.bin` >> ${LOGFILE} 2>&1
    echo "Command: ${LD} ${OBJECTS} -o `basename ${TARGETNAME}.bin` ${LDFLAGS} -lc -lm" >> ${LOGFILE}
    ${LD} ${OBJECTS} -o `basename ${TARGETNAME}.bin` ${LDFLAGS} -lc -lm >> ${LOGFILE} 2>&1
    local LD_STATUS=$?
    popd >> ${LOGFILE} 2>&1
    if [[ "${LD_STATUS}" != "0" ]]
    then
	echo "ERROR: ${TEST} failed: LD error" >> ${LOGFILE}
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
	echo "ERROR: ${TEST} failed: output error" >> ${LOGFILE}
	return 1
    fi
    return 0
}

MAIN=""
while [[ $# > 0 ]];
do
    compile_library_test $1
    if [[ $# == 1 ]]
    then
	MAIN=$1
    fi
    shift 1
done
run_succes_test ${MAIN}

