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

#ifndef IROHA_CONSENSUS_STATUS_PROCESSOR_HPP
#define IROHA_CONSENSUS_STATUS_PROCESSOR_HPP

#include "validation/stateful_validator_common.hpp"

#include <memory>

namespace iroha {
  namespace synchronizer {
    struct SynchronizationEvent;
  }  // namespace synchronizer
}  // namespace iroha

namespace iroha {
  namespace torii {
    /**
     * ConsensusStatusProcessor is interface which reflects statuses consensus
     * statuses and initiate processing of transactions in it
     */
    class ConsensusStatusProcessor {
     public:
      /**
       * Handler for new verified proposals
       */
      virtual void handleOnVerifiedProposal(
          std::shared_ptr<iroha::validation::VerifiedProposalAndErrors>
              validation_outcome) = 0;

      /**
       * Handler for new commits
       */
      virtual void handleOnCommit(
          const iroha::synchronizer::SynchronizationEvent &) = 0;

      virtual ~ConsensusStatusProcessor() = default;
    };
  }  // namespace torii
}  // namespace iroha
#endif  // IROHA_CONSENSUS_STATUS_PROCESSOR_HPP
