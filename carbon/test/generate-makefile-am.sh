#!/bin/bash -u
# Copyright (C) 2011 Peterpaul Taekele Klein Haneveld

pass_cases=$( for i in $( ls pass/*.test ); do echo $( basename $i ); done )
fail_cases=$( for i in $( ls fail/*.test ); do echo $( basename $i ); done )

function generate_testcase {
    local TYPE=$1 # pass/fail
    local NAME=$2

    local BASENAME=$( basename ${NAME} .test )
    local TARGET=${TYPE}/${BASENAME}.sh

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
    echo 'EXTRA_DIST=pass fail generate-testcases.sh run_pass_test.sh run_fail_test.sh testignore testdonotcreate'
    echo 'MAINTAINERCLEANFILES=Makefile.in'
    echo
    echo 'targetdir=target'
    echo
    echo 'TESTS_ENVIRONMENT=CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LD="$(LD)" SRCDIR="$(srcdir)"'
    echo
    echo 'distclean-local:'
    echo '	-rm -rf $(targetdir)'
    echo
    echo 'clean-local:'
    echo '	-rm -f $(TESTS)'
    echo '	-cat $(srcdir)/testdonotcreate | xargs rm -f'
    echo '	-cat $(srcdir)/testignore | xargs rm -f'
    echo
    echo '$(TESTS): $(srcdir)/generate-testcases.sh $(srcdir)/testdonotcreate'
    echo '	$(srcdir)/generate-testcases.sh $(srcdir)'
    echo
    echo 'Makefile.am: $(srcdir)/generate-makefile-am.sh $(srcdir)/testignore'
    echo '	$(srcdir)/generate-makefile-am.sh'
    echo
    echo -n 'TESTS='
}

generate_makefile_am_header | tee Makefile.am

generate_testcases pass "${pass_cases}" | tee -a Makefile.am
generate_testcases fail "${fail_cases}" | tee -a Makefile.am
# newline to cleanup commandline output
echo
