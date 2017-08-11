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
          : hash_(hash),
            peers_in_round_(peers_in_round) {
      }

      nonstd::optional<Answer> YacProposalStorage::insert(VoteMessage msg) {
        // update state branch
        if (shouldInsert(msg)) {
          // insert to block store
          auto index = findStore(msg.hash.proposal_hash,
                                 msg.hash.block_hash);
          auto block_state = block_votes_.at(index).insert(msg);

          // check block storage state
          auto state_of_block = block_state.state;

          // this switch use assumption that
          // if supermajority on some block achieved <==>
          // there is no available for reject, and vice versa.
          switch (state_of_block) {
            case not_committed:
              if (hasRejectProof()) {
                auto current_state = current_state_.state;
                switch (current_state) {
                  case not_committed:
                    // update to committed
                    current_state_.state = CommitState::committed;
                    current_state_.answer.reject =
                        RejectMessage(aggregateAll());
                    break;
                  case committed:
                    // update to committed before
                    current_state_.state = CommitState::committed_before;
                    current_state_.answer.reject =
                        RejectMessage(aggregateAll());
                    break;
                  case committed_before:
                    // nothing to do
                    break;
                }
              }
              break;
            case committed:current_state_ = block_state;
              break;
            case committed_before:current_state_ = block_state;
              break;
          }
        }
        return getState();
      };

      Answer YacProposalStorage::insert(std::vector<VoteMessage> messages) {
        if (commit.votes.empty()) return StorageResult();
        auto index = findStore(commit.votes.at(0).hash.proposal_hash,
                               commit.votes.at(0).hash.block_hash);
        auto result = block_votes_.at(index).insert(commit);
        if (result.state == CommitState::committed) {
          current_state_ = result;
        }
        return result;
      }

      Answer YacProposalStorage::getState() const {
        return current_state_;
      };

      // --------| private api |--------

      bool YacProposalStorage::shouldInsert(const VoteMessage &msg) {
        return checkProposalHash(msg.hash.proposal_hash) and
            checkPeerUniqueness(msg);
      };

      bool YacProposalStorage::checkProposalHash(ProposalHash vote_hash) {
        return vote_hash == hash_;
      };

      bool YacProposalStorage::checkPeerUniqueness(const VoteMessage &msg) {
        // todo implement method: checking based on public keys
        return true;
      };

      bool YacProposalStorage::hasRejectProof() {
        // todo implement
        return false;
      };

      bool YacProposalStorage::checkRejectScheme(const RejectMessage &reject) {
        auto votes = reject.votes;
        if (votes.size() == 0) return false;
        auto common_proposal = votes.at(0).hash.proposal_hash;
        for (auto &&vote:votes) {
          if (common_proposal != vote.hash.proposal_hash) {
            return false;
          }
        }
        return true;
      };

      uint64_t YacProposalStorage::findStore(ProposalHash proposal_hash,
                                             BlockHash block_hash) {

        // find exist
        for (uint32_t i = 0; i < block_votes_.size(); ++i) {
          if (block_votes_.at(i).getProposalHash() == proposal_hash and
              block_votes_.at(i).getBlockHash() == block_hash) {
            return i;
          }
        }
        // insert and return new
        YacBlockStorage
            new_container(YacHash(proposal_hash, block_hash), peers_in_round_);
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
