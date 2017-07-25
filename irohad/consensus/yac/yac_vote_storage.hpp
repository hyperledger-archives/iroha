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

#ifndef IROHA_YAC_VOTE_STORAGE_HPP
#define IROHA_YAC_VOTE_STORAGE_HPP

#include <unordered_map>
#include <vector>
#include <memory>
#include <nonstd/optional.hpp>

#include "consensus/yac/messages.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      using ProposalHash = decltype(YacHash::proposal_hash);
      using BlockHash = decltype(YacHash::block_hash);

      /**
       * Check that current number >= supermajority.
       * @param current - current number for validation
       * @param all - whole number (N)
       * @return true if belong supermajority
       */
      inline bool hasSupermajority(uint64_t current, uint64_t all) {
        if (current > all)
          return false;
        auto f = (all - 1) / 3;
        return current >= 2 * f + 1;
      }

      /**
       * Struct represents result of working storage.
       * Guarantee that at least one optional will be empty
       */
      struct StorageResult {
        StorageResult(nonstd::optional<CommitMessage> commit_result,
                      nonstd::optional<RejectMessage> reject_result,
                      bool inserted_result)
            : commit(std::move(commit_result)),
              reject(std::move(reject_result)),
              vote_inserted(inserted_result) {};

        bool operator=(const StorageResult &rhs) const {
          return commit == rhs.commit and
              reject == rhs.reject and
              vote_inserted == rhs.vote_inserted;
        }

        /**
         * Result contains commit if it available
         */
        nonstd::optional<CommitMessage> commit;

        /**
         * Result contains reject if it available
         */
        nonstd::optional<RejectMessage> reject;

        /**
         * Is vote was inserted in storage.
         * False, means that this vote suspicious
         */
        bool vote_inserted;
      };

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

      class YacProposalStorage {
       public:
        YacProposalStorage(ProposalHash hash, uint64_t peers_in_round)
            : hash_(hash), peers_in_round_(peers_in_round) {
          commit_state_ = nonstd::nullopt;
          reject_state_ = nonstd::nullopt;
        }

        /**
         * Try to insert vote to block
         * @param msg - vote
         * @return result, that contains actual state of storage
         */
        StorageResult insert(VoteMessage msg) {
          // already defined stated
          if (getHash() != msg.hash.proposal_hash or
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

        ProposalHash getHash() {
          return hash_;
        }

        /**
         * Provide proof of committing proposal (also block)
         */
        nonstd::optional<CommitMessage> getCommitState() {
          return commit_state_;
        };

        /**
         * Provide proof of rejecting proposal
         */
        nonstd::optional<RejectMessage> getRejectState() {
          return reject_state_;
        };

       private:
        // --------| private api |--------
        uint64_t findStore(ProposalHash proposal_hash,
                           BlockHash block_hash) {

          // find exist
          for (auto i = 0; i < block_votes_.size(); ++i) {
            if (block_votes_.at(i).getProposalHash() == proposal_hash and
                block_votes_.at(i).getBlockHash() == block_hash) {
              return i;
            }
          }
          // insert and return new
          YacBlockVoteStorage
              new_container(proposal_hash, block_hash, peers_in_round_);
          block_votes_.push_back(new_container);
          return block_votes_.size() - 1;
        }

        std::vector<VoteMessage> aggregateAll() {
          std::vector<VoteMessage> all_votes;
          for (auto vote_storage: block_votes_) {
            auto votes = vote_storage.getVotes();
            all_votes.insert(all_votes.end(),
                             votes.begin(), votes.end());
          }
          return all_votes;
        }

        /**
         * Hash of proposal
         */
        ProposalHash hash_;

        /**
         * Provide proof of rejecting proposal
         */
        nonstd::optional<RejectMessage> reject_state_;

        /**
         * Provide proof of committing proposal (also block)
         */
        nonstd::optional<CommitMessage> commit_state_;

        /**
         * Provide number of peers participated in current round
         */
        uint64_t peers_in_round_;

        /**
         * Vector of blocks based on this proposal
         */
        std::vector<YacBlockVoteStorage> block_votes_;
      };

      /**
       * Class provide storage for votes and useful methods for it.
       */
      class YacVoteStorage {
       public:

        /**
         * Insert vote
         * @param msg
         * @param peers_in_round - number of peers participated in round
         * @return result with
         */
        StorageResult storeVote(VoteMessage msg, uint64_t peers_in_round) {

          // TODO verify uniqueness of peer

          // try insert in available proposal storage
          for (auto &&proposal: proposals) {
            auto hash = proposal.getHash();
            if (msg.hash.proposal_hash == hash) {
              auto result = proposal.insert(msg);
              return result;
            }
          }

          // can't find proposal, create new
          YacProposalStorage storage(msg.hash.proposal_hash, peers_in_round);
          auto result = storage.insert(msg);
          proposals.push_back(storage);
          return result;
        }

       private:

        // --------| private fields |--------
        std::vector<YacProposalStorage> proposals;
      };

    } // namespace yac
  } // namespace consensus
} // namespace iroha
#endif //IROHA_YAC_VOTE_STORAGE_HPP