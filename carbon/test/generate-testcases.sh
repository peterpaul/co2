#!/bin/bash

succes_cases=$( for i in $( ls success/*.test ); do echo $( basename $i ); done )
fail_cases=$( for i in $( ls fail/*.test ); do echo $( basename $i ); done )

function generate_testcase {
    local TYPE=$1 # success/fail
    local NAME=$2

    local BASENAME=$( basename ${NAME} .test )

    local TARGET=${TYPE}_${BASENAME}.sh

    echo "#!/bin/bash -u" > ${TARGET}
    echo >> ${TARGET}

    if [[ "${TYPE}" == "success" ]];
    then
	echo "./run_succes_test.sh ${TYPE}/${BASENAME}.test" >> ${TARGET}
    else
	echo "./run_fail_test.sh ${TYPE}/${BASENAME}.test" >> ${TARGET}
    fi

    chmod +x ${TARGET}

    echo '\' >> Makefile.am
    echo -n ${TARGET} >> Makefile.am
}

function generate_testcases {
    local TYPE=$1
    local CASES=$2
    
    for TESTCASE in ${CASES}
    do
	generate_testcase ${TYPE} ${TESTCASE}
    done
}

function generate_makefile_am_header {
    echo 'EXTRA_DIST = success fail generate-testcases.sh run_succes_test.sh run_fail_test.sh'
    echo 'MAINTAINERCLEANFILES = Makefile.in'
    echo
    echo '#SUBDIRS = success fail'
    echo
    echo 'targetdir = target'
    echo
    echo 'TESTS_ENVIRONMENT = CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"'
    echo
    echo 'distclean-local:'
    echo '	-rm -rf $(targetdir)'
    echo
    echo 'clean-local:'
    echo '	-rm -f ${TESTS}'
    echo
    echo '${TESTS}: ./generate-testcases.sh'
    echo '	./generate-testcases.sh'
    echo
    echo -n 'TESTS='
}

generate_makefile_am_header > Makefile.am

generate_testcases success "${succes_cases}"
generate_testcases fail "${fail_cases}"
