#!/bin/bash -u
BASEDIR=`pwd`
COMPILER=${BASEDIR}/Compiler.bin

TESTDIR=${BASEDIR}/test
TARGET=${TESTDIR}/target
# Remove previous output
rm -rf ${TARGET}

# Create output folder
mkdir -p ${TARGET}/success > /dev/null
mkdir -p ${TARGET}/fail > /dev/null

ERRORS=0
# Testcases that should work
TESTS=`find ${TESTDIR}/success -name "*.test"`
for TEST in ${TESTS}
do
    # Compile the testcase
    BASENAME=`basename ${TEST} .test`
    TARGETNAME=${TARGET}/success/${BASENAME}
    ${COMPILER} ${TEST} ${TARGETNAME}.c > ${TARGETNAME}.err 2>&1
    if [[ "$?" != "0" ]]
    then
	ERRORS=$(( ERRORS + 1 ))
	cat ${TARGETNAME}.err
	echo "ERROR: ${TEST} failed: Compiler error"
    else
	# Compile the generated code with gcc
	pushd `dirname ${TARGETNAME}.bin` > /dev/null 2>&1
	gcc ${TARGETNAME}.c -o `basename ${TARGETNAME}.bin`
	GCC_STATUS=$?
	popd > /dev/null 2>&1
	if [[ "${GCC_STATUS}" != "0" ]]
	then
	    ERRORS=$(( ERRORS + 1 ))
	    echo "ERROR: ${TEST} failed: GCC error"
	else
	    # When no input and output exists, create empty in/output.
	    TESTINPUT=${TESTDIR}/success/${BASENAME}.in
	    TESTOUTPUT=${TESTDIR}/success/${BASENAME}.out
	    if [[ ! -f ${TESTINPUT} ]]; then touch ${TESTINPUT}; fi
	    if [[ ! -f ${TESTOUTPUT} ]]; then touch ${TESTOUTPUT}; fi
	    # Run program with input, and compare the output with the expected output.
	    cat  ${TESTINPUT} | ${TARGETNAME}.bin > ${TARGETNAME}.out
	    diff ${TESTOUTPUT} ${TARGETNAME}.out
	    if [[ "$?" != "0" ]]
	    then
		ERRORS=$(( ERRORS + 1 ))
		echo "ERROR: ${TEST} failed: output error"
	    else
		rm ${TARGETNAME}.err
	    fi
	fi
    fi
done

# Testcases that should not work
TESTS=`find ${TESTDIR}/fail -name "*.test"`
for TEST in ${TESTS}
do
    BASENAME=`basename ${TEST} .test`
    TARGETNAME=${TARGET}/fail/${BASENAME}
    ${COMPILER} ${TEST} ${TARGETNAME}.c > ${TARGETNAME}.err 2>&1
    if [[ "$?" == "0" ]]
    then
	ERRORS=$(( ERRORS + 1 ))
	cat ${TARGETNAME}.err
	echo "ERROR: ${TEST} failed."
    else
	rm -f ${TARGETNAME}.err
    fi
done

# TODO add commandline flag to prevent deleting test results
if [[ "${ERRORS}" == "0" ]]
then
    rm -rf ${TARGET}/success
    rm -rf ${TARGET}/fail
    touch ${TARGET}/OK
    echo "All tests passed."
fi

exit ${ERRORS}