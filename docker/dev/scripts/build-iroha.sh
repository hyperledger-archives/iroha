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


(mkdir -p $IROHA_BUILD && \
cd $IROHA_BUILD && \
cmake $IROHA_HOME && \
make) || error "Can't build iroha"

mkdir -p $IROHA_BUILD/config
