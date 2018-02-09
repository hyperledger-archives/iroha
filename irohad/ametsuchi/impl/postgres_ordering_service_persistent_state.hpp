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

#ifndef IROHA_POSTGRES_ORDERING_SERVICE_PERSISTENT_STATE_HPP
#define IROHA_POSTGRES_ORDERING_SERVICE_PERSISTENT_STATE_HPP

#include <cstddef>

#include "ametsuchi/impl/postgres_wsv_common.hpp"
#include "ametsuchi/ordering_service_persistent_state.hpp"

namespace iroha {
  namespace ametsuchi {

    class PostgresOrderingServicePersistentState
        : public OrderingServicePersistentState {
     public:
      explicit PostgresOrderingServicePersistentState(
          pqxx::nontransaction &transaction)
          : transaction_(transaction),
            log_(logger::log("PostgresOrderingServicePersistentState")),
            execute_{ametsuchi::makeExecute(transaction_, log_)} {}

      /**
       * Save proposal height that it can be restored
       * after launch
       */
      virtual bool saveProposalHeight(size_t height) {
        return execute(
            "UPDATE ordering_service_state "
            "SET proposal_height = "
            + transaction_.quote(height) + ";");
      }

      /**
       * Load proposal height
       */
      virtual boost::optional<size_t> loadProposalHeight() const {
        return execute_("SELECT * FROM ordering_service_state;") |
                   [&](const auto &result) -> boost::optional<size_t> {
          boost::optional<size_t> res;
          if (result.empty()) {
            log_->info("There is no proposal_height in ordering_service_state");
          } else {
            size_t height;
            auto row = result.at(0);
            row.at("proposal_height") >> height;
            res = height;
          }
          return res;
        };
      }

     private:
      pqxx::nontransaction &transaction_;
      logger::Logger log_;

      using ExecuteType = decltype(ametsuchi::makeExecute(transaction_, log_));
      ExecuteType execute_;

      // TODO: refactor to return Result when it is introduced IR-775
      bool execute(const std::string &statement) noexcept {
        return static_cast<bool>(execute_(statement));
      }
    };
  }  // namespace ordering
}  // namespace iroha

#endif  // IROHA_POSTGRES_ORDERING_SERVICE_PERSISTENT_STATE_HPP
