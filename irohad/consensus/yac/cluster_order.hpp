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

#ifndef IROHA_CLUSTER_ORDER_HPP
#define IROHA_CLUSTER_ORDER_HPP

#include "model/peer.hpp"
#include <vector>

namespace iroha {
  namespace consensus {
    namespace yac {

      /**
       * Class provide ordering on cluster for current round
       */
      class ClusterOrdering {
       public:

        ClusterOrdering();

        ClusterOrdering(std::vector<model::Peer> order);

        /**
         * Provide current leader peer
         */
        model::Peer currentLeader();

        /**
         * Switch to next peer as leader
         * @return this
         */
        ClusterOrdering &switchToNext();

        /**
         * Validate set is on of first 2f+1 elements from 3f+1.
         * @return true if leader is on of 2f+1 elements
         */
        bool leaderInValidateSet();

        /**
         * Check that parameter greater of equal(geq) that 2f+1
         * @param val - checked value
         * @return true if geq supermajority
         */
        bool haveSupermajority(uint64_t val);

        virtual ~ClusterOrdering() = default;
       private:
        std::vector<model::Peer> order_;
        uint32_t index_ = 0;
      };
    } // namespace yac
  } // namespace consensus
} // iroha
#endif //IROHA_CLUSTER_ORDER_HPP
