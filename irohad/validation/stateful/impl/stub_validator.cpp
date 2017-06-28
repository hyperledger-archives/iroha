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

#include <validation/stateful/stub_validator.hpp>
#include <dao/dao.hpp>
namespace iroha {
  namespace validation {

    /**
     * Interface for performing stateful validation
     */
    ValidatorStub::validate(const iroha::dao::Proposal &proposal,
                            ametsuchi::TemporaryWsv &wsv) = 0 {

      decltype(proposal.transactions) valid_transactions = {};

      for (auto tx = proposal.transactions.begin();
           it != proposal.transactions.end(); ++tx) {

        auto correct_transaction = wsv
            .apply(*tx, [](dao::Transaction &tx,
                           ametsuchi::CommandExecutor &executor,
                           ametsuchi::WsvQuery &query) {

              for (auto command = tx->commands.begin();
                   command != tx->commands.end(); ++command) {
                executor.execute(command);
                if (!this->command_validator.validate(*command)) {
                  return false;
                }
              }
              return true;
            });
      }
      return dao::Proposal(valid_transactions);
    }
  } // namespace validation
} // namespace iroha