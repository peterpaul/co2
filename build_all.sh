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
    run ./autogen.sh
    run ./configure --prefix=${HOME}/local/co2-0.0.1
    run make clean
    run autoscan
    run make
    run make check
    run make install
    popd
done
