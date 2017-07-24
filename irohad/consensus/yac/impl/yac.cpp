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

#include "consensus/yac/yac.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      std::shared_ptr<Yac> Yac::create(
          std::shared_ptr<YacNetwork> network,
          std::shared_ptr<YacCryptoProvider> crypto,
          std::shared_ptr<Timer> timer, uint64_t delay) {
        return std::make_shared<Yac>(network, crypto, timer, delay);
      }

      Yac::Yac(std::shared_ptr<YacNetwork> network,
               std::shared_ptr<YacCryptoProvider> crypto,
               std::shared_ptr<Timer> timer, uint64_t delay)
          : network_(network), crypto_(crypto), timer_(timer), delay_(delay) {}

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
        if (verifyVote(from) && crypto_->verify(vote)) {
          this->applyVote(vote);
        }
      }

      void Yac::on_commit(model::Peer from, CommitMessage commit) {
        if (verifyCommit(commit) && crypto_->verify(commit)) {
          timer_->deny();
          this->applyCommit(commit);
        }
      }

      void Yac::on_reject(model::Peer from, RejectMessage reject) {
        if (verifyReject(reject) && crypto_->verify(reject)) {
          this->applyReject(reject);
          timer_->deny();
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

      void Yac::clearRoundStorage() {
        timer_->deny();
        // todo incorrect behaviour when leader dropped on propagating commit
        votes_.clear();
        voted_peers_.clear();
      };

      // ------|Apply data|------

      void Yac::applyCommit(CommitMessage commit) {
        auto hash = commit.votes.at(0).hash;
        notifier_.get_subscriber().on_next(hash);
        clearRoundStorage();
      };

      void Yac::applyReject(RejectMessage reject) {
        clearRoundStorage();
        // todo after reject missed peers should receive reject also
      };

      void Yac::applyVote(VoteMessage commit) {
        auto hash_votes = votes_[commit.hash];
        hash_votes.push_back(commit);
        if (cluster_order_.haveSupermajority(hash_votes.size())) {
          propagateCommit(commit.hash);
        }
      };

      // ------|Checking of input|------

      bool Yac::verifyVote(const model::Peer &peer) {
        if (voted_peers_.count(peer) == 0) {
          voted_peers_.insert(peer);
          return true;
        } else {
          return false;
        }
      };

      bool Yac::verifyCommit(CommitMessage commit) {
        if (commit.votes.size() == 0) return false;

        auto hash = commit.votes.at(0).hash;
        for (const auto &cmt : commit.votes) {
          if (hash != cmt.hash) return false;
        }
        return true;
      };

      bool Yac::verifyReject(RejectMessage reject) {
        if (reject.votes.empty() ||
            reject.votes.size() > cluster_order_.getNumberOfPeers())
          return false;
        auto votes = std::unordered_map<YacHash, int>();
        for (const auto &vote : reject.votes) {
          votes[vote.hash] += 1;
        }
        auto flat_map_accum = std::vector<uint32_t>(votes.size());
        std::transform(votes.begin(), votes.end(), flat_map_accum.begin(),
                       [](auto pair) { return pair.second; });
        std::sort(flat_map_accum.begin(), flat_map_accum.end(),
                  [](auto a, auto b) { return a > b; });
        auto number_of_missed_votes =
            cluster_order_.getNumberOfPeers() - votes.size();
        return !cluster_order_.haveSupermajority(flat_map_accum.at(0) +
                                                 number_of_missed_votes);
      };

      // ------|Propagation|------

      void Yac::propagateCommit(YacHash committed_hash) {
        auto votes = votes_[committed_hash];
        auto commitMsg = CommitMessage(votes);
        for (const auto &peer : cluster_order_.getPeers()) {
          network_->send_commit(peer, commitMsg);
        }
      };
    }  // namespace yac
  }    // namespace consensus
}  // namespace iroha