#!/bin/bash -u
BASEDIR=`pwd`
COMPILER=${BASEDIR}/Compiler.bin

TESTDIR=${BASEDIR}/test
TARGET=${TESTDIR}/target
# Remove previous output
rm -rf ${TARGET}

# Define helper functions.
function fail_test {
    ERRORS=$(( ERRORS + 1 ))
    echo $1
}

function test_separator {
    echo "---------------------------------------------------------------------------"
}

# Create output folder
mkdir -p ${TARGET}/success > /dev/null
mkdir -p ${TARGET}/fail > /dev/null

test_separator
echo "Success test-cases"
test_separator
ERRORS=0
# Testcases that should work
TESTS=`find ${TESTDIR}/success -name "*.test" | sort`
for TEST in ${TESTS}
do
    # Compile the testcase
    BASENAME=`basename ${TEST} .test`
    echo -n "${BASENAME}... "
    TARGETNAME=${TARGET}/success/${BASENAME}
    ${COMPILER} ${TEST} ${TARGETNAME}.c > ${TARGETNAME}.err 2>&1
    if [[ "$?" != "0" ]]
    then
	fail_test "ERROR: ${TEST} failed: Compiler error"
	cat ${TARGETNAME}.err
	test_separator
    else
	# Compile the generated code with gcc
	pushd `dirname ${TARGETNAME}.bin` > /dev/null 2>&1
	gcc -g3 -I${BASEDIR}/../ ${TARGETNAME}.c -o `basename ${TARGETNAME}.bin` -L${BASEDIR}/../ -lObject  > ${TARGETNAME}.err 2>&1
	GCC_STATUS=$?
	popd > /dev/null 2>&1
	if [[ "${GCC_STATUS}" != "0" ]]
	then
	    fail_test "ERROR: ${TEST} failed: GCC error"
	    cat ${TARGETNAME}.err
	    test_separator
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
		fail_test "ERROR: ${TEST} failed: output error"
	    else
		echo "OK"
		rm ${TARGETNAME}.err
	    fi
	fi
    fi
done

test_separator
echo "Fail test-cases"
test_separator
# Testcases that should not work
TESTS=`find ${TESTDIR}/fail -name "*.test" | sort`
for TEST in ${TESTS}
do
    BASENAME=`basename ${TEST} .test`
    echo -n "${BASENAME}... "
    TARGETNAME=${TARGET}/fail/${BASENAME}
    ${COMPILER} ${TEST} ${TARGETNAME}.c > ${TARGETNAME}.err 2>&1
    if [[ "$?" == "0" ]]
    then
	fail_test "ERROR: ${TEST} failed."
	cat ${TARGETNAME}.err
	test_separator
    else
	echo "OK"
	# rm -f ${TARGETNAME}.err
    fi
done

test_separator
# TODO add commandline flag to prevent deleting test results
if [[ "${ERRORS}" == "0" ]]
then
#    rm -rf ${TARGET}/success
#    rm -rf ${TARGET}/fail
    touch ${TARGET}/OK
    echo "All tests passed."
fi

exit ${ERRORS}