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
#include "model/converters/pb_transaction_factory.hpp"
#include "torii/command_service.hpp"
#include "torii/processor/transaction_processor_impl.hpp"

Irohad::Irohad(const std::string &block_store_dir,
               const std::string &redis_host, size_t redis_port,
               const std::string &pg_conn, const std::string &address)
    : context(new Context()),
      block_store_dir_(block_store_dir),
      redis_host_(redis_host),
      redis_port_(redis_port),
      pg_conn_(pg_conn),
      address_(address),
      storage(iroha::ametsuchi::StorageImpl::create(block_store_dir, redis_host,
                                                    redis_port, pg_conn)) {}

void Irohad::run() {
  // TODO : Intergrate ServerRunner and all other components here.
  auto torii_server = std::make_unique<ServerRunner>(address_);
  std::thread server_thread([&torii_server] {
    // torii_server->run()
  });
  torii_server->waitForServersReady();
  server_thread.join();
}

std::unique_ptr<torii::CommandService> Irohad::createCommandService(
    std::shared_ptr<iroha::model::converters::PbTransactionFactory> pb_factory,
    std::shared_ptr<iroha::torii::TransactionProcessor> txProccesor) {
  return std::make_unique<torii::CommandService>(pb_factory, txProccesor);
}

std::unique_ptr<torii::QueryService> Irohad::createQueryService(
    std::shared_ptr<iroha::model::converters::PbQueryFactory> pb_query_factory,
    std::shared_ptr<iroha::model::converters::PbQueryResponseFactory>
        pb_query_response_factory,
    std::shared_ptr<iroha::torii::QueryProcessor> query_processor) {
  return std::make_unique<torii::QueryService>(
      pb_query_factory, pb_query_response_factory, query_processor);
}
