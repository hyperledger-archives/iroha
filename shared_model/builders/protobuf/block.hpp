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

#ifndef IROHA_PROTO_BLOCK_BUILDER_HPP
#define IROHA_PROTO_BLOCK_BUILDER_HPP

#include "backend/protobuf/queries/proto_query.hpp"
#include "block.pb.h"
#include "builders/protobuf/unsigned_proto.hpp"
#include "interfaces/base/hashable.hpp"
#include "interfaces/base/signable.hpp"
#include "interfaces/common_objects/types.hpp"
#include "interfaces/iroha_internal/block.hpp"
#include "interfaces/transaction.hpp"

namespace shared_model {
  namespace proto {
    template <int S = 0>
    class TemplateBlockBuilder {
     private:
      template <class T>
      using w = detail::PolymorphicWrapper<T>;

      template <int>
      friend class TemplateBlockBuilder;

      enum RequiredFields {
        Transactions,
        TxNumber,
        Height,
        PrevHash,
        CreatedTime,
        Signatures,
        TOTAL
      };

      template <int s>
      using NextBuilder = TemplateBlockBuilder<S | (1 << s)>;

      iroha::protocol::Block block_;

      template <int Sp>
      TemplateBlockBuilder(const TemplateBlockBuilder<Sp> &o)
          : block_(o.block_) {}

     public:
      TemplateBlockBuilder() = default;

      NextBuilder<Transactions> transactions(
          const std::vector<w<Transaction>> &transactions) {
        for (const auto &tx : transactions) {
          block_.payload().mutable_transactions()->Add(tx->getTransport());
        }
        return *this;
      }

      NextBuilder<TxNumber> tx_number(Block::TransactionsNumberType tx_number) {
        block_.payload().set_tx_number(tx_number);
        return *this;
      }

      NextBuilder<Height> height(interface::types::HeightType height) {
        block_.payload().set_height(height);
        return *this;
      }

      NextBuilder<PrevHash> prev_hash(crypto::Hash hash) {
        block_.payload().set_prev_block_hash(hash);
        return *this;
      }

      NextBuilder<CreatedTime> created_time(
          interface::types::TimestampType time) {
        block_.payload().set_created_time(time);
        return *this;
      }

      NextBuilder<Signatures> signatures(
          const interface::Signable<Block,
                                    iroha::model::Block>::SignatureSetType
              &signatures) {
        for (const auto &signature : signatures) {
          auto sig = block_.add_signatures();
          sig->set_pubkey(signature->publicKey().blob());
          sig->set_signature(signature->signedData().blob());
        }
        return *this;
      }

      UnsignedWrapper<Block> build() {
        static_assert(S == (1 << TOTAL) - 1, "Required fields are not set");
        return UnsignedWrapper<Block>(Block(iroha::protocol::Block(block_)));
      }

      static const int total = RequiredFields::TOTAL;
    };

    using BlockBuilder = TemplateBlockBuilder<>;
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_BLOCK_BUILDER_HPP
