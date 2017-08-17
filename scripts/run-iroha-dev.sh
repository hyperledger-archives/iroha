#!/bin/bash

USER=$(whoami)
USERID=$(id -u ${USER})

CURDIR="$(cd "$(dirname "$0")"; pwd)"
IROHA_HOME="$(dirname "${CURDIR}")"
PROJECT=iroha

docker-compose -f ${IROHA_HOME}/docker/docker-compose.yml -p ${PROJECT} down
docker-compose -f ${IROHA_HOME}/docker/docker-compose.yml -p ${PROJECT} up -d
docker-compose -f ${IROHA_HOME}/docker/docker-compose.yml -p ${PROJECT} exec \
    node adduser --disabled-password --gecos "" ${USER} --uid ${USERID}
docker-compose -f ${IROHA_HOME}/docker/docker-compose.yml -p ${PROJECT} exec \
    --user ${USER} node /bin/bash
