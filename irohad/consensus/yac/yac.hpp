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

#ifndef IROHA_YAC_HPP
#define IROHA_YAC_HPP

#include "network/consensus_gate.hpp"
#include "consensus/yac/yac_network_interface.hpp"
#include "model/block.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {
      class Yac : public network::ConsensusGate, public YacNetwork {
       public:

        // ------|ConsensusGate|------
        void vote(model::Block) override;

        rxcpp::observable<model::Block> on_commit() override;

        // ------|ConsensusGate|------

        virtual void on_commit(model::Peer from, CommitMessage commit) = 0;

        virtual void on_reject(model::Peer from, RejectMessage reject) = 0;

        virtual void on_vote(model::Peer from, VoteMessage vote) = 0;

        // ------|Send|------

        virtual void send_commit(model::Peer to, CommitMessage commit) = 0;

        virtual void send_reject(model::Peer to, RejectMessage reject) = 0;

        virtual void send_vote(model::Peer to, VoteMessage vote) = 0;

       private:
        rxcpp::subjects::subject<model::Block> notifier_;
      };
    } // namespace yac
  } // namespace consensus
} // iroha

#endif //IROHA_YAC_HPP
