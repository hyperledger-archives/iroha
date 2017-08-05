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

#ifndef IROHA_YAC_PEER_ORDERER_IMPL_HPP
#define IROHA_YAC_PEER_ORDERER_IMPL_HPP

#include "consensus/yac/yac_peer_orderer.hpp"
#include "ametsuchi/peer_query.hpp"
#include <memory>

namespace iroha {
  namespace consensus {
    namespace yac {
      class YacPeerOrdererImpl : public YacPeerOrderer {
       public:

        explicit YacPeerOrdererImpl(std::shared_ptr<ametsuchi::PeerQuery> wsv);

        nonstd::optional<ClusterOrdering> getInitialOrdering() override ;

        nonstd::optional<ClusterOrdering> getOrdering(YacHash hash) override;

       private:
        std::shared_ptr<ametsuchi::PeerQuery> wsv_;
      };
    }  // namespace yac
  }    // namespace consensus
}  // namespace iroha

#endif //IROHA_YAC_PEER_ORDERER_IMPL_HPP
