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

#include "yac_peer_orderer_impl.hpp"
#include <utility>

namespace iroha {
  namespace consensus {
    namespace yac {
      YacPeerOrdererImpl::YacPeerOrdererImpl(
          std::shared_ptr<ametsuchi::PeerQuery> wsv)
          : wsv_(std::move(wsv)) {
      };

      nonstd::optional<ClusterOrdering>
      YacPeerOrdererImpl::getInitialOrdering() {
        auto peers = wsv_->getLedgerPeers();
        if(not peers.has_value()) return nonstd::nullopt;
        return ClusterOrdering(peers.value());
      };

      nonstd::optional<ClusterOrdering>
      YacPeerOrdererImpl::getOrdering(YacHash hash) {
        // todo make shuffling with proposal_hash seed
        return getInitialOrdering();
      };
    }  // namespace yac
  }    // namespace consensus
}  // namespace iroha
