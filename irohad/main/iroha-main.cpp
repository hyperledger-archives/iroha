/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <api/command_service.hpp>
#include <api/query_service.hpp>
#include <consensus/connection/service.hpp>
#include <ordering/connection/service.hpp>

#include "server_runner.hpp"

int main(int argc,char* argv[]) {

  connection::api::CommandService commandService;
  connection::api::QueryService queryService;
  connection::consensus::SumeragiService sumeragiService;
  connection::ordering::OrderingService orderingService;

  connection::ServerRunner serverRunner("0.0.0.0", {
      &commandService,
      &queryService,
      &sumeragiService,
      &orderingService
  });

  return 0;
}
