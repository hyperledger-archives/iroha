#!/bin/bash
CURDIR="$(cd "$(dirname "$0")"; pwd)"
IROHA_HOME="$(dirname "${CURDIR}")"
PROJECT=iroha${USERID}
COMPOSE=${IROHA_HOME}/docker/docker-compose.yml
USER=$(whoami)

export G_ID=$(id -g $USER)
export U_ID=$(id -u $USER)
export COMPOSE_PROJECT_NAME=${PROJECT}

docker-compose -f ${COMPOSE} up -d
docker-compose -f ${COMPOSE} exec node /bin/bash
