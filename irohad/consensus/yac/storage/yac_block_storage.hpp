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

#include <vector>
#include <nonstd/optional.hpp>
#include "consensus/yac/storage/storage_result.hpp"
#include "consensus/yac/storage/yac_common.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      /**
       * Class provide storage of votes for one block.
       */
      class YacBlockStorage {
       public:

        YacBlockStorage(YacHash hash, uint64_t peers_in_round);

        /**
         * Try to insert vote to storage
         * @param msg - vote for insertion
         * @return actual state of storage,
         * nullopt when storage doesn't has supermajority
         */
        nonstd::optional<Answer> insert(VoteMessage msg);

        /**
         * Insert vector of votes to current storage
         * @param votes - bunch of votes for insertion
         * @return state of storage after insertion last vote,
         * nullopt when storage doesn't has supermajority
         */
        nonstd::optional<Answer> insert(std::vector<VoteMessage> votes);

        /**
         * @return votes attached to storage
         */
        auto getVotes();

        /**
         * @return current block store state
         */
        nonstd::optional<Answer> getState();

        /**
         * Provide hash attached to this storage
         */
        YacHash getStorageHash();

       private:
        // --------| private api |--------

        /**
         * Verify uniqueness of vote in storage
         * @param msg - vote for verification
         * @return true if vote doesn't appear in storage
         */
        bool uniqueVote(VoteMessage &vote);

        /**
         * Verify that vote has same proposal and
         * blocks hashes with storage
         * @return true, if validation passed
         */
        bool validScheme(VoteMessage &vote);

        // --------| fields |--------

        /**
         * Common hash of all votes in storage
         */
        YacHash hash_;

        /**
         * All votes stored in block store
         */
        std::vector<VoteMessage> votes_;

        /**
         * Number of peers in current round
         */
        uint64_t peers_in_round_;
      };

    } // namespace yac
  } // namespace consensus
} // namespace iroha
#endif //IROHA_YAC_BLOCK_VOTE_STORAGE_HPP
