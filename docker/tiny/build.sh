#!/bin/bash

if [ -z ${IROHA_HOME} ]; then
    echo "[FATAL] Empty variable IROHA_HOME"
    exit 1
fi

error(){
	echo "[Error] $1"
	exit 1
}

SOURCE=${IROHA_HOME}
BUILD=/tmp/iroha-build
TINY=${IROHA_HOME}/docker/tiny
RELEASE=${TINY}/iroha

export LD_LIBRARY_PATH=${TINY}/libx86_64

mkdir -p ${BUILD}
cd ${BUILD}
cmake ${IROHA_HOME} -DCMAKE_BUILD_TYPE=Release
make -j 10 || error "Can't build iroha"


rsync -avr ${BUILD}/bin ${RELEASE} && \
rsync -avr ${BUILD}/test_bin ${RELEASE} && \
rsync -avr ${TINY}/libx86_64 ${RELEASE} && \
rsync -avr ${TINY}/scripts ${RELEASE} && \
rsync -avr ${IROHA_HOME}/smart_contract/java_tests ${RELEASE} && \
rsync -avr ${IROHA_HOME}/jvm ${RELEASE} && \
rsync -avr ${IROHA_HOME}/external/src/google_leveldb/out-shared/lib* ${RELEASE}/libx86_64 && \
rsync -avr ${IROHA_HOME}/config ${RELEASE} || error "Can not copy release files"


docker build -t hyperledger/iroha-docker ${TINY}


rm -rf ${TINY}/iroha


cat << "EOF"
 _______   ______   .__   __.  _______ 
|       \ /  __  \  |  \ |  | |   ____|
|  .--.  |  |  |  | |   \|  | |  |__   
|  |  |  |  |  |  | |  . `  | |   __|  
|  '--'  |  `--'  | |  |\   | |  |____ 
|_______/ \______/  |__| \__| |_______|
                                      
EOF
