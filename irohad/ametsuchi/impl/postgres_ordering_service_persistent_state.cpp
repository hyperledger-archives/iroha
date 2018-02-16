/**
 * Copyright Soramitsu Co., Ltd. 2018 All Rights Reserved.
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

#include "ametsuchi/impl/postgres_ordering_service_persistent_state.hpp"
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <cstddef>
#include "common/types.hpp"

namespace iroha {
  namespace ametsuchi {

    expected::Result<std::shared_ptr<PostgresOrderingServicePersistentState>,
                     std::string>
    PostgresOrderingServicePersistentState::create(
        std::string postgres_options) {
      auto log_ =
          logger::log("PostgresOrderingServicePersistentState:initConnection");
      log_->info("Start storage creation");

      // create connection
      auto postgres_connection =
          std::make_unique<pqxx::lazyconnection>(postgres_options);
      try {
        postgres_connection->activate();
      } catch (const pqxx::broken_connection &e) {
        return expected::makeError(
            (boost::format("Connection to PostgreSQL broken: {}") % e.what())
                .str());
      }
      log_->info("connection to PostgreSQL completed");

      // create transaction
      auto postgres_transaction = std::make_unique<pqxx::nontransaction>(
          *postgres_connection, "Storage");
      log_->info("transaction to PostgreSQL initialized");

      expected::Result<std::shared_ptr<PostgresOrderingServicePersistentState>,
                       std::string>
          storage;
      storage = expected::makeValue(
          std::shared_ptr<PostgresOrderingServicePersistentState>(
              new PostgresOrderingServicePersistentState(
                  std::move(postgres_connection),
                  std::move(postgres_transaction))));
      return storage;
    }

    PostgresOrderingServicePersistentState::
        PostgresOrderingServicePersistentState(
            std::unique_ptr<pqxx::lazyconnection> postgres_connection,
            std::unique_ptr<pqxx::nontransaction> postgres_transaction)
        : postgres_connection_(std::move(postgres_connection)),
          postgres_transaction_(std::move(postgres_transaction)),
          log_(logger::log("PostgresOrderingServicePersistentState")),
          execute_{ametsuchi::makeExecute(*postgres_transaction_, log_)} {}

    void PostgresOrderingServicePersistentState::initStorage() {
      log_->info("Init storage");
      postgres_transaction_->exec(
          "CREATE TABLE IF NOT EXISTS ordering_service_state (\n"
          "    proposal_height bigserial\n"
          ");"
          "INSERT INTO ordering_service_state\n"
          "VALUES (2); -- expected height (1 is genesis)");
    }

    void PostgresOrderingServicePersistentState::dropStorgage() {
      log_->info("Drop storage");
      postgres_transaction_->exec(
          "DROP TABLE IF EXISTS ordering_service_state;");
    }

    bool PostgresOrderingServicePersistentState::saveProposalHeight(
        size_t height) {
      log_->info("Save proposal_height in ordering_service_state "
                 + std::to_string(height));
      auto res = execute(
          "DELETE FROM ordering_service_state;\n"
          "INSERT INTO ordering_service_state "
          "VALUES ("
          + postgres_transaction_->quote(height) + ");");
      return res;
    }

    boost::optional<size_t>
    PostgresOrderingServicePersistentState::loadProposalHeight() const {
      return execute_("SELECT * FROM ordering_service_state;") |
                 [&](const auto &result) -> boost::optional<size_t> {
        boost::optional<size_t> res;
        if (result.empty()) {
          log_->error(
              "There is no proposal_height in ordering_service_state. Use "
              "default value 2.");
          res = 2;
        } else {
          size_t height;
          auto row = result.at(0);
          row.at("proposal_height") >> height;
          res = height;
          log_->info("Load proposal_height in ordering_service_state "
                     + std::to_string(height));
        }
        return res;
      };
    }

    void PostgresOrderingServicePersistentState::reset() {
      dropStorgage();
      initStorage();
    }

  }  // namespace ametsuchi
}  // namespace iroha
