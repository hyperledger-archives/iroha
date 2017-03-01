#!/bin/bash

# Run this script inside 'iroha-dev' docker container to build iroha and its dependencies!

error(){
	echo "[Error] $1"
	exit 1
}


if [ -z ${IROHA_HOME} ]; then
	error "Empty variable IROHA_HOME"
fi


# build iroha (important: build using single thread!)
(mkdir -p $IROHA_BUILD && \
cd $IROHA_BUILD && \
cmake $IROHA_HOME && \
make) || error "Can't build iroha"


mkdir -p $IROHA_BUILD/config
