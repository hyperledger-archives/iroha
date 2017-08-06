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

      StorageResult YacVoteStorage::storeVote(VoteMessage msg,
                                              uint64_t peers_in_round) {
        return proposal_storages_
            .at(findProposalStorage(msg, peers_in_round))
            .insert(msg);
      }

      StorageResult YacVoteStorage::applyCommit(CommitMessage commit,
                                                uint64_t peers_in_round) {
        if (commit.votes.empty()) return StorageResult();

        auto index = findProposalStorage(commit.votes.at(0), peers_in_round);
        return proposal_storages_.at(index).applyCommit(commit, peers_in_round);
      };

      StorageResult YacVoteStorage::applyReject(RejectMessage reject,
                                                uint64_t peers_in_round) {
        if (reject.votes.empty()) return StorageResult();

        auto index = findProposalStorage(reject.votes.at(0), peers_in_round);
        return proposal_storages_.at(index).applyReject(reject, peers_in_round);
      };

      nonstd::optional<StorageResult> YacVoteStorage::findProposal(
          YacHash hash) {
        for (const auto &proposal_storage : proposal_storages_) {
          if (proposal_storage.getProposalHash() == hash.proposal_hash) {
            return proposal_storage.getState();
          }
        }
        return nonstd::nullopt;
      }

      // --------| private api |--------

      uint64_t YacVoteStorage::findProposalStorage(const VoteMessage &msg,
                                                   uint64_t peers_in_round) {
        for (uint64_t i = 0; i < proposal_storages_.size(); ++i) {
          if (proposal_storages_.at(i).getProposalHash()
              == msg.hash.proposal_hash) {
            return i;
          }
        }
        proposal_storages_.emplace_back(msg.hash.proposal_hash, peers_in_round);
        return proposal_storages_.size() - 1;
      }

    } // namespace yac
  } // namespace consensus
} // namespace iroha
