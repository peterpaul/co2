#!/bin/bash -u

BASEDIR=`pwd`
COMPILER=${BASEDIR}/Compiler.bin

TARGET=test/target
# Remove previous output
rm -rf ${TARGET}

# Create output folder
mkdir -p ${TARGET}/success > /dev/null
mkdir -p ${TARGET}/fail > /dev/null

ERRORS=0
# Testcases that should work
TESTS=`find test/success -name "*.test"`
for TEST in ${TESTS}
do
    BASENAME=`basename ${TEST} .test`
    TARGETNAME=${TARGET}/success/${BASENAME}
    ${COMPILER} ${TEST} ${TARGETNAME}.c > ${TARGETNAME}.err 2>&1
    if [[ "$?" != "0" ]]
    then
	ERRORS=$(( ERRORS + 1 ))
	cat ${TARGETNAME}.err
	echo "ERROR: ${TEST} failed."
    else
	gcc ${TARGETNAME}.c -o ${TARGETNAME}.bin
	if [[ "$?" != "0" ]]
	then
	    ERRORS=$(( ERRORS + 1 ))
	    echo "ERROR: ${TEST} failed."
	else
	    ${TARGETNAME}.bin
	fi
	rm ${TARGETNAME}.c ${TARGETNAME}.err
    fi
done

# Testcases that should not work
TESTS=`find test/fail -name "*.test"`
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
	rm -f ${TARGETNAME}.c ${TARGETNAME}.err
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