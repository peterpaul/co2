#!/bin/bash -u
BASEDIR=`pwd`
COMPILER=${BASEDIR}/../src/carbon

TESTDIR=${BASEDIR}
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

function run_tests {
    local TYPE=$1 # success or fail
    local SCRIPT=$2 # ./run_succes_test.sh or ./run_fail_test.sh

    # Create output folder
    mkdir -p ${TARGET}/${TYPE} > /dev/null
    cp ${TESTDIR}/${TYPE}/*.h ${TARGET}/${TYPE}

    test_separator
    echo "${TYPE} test-cases"
    test_separator
    local TESTS=`find ${TESTDIR}/${TYPE} -name "*.test" | sort`
    for TEST in ${TESTS}
    do
	local BASENAME=`basename ${TEST} .test`
	echo -n "--- ${BASENAME}... "
	TOTAL=$(( TOTAL + 1 ))
	${SCRIPT} ${TEST}
	if [[ "$?" != "0" ]];
	then
	    fail_test "ERROR: ${TEST} failed."
	else
	    echo "OK"
	fi
    done
}

# Initialize counters
ERRORS=0
TOTAL=0

# Testcases that should work
run_tests success ./run_succes_test.sh

# Testcases that should not work
run_tests fail ./run_fail_test.sh

test_separator
# TODO add commandline flag to prevent deleting test results
if [[ "${ERRORS}" == "0" ]]
then
#    rm -rf ${TARGET}/success
#    rm -rf ${TARGET}/fail
    touch ${TARGET}/OK
    echo "All $TOTAL tests passed."
else
    echo "$ERRORS test of the $TOTAL failed."    
fi
test_separator

exit ${ERRORS}