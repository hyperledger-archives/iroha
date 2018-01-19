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

#ifndef IROHA_SHARED_MODEL_PROTO_BLOCK_HPP
#define IROHA_SHARED_MODEL_PROTO_BLOCK_HPP

#include "interfaces/iroha_internal/block.hpp"

#include <boost/range/numeric.hpp>
#include "common_objects/trivial_proto.hpp"
#include "model/block.hpp"

#include "block.pb.h"
#include "utils/lazy_initializer.hpp"

namespace shared_model {
  namespace proto {
    class Block FINAL : public CopyableProto<interface::Block,
                                             iroha::protocol::Block,
                                             Block> {
      template <class T>
      using w = detail::PolymorphicWrapper<T>;

     public:
      template <class BlockType>
      explicit Block(BlockType &&block)
          : CopyableProto(std::forward<BlockType>(block)),
            transactions_([this] {
              std::vector<w<interface::Transaction>> txs;
              for (const auto &tx : proto_->payload().transactions()) {
                auto tmp = detail::make_polymorphic<proto::Transaction>(tx);
                txs.emplace_back(tmp);
              }
              return txs;
            }),
            blob_([this] { return BlobType(proto_->SerializeAsString()); }),
            prev_hash_([this] {
              return HashType(proto_->payload().prev_block_hash());
            }),
            signatures_([this] {
              SignatureSetType sigs;
              for (const auto &sig : proto_->signatures()) {
                auto curr = detail::make_polymorphic<proto::Signature>(sig);
                sigs.insert(curr);
              }
              return sigs;
            }),
            payload_(detail::makeReferenceGenerator(
                proto_, &iroha::protocol::Block::payload)),
            payload_blob_(
                [this] { return BlobType(payload_->SerializeAsString()); })

      {}

      Block(const Block &o) : Block(o.proto_) {}

      Block(Block &&o) noexcept : Block(std::move(o.proto_)) {}

      const TransactionsCollectionType &transactions() const override {
        return *transactions_;
      }

      interface::types::HeightType height() const override {
        return payload_->height();
      }

      const HashType &prevHash() const override {
        return *prev_hash_;
      }

      const BlobType &blob() const override {
        return *blob_;
      }

      const SignatureSetType &signatures() const override {
        return *signatures_;
      }

      bool addSignature(
          const interface::types::SignatureType &signature) override {
        if (signatures_->count(signature) > 0) {
          return false;
        }

        auto sig = proto_->add_signatures();
        sig->set_pubkey(signature->publicKey().blob());
        sig->set_signature(signature->signedData().blob());
        signatures_.invalidate();
        return true;
      }

      interface::types::TimestampType createdTime() const override {
        return payload_->created_time();
      }

      TransactionsNumberType txsNumber() const override {
        return payload_->tx_number();
      }

      const typename Hashable<Block, iroha::model::Block>::BlobType &payload()
          const override {
        return *payload_blob_;
      }

     private:
      // lazy
      template <typename T>
      using Lazy = detail::LazyInitializer<T>;

      const Lazy<std::vector<w<interface::Transaction>>> transactions_;
      const Lazy<BlobType> blob_;
      const Lazy<HashType> prev_hash_;
      const Lazy<SignatureSetType> signatures_;
      const Lazy<const iroha::protocol::Block::Payload &> payload_;
      const Lazy<
          const typename Hashable<Block, iroha::model::Block>::BlobType &>
          payload_blob_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROPOSAL_HPP
