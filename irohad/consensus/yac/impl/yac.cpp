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

#include <utility>

#include "consensus/yac/yac.hpp"
#include "consensus/yac/yac_network_interface.hpp"
#include "consensus/yac/messages.hpp"
#include "consensus/yac/cluster_order.hpp"
#include "consensus/yac/yac_gate.hpp"
#include "consensus/yac/yac_hash_provider.hpp"
#include "consensus/yac/cluster_order.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      Yac::Yac(std::shared_ptr<YacNetwork> network,
               std::shared_ptr<YacCryptoProvider> crypto,
               std::shared_ptr<Timer> timer,
               uint64_t delay) : network_(std::move(network)),
                                 crypto_(std::move(crypto)),
                                 timer_(std::move(timer)),
                                 delay_(delay) {
      }

      // ------|Hash gate|------

      void Yac::vote(YacHash hash, ClusterOrdering order) {
        this->cluster_order_ = order;
        this->current_hash_ = hash;
      };

      rxcpp::observable <YacHash> Yac::on_commit() {
        this->notifier_.get_observable();
      }

      // ------|Network notifications|------

      void Yac::on_commit(model::Peer from, CommitMessage commit) {
        auto verified = crypto_->verify(commit);
        if (verified) {
          this->applyCommit(from, commit);
          timer_->deny();
        }
      }

      void Yac::on_reject(model::Peer from, RejectMessage reject) {
        auto verified = crypto_->verify(reject);
        if (verified) {
          this->applyReject(from, reject);
          timer_->deny();
        }
      }

      void Yac::on_vote(model::Peer from, VoteMessage vote) {
        auto verified = crypto_->verify(vote);
        if (verified) {
          this->applyVote(from, vote);
        }
      }

      // ------|Private interface|------

      /**
       * Voting step is strategy of propagating vote until commit/reject message
       */
      void Yac::votingStep() {
        network_->send_vote(cluster_order_.currentLeader(),
                            crypto_->getVote(current_hash_));
        timer_->invokeAfterDelay(delay_, [this]() {
          cluster_order_.switchToNext();
          // todo if (not necessary) { ...
          this->votingStep();
        });
      }

      void Yac::applyCommit(model::Peer from, CommitMessage commit) {

      };

      void Yac::applyReject(model::Peer from, RejectMessage commit) {

      };
    } // namespace yac
  } // namespace consensus
} // iroha