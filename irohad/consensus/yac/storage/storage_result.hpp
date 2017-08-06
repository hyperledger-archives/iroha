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

#ifndef IROHA_STORAGE_RESULT_HPP
#define IROHA_STORAGE_RESULT_HPP

#include "consensus/yac/messages.hpp"
#include <nonstd/optional.hpp>

namespace iroha {
  namespace consensus {
    namespace yac {

      enum CommitState {
        /**
         * Means that new state after applying still as before - not commit;
         * state changes: not_committed => not_committed
         */
            not_committed,

        /**
         * State means that change state to committed on current insertion;
         * state changes: not_committed => committed
         */
            committed,

        /**
         * State means that state still as before - committed;
         * state changes: committed => committed_before OR
         *         committed_before => committed_before
         */
            committed_before
      };

      /**
       * Contains proof of supermajority for all purposes
       */
      struct Answer {
        Answer() {
          commit = nonstd::nullopt;
          reject = nonstd::nullopt;
        }
        /**
         * Result contains commit if it available
         */
        nonstd::optional<CommitMessage> commit;

        /**
         * Result contains reject if it available
         */
        nonstd::optional<RejectMessage> reject;

        bool operator==(const Answer &rhs) const;
      };

      /**
       * Struct represents result of working storage.
       * Guarantee that at least one optional will be empty
       */
      struct StorageResult {

        StorageResult() {
          state = CommitState::not_committed;
        };

        StorageResult(Answer provided_answer,
                      CommitState provided_state);

        bool operator==(const StorageResult &rhs) const;

        /**
         * Answer with proof of state
         */
        Answer answer;

        /**
         * Current state computed after application
         */
        CommitState state;

      };

    } // namespace yac
  } // namespace consensus
} // namespace iroha
#endif //IROHA_STORAGE_RESULT_HPP
