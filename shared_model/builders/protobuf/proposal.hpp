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

#ifndef IROHA_PROTO_PROPOSAL_BUILDER_HPP
#define IROHA_PROTO_PROPOSAL_BUILDER_HPP

#include "backend/protobuf/queries/proto_query.hpp"
#include "builders/protobuf/unsigned_proto.hpp"
#include "interfaces/common_objects/types.hpp"
#include "interfaces/proposal.hpp"
#include "interfaces/transaction.hpp"

#include "ordering.pb.h"

namespace shared_model {
  namespace proto {
    template <int S = 0>
    class TemplateProposalBuilder {
     private:
      template <class T>
      using w = detail::PolymorphicWrapper<T>;

      template <int>
      friend class TemplateProposalBuilder;

      enum RequiredFields { Transactions, Height, TOTAL };

      template <int s>
      using NextBuilder = TemplateProposalBuilder<S | (1 << s)>;

      iroha::ordering::proto::Proposal proposal_;

      template <int Sp>
      TemplateProposalBuilder(const TemplateProposalBuilder<Sp> &o)
          : proposal_(o.proposal_) {}

      /**
       * Make transformation on copied content
       * @tparam Transformation - callable type for changing the copy
       * @param t - transform function for proto object
       * @return new builder with updated state
       */
      template <int Fields, typename Transformation>
      auto transform(Transformation t) const {
        NextBuilder<Fields> copy = *this;
        t(copy.proposal_);
        return copy;
      }

     public:
      TemplateProposalBuilder() = default;

      auto height(uint64_t height) const {
        return transform<Height>(
            [&](auto &proposal) { proposal.set_height(height); });
      }

      auto transactions(const std::vector<w<Transaction>> &transactions) const {
        return transform<Transactions>([&](auto &proposal) {
          for (const auto &tx : transactions) {
            proposal.mutable_transactions()->Add(tx->getTransport());
          }
        });
      }

      Proposal build() {
        static_assert(S == (1 << TOTAL) - 1, "Required fields are not set");
        return Proposal(iroha::ordering::proto::Proposal(proposal_));
      }
    };

    using ProposalBuilder = TemplateProposalBuilder<>;
  }  // namespace proto
}  // namespace shared_model
#endif  // IROHA_PROTO_PROPOSAL_BUILDER_HPP
