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

#include <unordered_map>
#include <tuple>
#include <memory>

#include "consensus/yac/yac_network_interface.hpp"
#include "consensus/yac/yac_crypto_provider.hpp"
#include "consensus/yac/yac_gate.hpp"
#include "network/consensus_gate.hpp"
#include "model/block.hpp"
#include "consensus/yac/timer.hpp"
#include "consensus/yac/yac_network_interface.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {
      class Yac : public HashGate, public YacNetworkNotifications,
                  std::enable_shared_from_this<Yac> {
       public:

        Yac(std::shared_ptr<YacNetwork> network,
            std::shared_ptr<YacCryptoProvider> crypto,
            std::shared_ptr<Timer> timer,
            uint64_t delay);

        // ------|Hash gate|------

        virtual void vote(YacHash hash, ClusterOrdering order);

        virtual rxcpp::observable <YacHash> on_commit();

        // ------|Network notifications|------

        virtual void on_commit(model::Peer from, CommitMessage commit);

        virtual void on_reject(model::Peer from, RejectMessage reject);

        virtual void on_vote(model::Peer from, VoteMessage vote);

       private:
        // ------|Private interface|------
        void votingStep();
        void applyCommit(model::Peer from, CommitMessage commit);
        void applyReject(model::Peer from, RejectMessage commit);
        void applyVote(model::Peer from, VoteMessage commit);

        // ------|Fields|------
        rxcpp::subjects::subject <YacHash> notifier_;
        std::shared_ptr<YacCryptoProvider> crypto_;
        std::shared_ptr<Timer> timer_;
        std::shared_ptr<YacNetwork> network_;
        std::unordered_map<YacHash,
                           std::tuple<model::Peer, VoteMessage>> votes_;

        // ------|One round|------
        ClusterOrdering cluster_order_;
        YacHash current_hash_;

        // ------|Constants|------
        const uint64_t delay_;
      };
    } // namespace yac
  } // namespace consensus
} // iroha

#endif //IROHA_YAC_HPP
