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
#include "network/impl/peer_communication_service_impl.hpp"

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
  std::thread server_thread([this] {
    // Protobuf converters
    auto pb_tx_factory =
        std::make_shared<iroha::model::converters::PbTransactionFactory>();
    auto pb_query_factory =
        std::make_shared<iroha::model::converters::PbQueryFactory>();
    auto pb_query_response_factory =
        std::make_shared<iroha::model::converters::PbQueryResponseFactory>();
    // Crypto Provider:
    auto crypto_verifier =
        std::make_shared<iroha::model::ModelCryptoProviderImpl>();
    // Validators:
    auto stateless_validator = createStatelessValidator(crypto_verifier);

    // PeerCommunicationService
    // TODO: replace with create
    // auto pcs = createPeerCommunicationService();
    // Torii:
    // --- Transactions:
    // auto tx_processor = createTransactionProcessor(pcs, stateless_validator);
    // auto comand_service = createCommandService(pb_tx_factory, tx_processor);
    // --- Queries
    auto query_proccessing_factory =
        createQueryProcessingFactory(storage, storage);
    auto query_processor =
        createQueryProcessor(std::move(query_proccessing_factory), stateless_validator);
    auto query_service = createQueryService(
        pb_query_factory, pb_query_response_factory, query_processor);
    //torii_server->run(comand_service, query_service);

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

std::shared_ptr<iroha::torii::QueryProcessor> Irohad::createQueryProcessor(
    std::unique_ptr<iroha::model::QueryProcessingFactory> qpf,
    std::shared_ptr<iroha::validation::StatelessValidator>
        stateless_validator) {
  return std::make_shared<iroha::torii::QueryProcessorImpl>(
      std::move(qpf), stateless_validator);
}

std::shared_ptr<iroha::torii::TransactionProcessor>
Irohad::createTransactionProcessor(
    std::shared_ptr<iroha::network::PeerCommunicationService> pcs,
    std::shared_ptr<iroha::validation::StatelessValidator> validator) {
  return std::make_shared<iroha::torii::TransactionProcessorImpl>(pcs,
                                                                  validator);
}
std::shared_ptr<iroha::validation::StatelessValidator>
Irohad::createStatelessValidator(
    std::shared_ptr<iroha::model::ModelCryptoProvider> crypto_provider) {
  return std::make_shared<iroha::validation::StatelessValidatorImpl>(
      crypto_provider);
}
std::unique_ptr<iroha::model::QueryProcessingFactory>
Irohad::createQueryProcessingFactory(
    std::shared_ptr<iroha::ametsuchi::WsvQuery> wsvQuery,
    std::shared_ptr<iroha::ametsuchi::BlockQuery> blockQuery) {
  return std::make_unique<iroha::model::QueryProcessingFactory>(wsvQuery,
                                                                blockQuery);
}
