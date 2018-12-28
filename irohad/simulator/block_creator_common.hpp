/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_BLOCK_CREATOR_COMMON_HPP
#define IROHA_BLOCK_CREATOR_COMMON_HPP

#include <memory>

#include <boost/optional.hpp>
#include "consensus/round.hpp"
#include "consensus/ledger_event.hpp"

namespace shared_model {
  namespace interface {
    class Block;
    class Proposal;
  }  // namespace interface
}  // namespace shared_model

namespace iroha {
  namespace simulator {

    struct RoundData {
      std::shared_ptr<shared_model::interface::Proposal> proposal;
      std::shared_ptr<shared_model::interface::Block> block;
    };

    /**
     * Event, which is emitted by block creator, when it receives and processes
     * a verified proposal
     */
    struct BlockCreatorEvent {
      std::shared_ptr<PeerList> ledger_peers;
      boost::optional<RoundData> round_data;
      consensus::Round round;
    };

    std::shared_ptr<shared_model::interface::Block> getBlockUnsafe(
        const BlockCreatorEvent &event);

  }  // namespace simulator
}  // namespace iroha

#endif  // IROHA_BLOCK_CREATOR_COMMON_HPP
