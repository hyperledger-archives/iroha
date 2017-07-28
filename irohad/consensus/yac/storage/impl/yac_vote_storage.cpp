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
        return proposals_
            .at(findProposalStorage(msg, peers_in_round))
            .insert(msg);
      }

      // --------| private api |--------

      uint64_t YacVoteStorage::findProposalStorage(const VoteMessage &msg,
                                                   uint64_t peers_in_round) {
        for (uint64_t i = 0; i < proposals_.size(); ++i) {
          if (proposals_.at(i).getProposalHash() == msg.hash.proposal_hash) {
            return i;
          }
        }
        proposals_.emplace_back(msg.hash.proposal_hash, peers_in_round);
        return proposals_.size() - 1;
      }

    } // namespace yac
  } // namespace consensus
} // namespace iroha