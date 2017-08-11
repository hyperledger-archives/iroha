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

#include "consensus/yac/storage/yac_block_storage.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      YacBlockStorage::YacBlockStorage(YacHash hash, uint64_t peers_in_round)
          : hash_(std::move(hash)), peers_in_round_(peers_in_round) {
      }

      nonstd::optional<Answer> YacBlockStorage::insert(VoteMessage msg) {
        auto inserted = tryInsert(msg);
        if (inserted) {
          // update state branch
          auto prev_state = current_state_.state;

          switch (prev_state) {

            case not_committed:
              current_state_.state = updateSupermajorityState();
              if (current_state_.state == CommitState::committed) {
                current_state_.answer.commit = CommitMessage(votes_);
              }
              break;

            case committed:current_state_.state = committed_before;
              current_state_.answer.commit = CommitMessage(votes_);
              break;

            case committed_before:
              current_state_.answer.commit = CommitMessage(votes_);
              break;
          }
        }
        return getState();
      }

      nonstd::optional<Answer>
      YacBlockStorage::insert(std::vector<CommitMessage>) {
        if (checkCommitScheme(commit)) {
          auto initial_state = getState().state;
          for (auto &&vote : commit.votes) {
            current_state_ = insert(vote);
          }
          if (initial_state == CommitState::not_committed and
              current_state_.state == CommitState::committed_before) {
            current_state_.state = CommitState::committed;
          }
          return getState();
        }
        return StorageResult();
      }

      Answer YacBlockStorage::getState() {
        return current_state_;
      }

      std::vector<VoteMessage> YacBlockStorage::getVotes() {
        return votes_;
      }

      YacHash YacBlockStorage::getStorageHash() {
        // todo implement
      }

      // --------| private api |--------

      bool YacBlockStorage::unique_vote(VoteMessage &msg) {
        for (auto &&vote: votes_) {
          if (vote == msg) {
            return false;
          }
        }
        return true;
      }

    } // namespace yac
  } // namespace consensus
} // namespace iroha
