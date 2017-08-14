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

#include <utility>

#include "consensus/yac/storage/yac_common.hpp"
#include "consensus/yac/messages.hpp"
#include <nonstd/optional.hpp>

namespace iroha {
  namespace consensus {
    namespace yac {

      /**
       * Contains proof of supermajority for all purposes;
       * Guarantee that at least one optional will be empty
       */
      struct Answer {
        Answer(CommitMessage cmt){
          commit = std::move(cmt);
        }

        Answer(RejectMessage rjt){
          reject = std::move(rjt);
        }

        Answer() = delete;

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

    } // namespace yac
  } // namespace consensus
} // namespace iroha
#endif //IROHA_STORAGE_RESULT_HPP
