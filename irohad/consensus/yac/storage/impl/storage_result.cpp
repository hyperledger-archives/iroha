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

#include "consensus/yac/storage/storage_result.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {
      StorageResult::StorageResult(nonstd::optional <CommitMessage> commit_result,
      nonstd::optional <RejectMessage> reject_result,
      bool inserted_result)
      : commit(std::move(commit_result)),
      reject(std::move(reject_result)),
      vote_inserted(inserted_result) {};

      bool StorageResult::operator==(const StorageResult &rhs) const {
        return commit == rhs.commit and
            reject == rhs.reject and
            vote_inserted == rhs.vote_inserted;
      };

    } // namespace yac
  } // namespace consensus
} // namespace iroha