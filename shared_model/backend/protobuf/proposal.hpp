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

#ifndef IROHA_SHARED_MODEL_PROTO_PROPOSAL_HPP
#define IROHA_SHARED_MODEL_PROTO_PROPOSAL_HPP

#include "interfaces/proposal.hpp"
#include "interfaces/transaction.hpp"

#include <boost/range/numeric.hpp>
#include "common_objects/trivial_proto.hpp"
#include "model/proposal.hpp"

#include "block.pb.h"
#include "ordering.pb.h"
#include "utils/lazy_initializer.hpp"

#include "transaction.hpp"
namespace shared_model {

  namespace proto {
    class Proposal final
        : public CopyableProto<interface::Proposal,
                               iroha::ordering::proto::Proposal,
                               Proposal> {
      template <class T>
      using w = detail::PolymorphicWrapper<T>;

    public:
      template <class ProposalType>
      explicit Proposal(ProposalType &&proposal)
          : CopyableProto(std::forward<ProposalType>(proposal)),
            transactions_([this] {
              return boost::accumulate(proto_->transactions(),
                                       std::vector<w<interface::Transaction>>{},
                                       [](auto&& vec, const auto& tx){
                                         vec.emplace_back(new proto::Transaction(tx));
                                         return std::forward<decltype(vec)>(vec);
                                       });

            }),
            blob_([this] { return makeBlob(*proto_); }) {}

      Proposal(const Proposal &o) : Proposal(o.proto_) {}

      Proposal(Proposal &&o) noexcept : Proposal(std::move(o.proto_)) {}

      const std::vector<w<interface::Transaction>> &transactions()
          const override {
        return *transactions_;
      }

      uint64_t height() const override {
        return proto_->height();
      }

      const BlobType &blob() const override {
        return *blob_;
      }

     private:

      // lazy
      template <typename T>
      using Lazy = detail::LazyInitializer<T>;

      const Lazy<std::vector<w<interface::Transaction>>> transactions_;
      const Lazy<BlobType> blob_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROPOSAL_HPP_HPP
