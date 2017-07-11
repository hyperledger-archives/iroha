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

#include <torii/processor/transaction_processor_stub.hpp>

namespace iroha {
  namespace torii {

    using validation::StatelessValidator;
    using model::TransactionResponse;
    using model::ModelCryptoProvider;

    TransactionProcessorStub::TransactionProcessorStub(
        const StatelessValidator &validator,
        ModelCryptoProvider &crptoProvider)
        : validator_(validator), crptoProvider_(crptoProvider) {
      // Handle on_proposal
      auto proposal_tx_filter = [](const auto &tx) {
        // TODO filter depending on client-tx map
        return true;
      };
      auto proposal_tx_map = [](const auto &tx) {
        // TODO form response
        model::TransactionResponse res;
        res.msg = "proposal";
        return res;
      };
      auto proposal_response = [proposal_tx_filter,
                                proposal_tx_map](auto proposal) {
        return rxcpp::observable<>::from(proposal)
            .filter(proposal_tx_filter)
            .map(proposal_tx_map);
      };
      // Handle on_commit
      auto identity = [](auto commit) { return commit; };
      auto commit_tx_filter = [](const auto &tx) {
        // TODO filter depending on client-tx map
        return true;
      };
      auto commit_tx_map = [](const auto &tx) {
        // TODO form response;
        model::TransactionResponse res;
        res.msg = "commit";
        return res;
      };
      auto commit_response = [commit_tx_filter, commit_tx_map](auto block) {
        return rxcpp::observable<>::from(block)
            .filter(commit_tx_filter)
            .map(commit_tx_map);
      };
    }

    void TransactionProcessorStub::transaction_handle(model::Client client,
                                          model::Transaction &transaction) {
      if (validator_.validate(transaction)) {
        // TODO accumulate client-tx map, send tx to ordering service

      }
    }

  }  // namespace torii
}  // namespace iroha
