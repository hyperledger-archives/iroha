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

#include "consensus/yac/storage/yac_vote_storage.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      nonstd::optional<Answer> YacVoteStorage::store(VoteMessage vote,
                                                     uint64_t peers_in_round) {
        return findProposalStorage(vote, peers_in_round)->insert(vote);
      }

      nonstd::optional<Answer> YacVoteStorage::store(CommitMessage commit,
                                                     uint64_t peers_in_round) {
        return insert_votes(commit.votes, peers_in_round);
      }

      nonstd::optional<Answer> YacVoteStorage::store(RejectMessage reject,
                                                     uint64_t peers_in_round) {
        return insert_votes(reject.votes, peers_in_round);
      }

      std::shared_ptr<YacProposalStorage>
      YacVoteStorage::getProposalStorage(ProposalHash hash) {
        for (auto storage: proposal_storages_) {
          if (storage->getProposalHash() == hash) {
            return storage;
          }
        }
        return nullptr;
      }

      bool YacVoteStorage::getProcessingState(const ProposalHash &hash) {
        auto val = processing_state_.find(hash);
        if (val == processing_state_.end()) {
          return false;
        }

        return val->second;
      }

      void YacVoteStorage::markAsProcessedState(const ProposalHash &hash) {
        processing_state_[hash] = true;
      }

      // --------| private api |--------

      nonstd::optional<Answer>
      YacVoteStorage::insert_votes(std::vector<VoteMessage> &votes,
                                   uint64_t peers_in_round) {
        if (not sameProposals(votes)) {
          return nonstd::nullopt;
        }

        auto storage = findProposalStorage(votes.at(0), peers_in_round);
        return storage->insert(votes);
      }

      std::shared_ptr<YacProposalStorage>
      YacVoteStorage::findProposalStorage(const VoteMessage &msg,
                                          uint64_t peers_in_round) {
        auto val = getProposalStorage(msg.hash.proposal_hash);
        if (val != nullptr) {
          return val;
        }
        proposal_storages_.push_back(
            std::make_shared<YacProposalStorage>(
                msg.hash.proposal_hash, peers_in_round));
        return proposal_storages_.at(proposal_storages_.size() - 1);
      }

    } // namespace yac
  } // namespace consensus
} // namespace iroha
