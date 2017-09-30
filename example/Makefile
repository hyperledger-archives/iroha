# Copyright 2017 Soramitsu Co., Ltd. All Rights Reserved.
#
#---------------------------------------------------------------
# This makefile defines the following targets
#
# - all (default) - buid iroha-build container, and build iroha
#---------------------------------------------------------------
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

.PHONY: help up dwon logs hakodate

IROHA_HOME := $(shell pwd)

help:
	@echo "help          - show make targets"
	@echo "up            - running iroha container by docker-compose"
	@echo "down          - stop and remove iroha container by docker-compose"
	@echo "logs          - show logs of iroha_node_1 container"
	@echo "hakodate      - hakodate hands-on sample using iroha-cli"

up:
	env COMPOSE_PROJECT_NAME=iroha docker-compose -p iroha up -d

down:
	env COMPOSE_PROJECT_NAME=iroha docker-compose -p iroha down

logs:
	docker logs -f iroha_node_1

hakodate:
	bash hakodate.sh
