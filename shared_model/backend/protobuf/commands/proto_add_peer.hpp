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
      using RefAddPeer =
          detail::ReferenceHolder<iroha::protocol::Command,
                                  const iroha::protocol::AddPeer &>;

     public:
      explicit AddPeer(const iroha::protocol::Command &command)
          : AddPeer(RefAddPeer(command,
                               detail::makeReferenceGetter(
                                   &iroha::protocol::Command::add_peer))) {}

      explicit AddPeer(iroha::protocol::Command &&command)
          : AddPeer(RefAddPeer(std::move(command),
                               detail::makeReferenceGetter(
                                   &iroha::protocol::Command::add_peer))) {}

      const AddressType &peerAddress() const override {
        return add_peer_->address();
      }

      const interface::types::PubkeyType &peerKey() const override {
        return *pubkey_;
      }

      interface::AddPeer *copy() const override {
        iroha::protocol::Command command;
        *command.mutable_add_peer() = *add_peer_;
        return new AddPeer(std::move(command));
      }

     private:
      explicit AddPeer(RefAddPeer &&ref)
          : add_peer_(std::move(ref)), pubkey_([this] {
              return interface::types::PubkeyType(add_peer_->peer_key());
            }) {}

      // proto
      RefAddPeer add_peer_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

      // lazy
      Lazy<interface::types::PubkeyType> pubkey_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_ADD_PEER_HPP
