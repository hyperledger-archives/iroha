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

#ifndef IROHA_YAC_PEER_ORDERER_HPP
#define IROHA_YAC_PEER_ORDERER_HPP

#include <nonstd/optional.hpp>
#include "consensus/yac/cluster_order.hpp"
#include "consensus/yac/yac_hash_provider.hpp"

namespace iroha {
  namespace consensus {
    namespace yac {

      class YacPeerOrderer {
       public:
        virtual nonstd::optional<ClusterOrdering> getInitialOrdering() = 0;

        virtual nonstd::optional<ClusterOrdering> getOrdering(YacHash hash) = 0;

        virtual ~YacPeerOrderer() = default;
      };

    }  // namespace yac
  }    // namespace consensus
}  // namespace iroha

#endif  // IROHA_YAC_PEER_ORDERER_HPP
