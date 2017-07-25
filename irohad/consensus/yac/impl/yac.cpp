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

namespace iroha {
  namespace consensus {
    namespace yac {

      std::shared_ptr<Yac> Yac::create(
          YacVoteStorage vote_storage,
          std::shared_ptr<YacNetwork> network,
          std::shared_ptr<YacCryptoProvider> crypto,
          std::shared_ptr<Timer> timer,
          ClusterOrdering order,
          uint64_t delay) {
        return std::make_shared<Yac>(vote_storage,
                                     network,
                                     crypto,
                                     timer,
                                     order,
                                     delay);
      }

      Yac::Yac(YacVoteStorage vote_storage,
               std::shared_ptr<YacNetwork> network,
               std::shared_ptr<YacCryptoProvider> crypto,
               std::shared_ptr<Timer> timer,
               ClusterOrdering order,
               uint64_t delay)
          : vote_storage_(std::move(vote_storage)),
            network_(std::move(network)),
            crypto_(std::move(crypto)),
            timer_(std::move(timer)),
            cluster_order_(order),
            delay_(delay) {}

      // ------|Hash gate|------

      void Yac::vote(YacHash hash, ClusterOrdering order) {
        this->cluster_order_ = order;
        votingStep(hash);
      };

      rxcpp::observable<YacHash> Yac::on_commit() {
        return this->notifier_.get_observable();
      }

      // ------|Network notifications|------

      void Yac::on_vote(model::Peer from, VoteMessage vote) {
        if (crypto_->verify(vote)) {
          this->applyVote(vote);
        }
      }

      void Yac::on_commit(model::Peer from, CommitMessage commit) {
        if (crypto_->verify(commit)) {
          this->applyCommit(commit);
        }
      }

      void Yac::on_reject(model::Peer from, RejectMessage reject) {
        if (crypto_->verify(reject)) {
          this->applyReject(reject);
        }
      }

      // ------|Private interface|------

      void Yac::votingStep(YacHash hash) {
        network_->send_vote(cluster_order_.currentLeader(),
                            crypto_->getVote(hash));
        timer_->invokeAfterDelay(delay_, [this, hash]() {
          cluster_order_.switchToNext();
          if (cluster_order_.hasNext()) {
            this->votingStep(hash);
          }
        });
      }

      void Yac::closeRound() {
        timer_->deny();
      };

      // ------|Apply data|------

      void Yac::applyCommit(CommitMessage commit) {
        // todo apply to vote storage for verification
        auto hash = commit.votes.at(0).hash;
        notifier_.get_subscriber().on_next(hash);
        closeRound();
      };

      void Yac::applyReject(RejectMessage reject) {
        // todo apply to vote storage
        closeRound();
      };

      void Yac::applyVote(VoteMessage vote) {
        auto result =
            vote_storage_.storeVote(vote, cluster_order_.getNumberOfPeers());
        if (result.vote_inserted) {
          if (result.commit != nonstd::nullopt) {
            propagateCommit(*result.commit);
          } else {
            // todo propagate reject
          }
        } else {
          // todo propagate for this peer currently
        }
      };

      // ------|Propagation|------

      void Yac::propagateCommit(CommitMessage msg) {
        for (auto peer :cluster_order_.getPeers()) {
          propagateCommitDirectly(peer, msg);
        }
      }

      void Yac::propagateCommitDirectly(model::Peer to, CommitMessage msg) {
        network_->send_commit(std::move(to), std::move(msg));
      }

      void Yac::propagateReject(RejectMessage msg) {
        for (auto peer :cluster_order_.getPeers()) {
          propagateRejectDirectly(peer, msg);
        }
      }

      void Yac::propagateRejectDirectly(model::Peer to, RejectMessage msg) {
        network_->send_reject(std::move(to), std::move(msg));
      }

    }  // namespace yac
  }    // namespace consensus
}  // namespace iroha