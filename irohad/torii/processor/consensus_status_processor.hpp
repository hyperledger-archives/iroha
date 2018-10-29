/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
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
     * ConsensusStatusProcessor is interface which is responsible for
     * appropriate statuses from consensus
     */
    class ConsensusStatusProcessor {
     public:
      virtual void handleOnVerifiedProposal(
          std::shared_ptr<iroha::validation::VerifiedProposalAndErrors>
              validation_outcome) = 0;

      virtual void handleOnCommit(
          const iroha::synchronizer::SynchronizationEvent &) = 0;

      virtual ~ConsensusStatusProcessor() = default;
    };
  }  // namespace torii
}  // namespace iroha
#endif  // IROHA_CONSENSUS_STATUS_PROCESSOR_HPP
