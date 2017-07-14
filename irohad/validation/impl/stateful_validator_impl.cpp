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

#include <algorithm>
#include "validation/impl/stateful_validatior_impl.hpp"

namespace iroha {
  namespace validation {

    model::Proposal StatefulValidatorImpl::validate(
        const model::Proposal &proposal, ametsuchi::WsvCommand &wsv_commands,
        ametsuchi::WsvQuery &wsv_queries) {
      //
      std::vector<model::Transaction> verified_transactions;
      for (auto tx : proposal.transactions) {
        auto account = wsv_queries.getAccount(tx.creator_account_id);
        // Check if tx creator has account and has quorum to execute transaction
        if (account && tx.signatures.size() >= account.value().quorum) {
          // TODO: check that signatories are in tx creator account
          // auto account_signs = queries.getSignatories(tx.creator_account_id);

          if (std::all_of(std::begin(tx.commands), std::end(tx.commands),
                          [](model::Command command) {
                            return command.validate(wsv_queries,
                                                    account.value()) &&
                                   command.execute(wsv_queries, wsv_commands);
                          }))
            // If all commands are valid - add that tx is verified
            verified_transactions.push_back(tx);
        }
      }

      // Return verified Proposal
      return model::Proposal(verified_transactions);
    }

  }  // namespace validation
}  // namespace iroha