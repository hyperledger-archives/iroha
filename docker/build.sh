#!/bin/bash

# TODO: change warchantua to hyperledger.

# pull image from docker-hub, or (in case of fail), build new
#docker pull warchantua/iroha-dev || \
 docker build -t warchantua/iroha-dev dev 

# run dev container to build iroha
docker run -i \
    # mount ./build to /build
    -v ${PWD}/build:/build \
    # mount ${IROHA_HOME} to /opt/iroha (IROHA_HOME=/opt/iroha inside iroha-dev)
    -v ${IROHA_HOME}:/opt/iroha \
    warchantua/iroha-dev \
    # everything between COMMANDS will be executed inside a container
    cd ${IROHA_HOME}
    /build-iroha.sh || (echo "[-] Can't build iroha" && exit 1)
    /mktar-iroha.sh || (echo "[-] Can't make tarball" && exit 1)
    # at this step we have /tmp/iroha.tar 
    (cp /tmp/iroha.tar /build/iroha.tar || \
        (echo "[-] FAILED! Mount /build folder from your host or use iroha/docker/build.sh script!" && exit 1))
COMMANDS # do not put spaces/tabs before COMMANDS on this line

# build warchantua/iroha container
docker build -t warchantua/iroha build
