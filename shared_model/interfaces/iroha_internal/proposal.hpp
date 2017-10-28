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

#include "interfaces/hashable.hpp"
#include "interfaces/transaction.hpp"
#include "model/proposal.hpp"

#ifndef IROHA_SHARED_MODEL_PROPOSAL_HPP
#define IROHA_SHARED_MODEL_PROPOSAL_HPP
namespace shared_model {
  namespace interface {

    class Proposal : public Hashable<Proposal, iroha::model::Proposal> {
      /// Type of proposal transactions' collection
      using ProposalTransactionsType = std::vector<Transaction>;

      /**
       * @return collection of proposal's transactions
       */
      virtual ProposalTransactionsType &transactions() const = 0;

      /// Type of proposal height
      using ProposalHeight = uint64_t;

      /**
       * @return height of proposal
       */
      virtual ProposalHeight &height() const = 0;

      iroha::model::Proposal *makeOldModel() const {
        std::vector<iroha::model::Transaction> txs;
        for (auto &tx : transactions()) {
          txs.push_back(*tx.makeOldModel());
        }
        iroha::model::Proposal *oldStyleProposal =
            new iroha::model::Proposal(txs);
        oldStyleProposal->height = height();
        return oldStyleProposal;
      }

      std::string toString() const {
        std::string result("Proposal: [");
        result += "height=" + height() + ", ";
        result += "transactions=[";
        for (auto &tx : transactions()) {
          result += tx.toString() + " ";
        }
        result += "]]";
        return result;
      }
    };

  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_PROPOSAL_HPP
