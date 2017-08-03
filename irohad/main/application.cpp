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

#include "main/application.hpp"
#include "torii/command_service.hpp"
#include "model/converters/pb_transaction_factory.hpp"
#include "torii/processor/transaction_processor_impl.hpp"

using namespace iroha;

Irohad::Irohad(const std::string &block_store_dir,
               const std::string &redis_host, size_t redis_port,
               const std::string &pg_conn, const std::string &address)
    : context(new Context()),
      block_store_dir_(block_store_dir),
      redis_host_(redis_host),
      redis_port_(redis_port),
      pg_conn_(pg_conn),
      storage(iroha::ametsuchi::StorageImpl::create(block_store_dir, redis_host,
                                                    redis_port, pg_conn)),
      server_runner_(new ServerRunner(address)){}

void Irohad::run() {
  // TODO : Intergrate ServerRunner and all other components here.
}
