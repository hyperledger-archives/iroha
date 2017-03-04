#!/bin/bash

if [ -z ${IROHA_RELEASE} ]; then
  echo "[FATAL][mktar-iroha.sh] Empty variable IROHA_RELEASE"
  exit 1
fi

if [ -z ${IROHA_BUILD} ]; then
  echo "[FATAL][mktar-iroha.sh] Empty variable IROHA_BUILD"
  exit 1
fi


if [ -z ${IROHA_HOME} ]; then
  echo "[FATAL][mktar-iroha.sh] Empty variable IROHA_HOME"
  exit 1
fi

mkdir -p $IROHA_RELEASE
cd ${IROHA_RELEASE}
rsync -avr ${IROHA_BUILD}/bin $IROHA_RELEASE && \
rsync -avr ${IROHA_BUILD}/lib $IROHA_RELEASE && \
rsync -avr ${IROHA_BUILD}/test_bin $IROHA_RELEASE && \
rsync -avr ${IROHA_HOME}/jvm $IROHA_RELEASE && \
rsync -avr ${IROHA_HOME}/smart_contract/java_tests $IROHA_RELEASE && \
rsync -avr ${IROHA_HOME}/external/src/google_leveldb/out-shared/lib* $IROHA_RELEASE/lib && \
rsync -avr ${IROHA_HOME}/config $IROHA_RELEASE

tar cvf /tmp/iroha.tar \
  /usr/lib/libproto* \
  /usr/local

