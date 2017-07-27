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

#include "consensus/yac/storage/yac_proposal_storage.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {
      YacProposalStorage::YacProposalStorage(ProposalHash hash,
                                             uint64_t peers_in_round)
          : hash_(hash), peers_in_round_(peers_in_round) {
        commit_state_ = nonstd::nullopt;
        reject_state_ = nonstd::nullopt;
      }

      StorageResult YacProposalStorage::insert(VoteMessage msg) {
        // already defined stated
        if (getProposalHash() != msg.hash.proposal_hash or
            commit_state_ != nonstd::nullopt or
            reject_state_ != nonstd::nullopt) {
          return StorageResult(commit_state_, reject_state_, false);
        }

        // try to fill
        auto inserted = false;
        auto index = findStore(msg.hash.proposal_hash, msg.hash.block_hash);
        auto result = block_votes_.at(index).insert(msg);
        inserted = result.vote_inserted;
        if (inserted) {
          if (result.commit != nonstd::nullopt) {
            // commit case
            commit_state_ = result.commit;
            return result;
          } else {
            // check may be reject

            // todo fix reject case
            auto all_votes = aggregateAll();
            if (hasSupermajority(all_votes.size(), peers_in_round_)) {
              reject_state_ = RejectMessage(all_votes);
              result.reject->votes = all_votes;
              return result;
            }
          }
        }
        return result;
      }

      ProposalHash YacProposalStorage::getProposalHash() {
        return hash_;
      }

      nonstd::optional<CommitMessage> YacProposalStorage::getCommitState() {
        return commit_state_;
      }

      nonstd::optional<RejectMessage> YacProposalStorage::getRejectState() {
        return reject_state_;
      };

      // --------| private api |--------

      uint64_t YacProposalStorage::findStore(ProposalHash proposal_hash,
                                             BlockHash block_hash) {

        // find exist
        for (auto i = 0; i < block_votes_.size(); ++i) {
          if (block_votes_.at(i).getProposalHash() == proposal_hash and
              block_votes_.at(i).getBlockHash() == block_hash) {
            return i;
          }
        }
        // insert and return new
        YacBlockStorage
            new_container(proposal_hash, block_hash, peers_in_round_);
        block_votes_.push_back(new_container);
        return block_votes_.size() - 1;
      };

      std::vector<VoteMessage> YacProposalStorage::aggregateAll() {
        std::vector<VoteMessage> all_votes;
        for (auto vote_storage: block_votes_) {
          auto votes = vote_storage.getVotes();
          all_votes.insert(all_votes.end(),
                           votes.begin(), votes.end());
        }
        return all_votes;
      };

    } // namespace yac
  } // namespace consensus
} // namespace iroha