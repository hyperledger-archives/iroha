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

#include "ametsuchi/impl/peer_query_wsv.hpp"

#include "ametsuchi/wsv_query.hpp"
#include "backend/protobuf/common_objects/peer.hpp"
#include "model/peer.hpp"
#include "primitive.pb.h"

namespace iroha {
  namespace ametsuchi {

    PeerQueryWsv::PeerQueryWsv(std::shared_ptr<WsvQuery> wsv)
        : wsv_(std::move(wsv)) {}

    nonstd::optional<std::vector<wPeer>> PeerQueryWsv::getLedgerPeers() {
      const auto &tmp = wsv_->getPeers();
      if (not tmp) {
        return nonstd::nullopt;
      }
      std::vector<wPeer> peers = [&tmp] {
        std::vector<wPeer> result;
        for (const auto &item : *tmp) {
          iroha::protocol::Peer peer;
          peer.set_address(item.address);
          peer.set_peer_key(item.pubkey.to_string());
          auto curr =
              shared_model::detail::makePolymorphic<shared_model::proto::Peer>(
                  shared_model::proto::Peer(std::move(peer)));
          result.emplace_back(curr);
        }
        return result;
      }();
      return peers;
    }

  }  // namespace ametsuchi
}  // namespace iroha
