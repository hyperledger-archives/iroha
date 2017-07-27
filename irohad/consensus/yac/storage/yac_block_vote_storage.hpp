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

#ifndef IROHA_YAC_BLOCK_VOTE_STORAGE_HPP
#define IROHA_YAC_BLOCK_VOTE_STORAGE_HPP

#include "consensus/yac/storage/storage_result.hpp"
#include "consensus/yac/storage/yac_common.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {
      /**
       * Class provide storage of votes for one block
       */
      class YacBlockVoteStorage {
       public:

        YacBlockVoteStorage(ProposalHash proposal_hash,
                            BlockHash block_hash,
                            uint64_t peers_in_round)
            : proposal_hash_(proposal_hash),
              block_hash_(block_hash),
              peers_in_round_(peers_in_round) {
          committed_state_ = nonstd::nullopt;
        }

        /**
         * Try to insert vote to storage
         * @param msg - vote for insertion
         * @return actual state of storage
         */
        StorageResult insert(VoteMessage msg) {

          // already have supermajority
          if (committed_state_ != nonstd::nullopt) {
            return StorageResult(committed_state_, nonstd::nullopt, false);
          }

          auto inserted = false;

          // check and insert
          if (msg.hash.proposal_hash == proposal_hash_ and
              msg.hash.block_hash == block_hash_) {
            inserted = unique_vote(msg);
            if (inserted) {
              votes_.push_back(msg);
            }
          }
          // check supermajority
          auto supermajority = hasSupermajority(votes_.size(), peers_in_round_);
          if (supermajority) {
            committed_state_ = CommitMessage(votes_);
          }
          return StorageResult(committed_state_, nonstd::nullopt, inserted);
        }

        nonstd::optional<CommitMessage> getState() {
          return committed_state_;
        };

        std::vector<VoteMessage> getVotes() {
          return votes_;
        }

        ProposalHash getProposalHash() {
          return proposal_hash_;
        }

        BlockHash getBlockHash() {
          return block_hash_;
        }

       private:
        // --------| private fields |--------

        /**
         * Verify uniqueness of vote in storage
         * @param msg - vote for verification
         * @return true if vote doesn't appear in storage
         */
        bool unique_vote(VoteMessage &msg) {
          for (auto &&vote: votes_) {
            if (vote == msg) {
              return false;
            }
          }
          return true;
        }

        /**
         * Unique hash of proposal for all storage votes
         */
        ProposalHash proposal_hash_;

        /**
         * Unique hash of all votes
         */
        BlockHash block_hash_;

        /**
         * Votes for block hash
         */
        std::vector<VoteMessage> votes_;

        /**
         * Commit state
         */
        nonstd::optional<CommitMessage> committed_state_;

        /**
         * Number of peers in current round
         */
        uint64_t peers_in_round_;
      };

    } // namespace yac
  } // namespace consensus
} // namespace iroha
#endif //IROHA_YAC_BLOCK_VOTE_STORAGE_HPP
