#!/bin/bash

function run {
    echo "$@"
    local OUT=$( $@ )
    if [[ "$?" != "0" ]];
    then
	echo "FAILED: $@"
	echo "${OUT}"
	exit 1
    fi
}

DIRS=$( find . -type d -maxdepth 1 | grep -v .git )
for DIR in ${DIRS}
do
    echo "---------------------------------------------------------------------------"
    echo "--- ${DIR}"
    echo "---------------------------------------------------------------------------"

    pushd ${DIR}
    run make $@
    popd
done
