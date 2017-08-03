/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef IROHA_APPLICATION_HPP
#define IROHA_APPLICATION_HPP

#include <main/context.hpp>
#include <network/peer_communication_service.hpp>
#include <torii/processor/query_processor_impl.hpp>
#include <torii/processor/transaction_processor_impl.hpp>
#include <validation/impl/stateless_validator_impl.hpp>

#include <ametsuchi/impl/storage_impl.hpp>
#include <crypto/crypto.hpp>

#include "main/server_runner.hpp"
#include "model/model_crypto_provider_impl.hpp"
#include "torii/command_service.hpp"

class Irohad {
 public:
  std::shared_ptr<Context> context;

  Irohad(const std::string &block_store_dir, const std::string &redis_host,
         size_t redis_port, const std::string &pg_conn,
         const std::string &address);
  void run();

  std::unique_ptr<torii::CommandService> createCommandService(
      std::shared_ptr<iroha::model::converters::PbTransactionFactory>
          pb_factory,
      std::shared_ptr<iroha::torii::TransactionProcessor> txProccesor);

  std::unique_ptr<torii::QueryService> createQueryService(
      std::shared_ptr<iroha::model::converters::PbQueryFactory>
          pb_query_factory,
      std::shared_ptr<iroha::model::converters::PbQueryResponseFactory>
          pb_query_response_factory,
      std::shared_ptr<iroha::torii::QueryProcessor> query_processor);

 private:
  std::string block_store_dir_;
  std::string redis_host_;
  size_t redis_port_;
  std::string pg_conn_;
  std::string address_;

 public:
  std::shared_ptr<iroha::ametsuchi::StorageImpl> storage;

};

#endif  // IROHA_APPLICATION_HPP