/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_MST_NOTIFICATOR_HPP
#define IROHA_MST_NOTIFICATOR_HPP

#include "multi_sig_transactions/mst_processor.hpp"

namespace iroha {
  /**
   * Interface provides notification handlers for
   */
  class MstNotificator {
   public:
    /**
     * Handler which should be invoked on update of MST state
     * @param state - new data from MST
     */
    virtual void handleOnStateUpdate(
        const MstProcessor::UpdatedStateType &state) = 0;

    /**
     * Handler which should be invoked on expired batches
     */
    virtual void handleOnExpiredBatches(
        const MstProcessor::BatchType &expired_batch) = 0;

    /**
     * Handler which should be invoked on completed batches
     */
    virtual void handleOnCompletedBatches(
        const MstProcessor::BatchType &batch) = 0;

    virtual ~MstNotificator() = default;
  };
}  // namespace iroha

#endif  // IROHA_MST_NOTIFICATOR_HPP
