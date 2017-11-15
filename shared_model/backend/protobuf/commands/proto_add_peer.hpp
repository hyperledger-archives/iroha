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

#ifndef IROHA_PROTO_ADD_PEER_HPP
#define IROHA_PROTO_ADD_PEER_HPP

#include <utility>

#include "commands.pb.h"
#include "interfaces/commands/add_peer.hpp"

namespace shared_model {
  namespace proto {

    class AddPeer final : public interface::AddPeer {
     private:
      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

     public:
      explicit AddPeer(const iroha::protocol::Command &command)
          : AddPeer(command.add_peer()) {
        if (not command.has_add_peer()) {
          throw std::invalid_argument(
              "Object does not contain add_peer");
        }
      }

      const AddressType &peerAddress() const override {
        return add_peer_.address();
      }

      const interface::types::PubkeyType &peerKey() const override {
        return pubkey_.get();
      }

      interface::AddPeer *copy() const override {
        return new AddPeer(add_peer_);
      }

      const HashType &hash() const override { return hash_.get(); }

     private:
      explicit AddPeer(const iroha::protocol::AddPeer &add_peer)
          : add_peer_(add_peer),
            pubkey_([this] {
              return interface::types::PubkeyType(add_peer_.peer_key());
            }),
            hash_([this] {
              // TODO 14/11/2017 kamilsa replace with effective implementation
              return crypto::StubHash();
            }) {}

      const iroha::protocol::AddPeer add_peer_;

      // lazy
      Lazy<interface::types::PubkeyType> pubkey_;
      Lazy<HashType> hash_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_ADD_PEER_HPP
