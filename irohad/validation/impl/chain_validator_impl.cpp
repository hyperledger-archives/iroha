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

#include "validation/impl/chain_validator_impl.hpp"

namespace iroha {
  namespace validation {

    ChainValidatorImpl::ChainValidatorImpl() {
      log_ = logger::log("ChainValidator");
    }

    bool ChainValidatorImpl::validateBlock(const model::Block &block,
                                           ametsuchi::MutableStorage &storage) {
      log_->info("validate block: height {}, hash {}", block.height,
                 block.hash.to_hexstring());
      auto apply_block = [this](const auto &block,
                                auto &query, const auto &top_hash) {
        auto peers = query.getPeers();
        if (not peers.has_value()) {
          return false;
        }
        return block.prev_hash == top_hash &&
            this->hasSupermajority(block.sigs.size(),
                                   peers.value().size()) &&
            this->peersSubset(block.sigs, peers.value());
      };

      // Apply to temporary storage
      return storage.apply(block, apply_block);
    }

    bool ChainValidatorImpl::validateChain(Commit blocks,
                                           ametsuchi::MutableStorage &storage) {
      log_->info("validate chain...");
      return blocks
          .all([this, &storage](auto block) {
            log_->info("Validating block: height {}, hash {}",
                       block.height,
                       block.hash.to_hexstring());
            return this->validateBlock(block, storage);
          })
          .as_blocking()
          .first();
    }

    bool ChainValidatorImpl::hasSupermajority(uint64_t current, uint64_t all) {
      if (current > all)
        return false;
      auto f = (all - 1) / 3.0;
      return current >= 2 * f + 1;
    }

    bool ChainValidatorImpl::peersSubset(
        std::vector<model::Signature> signatures,
        std::vector<model::Peer> peers) {
      return std::all_of(
          signatures.begin(), signatures.end(), [peers](auto signature) {
            return std::find_if(peers.begin(), peers.end(),
                                [signature](auto peer) {
                                  return signature.pubkey == peer.pubkey;
                                }) != peers.end();
          });
    }
  }
}
