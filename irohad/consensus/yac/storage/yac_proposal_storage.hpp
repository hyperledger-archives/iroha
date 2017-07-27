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

#ifndef IROHA_YAC_PROPOSAL_STORAGE_HPP
#define IROHA_YAC_PROPOSAL_STORAGE_HPP

#include <vector>
#include "consensus/yac/messages.hpp"
#include "consensus/yac/storage/yac_common.hpp"
#include "consensus/yac/storage/storage_result.hpp"
#include "consensus/yac/storage/yac_block_vote_storage.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      /**
       * Class for storing votes related to given proposal hash
       * and gain information about commits/rejects in system
       */
      class YacProposalStorage {
       public:
        YacProposalStorage(ProposalHash hash, uint64_t peers_in_round);

        /**
         * Try to insert vote to block
         * @param msg - vote
         * @return result, that contains actual state of storage
         */
        StorageResult insert(VoteMessage msg);

        /**
         * @return current stored proposal hash
         */
        ProposalHash getProposalHash();

        /**
         * Provide proof of committing proposal (also block)
         */
        nonstd::optional<CommitMessage> getCommitState();

        /**
         * Provide proof of rejecting proposal
         */
        nonstd::optional<RejectMessage> getRejectState();

       private:
        // --------| private api |--------

        /**
         * Find block index with provided parameters,
         * if those store absent - create new
         * @param proposal_hash - hash of proposal
         * @param block_hash - hash of block
         * @return index of storage
         */
        uint64_t findStore(ProposalHash proposal_hash, BlockHash block_hash);

        /**
         * flat map of all votes stored in this proposal storage
         * @return all votes with currect proposal hash
         */
        std::vector<VoteMessage> aggregateAll();

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
    } // namespace yac
  } // namespace consensus
} // namespace iroha
#endif //IROHA_YAC_PROPOSAL_STORAGE_HPP
