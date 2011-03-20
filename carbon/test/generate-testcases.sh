#!/bin/bash -u
# Copyright (C) 2011 Peterpaul Taekele Klein Haneveld

pass_cases=$( for i in $( ls pass/*.test ); do echo $( basename $i ); done )
fail_cases=$( for i in $( ls fail/*.test ); do echo $( basename $i ); done )

DEPENDENCIES=
function generate_dependencies {
    local TESTCASE=$1
    local TYPE=$2

    local DEPS=$( grep -E "^include" ${TESTCASE} | sort -u | cut -d' ' -f2 )
    DEPENDENCIES=""
    for DEP in ${DEPS}
    do
	DEPENDENCIES="${DEPENDENCIES} ${TYPE}/${DEP}.co2"
    done
}

function generate_testcase {
    local TYPE=$1 # pass/fail
    local NAME=$2

    local BASENAME=$( basename ${NAME} .test )
    local TARGET=${TYPE}/${BASENAME}.sh
    local GENERATE=$( grep ${TARGET} testdonotcreate | wc -l )

    if [[ ! -e ${TARGET} || ${GENERATE} != 1 ]];
    then
	echo "#!/bin/bash -u" > ${TARGET}
	echo >> ${TARGET}

	if [[ "${TYPE}" == "pass" ]];
	then
	    generate_dependencies ${TYPE}/${BASENAME}.test ${TYPE}
	    echo "./run_pass_test.sh ${DEPENDENCIES} ${TYPE}/${BASENAME}.test" >> ${TARGET}
	else
	    generate_dependencies ${TYPE}/${BASENAME}.test ${TYPE}
	    echo "./run_fail_test.sh ${DEPENDENCIES} ${TYPE}/${BASENAME}.test" >> ${TARGET}
	fi

	chmod +x ${TARGET}
    fi

    local IGNORE=$( grep ${TARGET} testignore | wc -l )
    if [[ ${IGNORE} == 0 ]];
    then
	echo '\'
	echo -n ${TARGET}
    fi
}

function generate_testcases {
    local TYPE=$1
    local CASES=$2
    local TESTCASE=
    for TESTCASE in ${CASES}
    do
	generate_testcase ${TYPE} ${TESTCASE}
    done
}

function generate_makefile_am_header {
    echo 'EXTRA_DIST=pass fail generate-testcases.sh run_pass_test.sh run_fail_test.sh'
    echo 'MAINTAINERCLEANFILES=Makefile.in'
    echo
    echo 'targetdir=target'
    echo
    echo 'TESTS_ENVIRONMENT=CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LD="$(LD)"'
    echo
    echo 'distclean-local:'
    echo '	-rm -rf $(targetdir)'
    echo
    echo 'clean-local:'
    echo '	-rm -f ${TESTS}'
    echo
    echo '${TESTS}, Makefile.am: ./generate-testcases.sh testignore testdonotcreate'
    echo '	./generate-testcases.sh'
    echo
    echo -n 'TESTS='
}

generate_makefile_am_header | tee Makefile.am

generate_testcases pass "${pass_cases}" | tee -a Makefile.am
generate_testcases fail "${fail_cases}" | tee -a Makefile.am
