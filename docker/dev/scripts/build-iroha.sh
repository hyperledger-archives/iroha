#!/bin/bash

# Run this script inside 'iroha-dev' docker container to build iroha and its dependencies!

error(){
	echo "[Error] $1"
	exit 1
}

if [ -z ${IROHA_HOME} ]; then
	error "Empty variable IROHA_HOME"
fi

if [ -z ${IROHA_BUILD} ]; then
	error "Empty variable IROHA_BUILD"
fi

if [ -z "$1" ] || [ "$1" != "Release" ] || [ "$1" != "Debug" ] ; then
	build_type="Debug"
else
	build_type="$1"
fi 


(mkdir -p $IROHA_BUILD && \
cd $IROHA_BUILD && \
cmake $IROHA_HOME -DCMAKE_BUILD_TYPE=$build_type && \
make) || error "Can't build iroha"

mkdir -p $IROHA_BUILD/config
