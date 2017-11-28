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
     public:
      template <typename CommandType>
      explicit AddPeer(CommandType &&command)
          : command_(std::forward<CommandType>(command)),
            add_peer_([this] { return command_->add_peer(); }),
            pubkey_([this] {
              return interface::types::PubkeyType(add_peer_->peer_key());
            }) {}

      AddPeer(const AddPeer &o)
          : AddPeer(*o.command_) {}

      AddPeer(AddPeer &&o) noexcept
          : AddPeer(std::move(o.command_.variant())) {}

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
      // proto
      detail::ReferenceHolder<iroha::protocol::Command> command_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

      // lazy
      const Lazy<const iroha::protocol::AddPeer &> add_peer_;
      Lazy<interface::types::PubkeyType> pubkey_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_ADD_PEER_HPP
