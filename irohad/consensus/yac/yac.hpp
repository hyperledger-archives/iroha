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

#include "consensus/yac/yac_network_interface.hpp"
#include "consensus/yac/yac_gate.hpp"
#include "network/consensus_gate.hpp"
#include "model/block.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {
      class Yac : public HashGate, public YacNetworkNotifications {
       public:

        // ------|ConsensusGate|------

        virtual void vote(YacHash hash) =0;

        virtual rxcpp::observable<YacHash> on_commit() =0;

        // ------|Network Notifications|------

        void on_commit(model::Peer from, CommitMessage commit) =0;

        void on_reject(model::Peer from, RejectMessage reject) =0;

        void on_vote(model::Peer from, VoteMessage vote) =0;

       private:
        rxcpp::subjects::subject<YacHash> notifier_;
      };
    } // namespace yac
  } // namespace consensus
} // iroha

#endif //IROHA_YAC_HPP
