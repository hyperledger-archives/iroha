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

#include "synchronization/impl/synchronizer_impl.hpp"

namespace iroha {
  namespace synchronization {

    SynchronizerImpl::SynchronizerImpl(
        validation::ChainValidator &validator,
        ametsuchi::MutableFactory &mutableFactory,
        network::BlockLoader &blockLoader)
        : validator_(validator),
          mutableFactory_(mutableFactory),
          blockLoader_(blockLoader) {}

    void SynchronizerImpl::process_commit(iroha::model::Block commit_message) {
      auto storage = mutableFactory_.createMutableStorage();
      if (validator_.validate_block(commit_message, *storage)) {
        // Block can be applied to current storage
        // Commit to main Ametsuchi
        mutableFactory_.commit(std::move(storage));

        auto single_commit =
            rxcpp::observable<>::create<model::Block>([&commit_message](auto s) {
              s.on_next(commit_message);
              s.on_completed();
            });

        notifier_.get_subscriber().on_next(single_commit);
      } else {
        // Block can't be applied to current storage
        // Download all missing blocks
        // TODO: Loading blocks from other Peer
        // TODO: Replace with fair realization
        auto stub_peer = model::Peer();
        auto stub_block = model::Block();
        auto chain = blockLoader_.requestBlocks(stub_peer, stub_block);
        storage = mutableFactory_.createMutableStorage();

        if (validator_.validate_chain(chain, *storage)) {
          // Chain can be applied to ametsuchi
          mutableFactory_.commit(std::move(storage));
          notifier_.get_subscriber().on_next(chain);
        } else {
          // Chain is wrong, try other peer
          // TODO: Replace if with loop over all other peers
        }
      }
    }

    rxcpp::observable<Synchronizer::Commit>
    SynchronizerImpl::on_commit_chain() {
      return notifier_.get_observable();
    }
  }
}