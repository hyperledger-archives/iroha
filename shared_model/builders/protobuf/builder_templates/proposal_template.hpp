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

#ifndef IROHA_PROTO_TEMPLATE_PROPOSAL_BUILDER_HPP
#define IROHA_PROTO_TEMPLATE_PROPOSAL_BUILDER_HPP

#include "backend/protobuf/proposal.hpp"
#include "interfaces/common_objects/types.hpp"
#include "validators/default_validator.hpp"

#include "ordering.pb.h"

namespace shared_model {
  namespace proto {
    template <int S = 0, typename SV = validation::DefaultProposalValidator>
    class TemplateProposalBuilder {
     private:
      template <class T>
      using w = detail::PolymorphicWrapper<T>;
      using TransactionContainer = std::vector<w<Transaction>>;

      template <int, typename>
      friend class TemplateProposalBuilder;

      enum RequiredFields { Transactions, Height, CreatedTime, TOTAL };

      template <int s>
      using NextBuilder = TemplateProposalBuilder<S | (1 << s), SV>;

      iroha::ordering::proto::Proposal proposal_;
      SV stateless_validator_;

      template <int Sp, typename SVp>
      TemplateProposalBuilder(const TemplateProposalBuilder<Sp, SVp> &o)
          : proposal_(o.proposal_),
            stateless_validator_(o.stateless_validator_) {}

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
      TemplateProposalBuilder(const SV &validator = SV())
          : stateless_validator_(validator){};

      auto height(const interface::types::HeightType height) const {
        return transform<Height>(
            [&](auto &proposal) { proposal.set_height(height); });
      }

      auto transactions(const TransactionContainer &transactions) const {
        return transform<Transactions>([&](auto &proposal) {
          for (const auto &tx : transactions) {
            proposal.mutable_transactions()->Add(tx->getTransport());
          }
        });
      }

      auto createdTime(
          const interface::types::TimestampType created_time) const {
        return transform<CreatedTime>(
            [&](auto &proposal) { proposal.set_created_time(created_time); });
      }

      Proposal build() {
        // TODO 22/01/2018 x3medima17: add stateless validator IR-836
        static_assert(S == (1 << TOTAL) - 1, "Required fields are not set");
        return Proposal(iroha::ordering::proto::Proposal(proposal_));
      }

      static const int total = RequiredFields::TOTAL;
    };
  }  // namespace proto
}  // namespace shared_model
#endif  // IROHA_PROTO_TEMPLATE_PROPOSAL_BUILDER_HPP
