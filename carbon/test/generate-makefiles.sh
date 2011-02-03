#!/bin/bash

pass_cases=$( for i in $( ls pass/*.test ); do echo $( basename $i ); done )
fail_cases=$( for i in $( ls fail/*.test ); do echo $( basename $i ); done )

function generate_testcase {
    local testcase=$1
    local name=$( basename ${testcase} .test )
    echo "${name}_SOURCES = ${name}.c"
    echo "${name}_CPPFLAGS = -I\$(top_srcdir)/include"
    echo "${name}_LDFLAGS = \$(CO2_LIBS) -lm"
    echo "${name}.c: ${testcase}"
    echo "	\$(top_srcdir)/src/carbon \$< \$@"
    echo
}

function generate_makefile {
    local cases=$1
    echo -n "bin_PROGRAMS="
    for testcase in ${cases}
    do
	name=$( basename ${testcase} .test )
	echo "\\"
	echo -n $name
    done
    echo
    echo
    
    for testcase in ${cases}
    do
	generate_testcase ${testcase}
    done

    echo "clean-local:"
    for testcase in ${cases}
    do
	name=$( basename ${testcase} .test )
	echo "	-rm -f ${name}.c ${name}.h"
    done
}

generate_makefile "${pass_cases}" > pass/Makefile.am
generate_makefile "${fail_cases}" > fail/Makefile.am
