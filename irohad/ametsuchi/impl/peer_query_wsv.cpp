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

#include "builders/protobuf/common_objects/proto_peer_builder.hpp"
#include "ametsuchi/impl/peer_query_wsv.hpp"

#include "ametsuchi/wsv_query.hpp"

using wPeer = std::shared_ptr<shared_model::interface::Peer>;

namespace iroha {
  namespace ametsuchi {

    PeerQueryWsv::PeerQueryWsv(std::shared_ptr<WsvQuery> wsv)
        : wsv_(std::move(wsv)) {}

    boost::optional<std::vector<wPeer>> PeerQueryWsv::getLedgerPeers() {
      const auto &tmp = wsv_->getPeers();
      if (not tmp) {
        return boost::none;
      }
      std::vector<wPeer> peers = [&tmp] {
        std::vector<wPeer> result;
        for (const auto &item : *tmp) {
          shared_model::proto::PeerBuilder builder;

          auto key = shared_model::crypto::PublicKey(item.pubkey.to_string());
          auto peer = builder.address(item.address).pubkey(key).build();

          auto curr = std::make_shared<shared_model::proto::Peer>(peer.getTransport());
          result.emplace_back(curr);
        }
        return result;
      }();
      return peers;
    }

  }  // namespace ametsuchi
}  // namespace iroha
