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

#include <ametsuchi/impl/postgres_wsv_query.hpp>

namespace iroha {
  namespace ametsuchi {

    PostgresWsvQuery::PostgresWsvQuery(
        std::unique_ptr<pqxx::nontransaction> &transaction)
        : transaction_(transaction) {}

    model::Account PostgresWsvQuery::getAccount(const std::string &account_id) {
      model::Account account;
      pqxx::result result;
      try {
//        result = transaction_->exec();
      } catch (const std::exception &e) {
        // TODO log
        return account;
      }
      if (result.size() != 1){
        return account;
      }

      return account;
    }

    std::vector<ed25519::pubkey_t> PostgresWsvQuery::getSignatories(
        const std::string &account_id) {
      std::vector<ed25519::pubkey_t> result;
      return result;
    }

    model::Asset PostgresWsvQuery::getAsset(const std::string &asset_id) {
      model::Asset result;
      result.name = "";
      return result;
    }

    model::AccountAsset PostgresWsvQuery::getAccountAsset(
        const std::string &account_id, const std::string &asset_id) {
      model::AccountAsset result;
      result.account_id = "";
      return result;
    }

    model::Peer PostgresWsvQuery::getPeer(const std::string &address) {
      model::Peer result;
      result.address = "";
      return result;
    }
  }  // namespace ametsuchi
}  // namespace iroha