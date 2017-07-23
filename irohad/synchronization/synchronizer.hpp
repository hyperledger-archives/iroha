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
#ifndef IROHA_SYNCHRONIZER_HPP
#define IROHA_SYNCHRONIZER_HPP

#include <rxcpp/rx-observable.hpp>
#include "model/block.hpp"

namespace iroha {
  namespace synchronization {

    class Synchronizer {
     public:
      // Commit is a bunch of Blocks
      using Commit = rxcpp::observable<model::Block>;

      /**
       * Process commit message with Chain Validation and write to Ametsuchi
       * @param commit_message
       */
      virtual void process_commit(iroha::model::Block commit_message) = 0;

      /**
       * Event on synchronization of Blocks
       * @return Commit messages
       */
      virtual rxcpp::observable<Commit> on_commit_chain() = 0;
    };
  }
}
#endif  // IROHA_SYNCHRONIZER_HPP
