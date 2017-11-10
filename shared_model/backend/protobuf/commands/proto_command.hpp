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

#ifndef IROHA_SHARED_MODEL_PROTO_COMMAND_HPP
#define IROHA_SHARED_MODEL_PROTO_COMMAND_HPP

#include "backend/protobuf/commands/proto_add_asset_quantity.hpp"
#include "commands.pb.h"
#include "interfaces/commands/command.hpp"
#include "utils/lazy_initializer.hpp"
#include "utils/variant_deserializer.hpp"

template <typename... T>
auto load(const iroha::protocol::Command &ar) {
  shared_model::interface::Command::CommandVariantType result;
  int which = ar.command_case() - 1;
  shared_model::detail::variant_impl<T...>::load(ar, which, result);
  return result;
}

namespace shared_model {
  namespace proto {
    class Command final : public interface::Command {
     private:
      /// polymorphic wrapper type shortcut
      template <typename Value>
      using w = detail::PolymorphicWrapper<Value>;
      // private API

      /// lazy variant shortcut
      using LazyVariantType = detail::LazyInitializer<CommandVariantType>;

     public:
      /// type of proto variant
      using ProtoCommandVariantType = boost::variant<w<AddAssetQuantity>>;

      /// list of types in proto variant
      using ProtoCommandListType = ProtoCommandVariantType::types;

      /**
       * Create command container from proto command
       * @param command - proto instance
       */
      explicit Command(const iroha::protocol::Command &command)
          : command_(command),
            lazy_variant_(detail::makeLazyInitializer([this] {
              return CommandVariantType(
                  load<ProtoCommandListType>(this->command_));
            })) {}

      const CommandVariantType &get() const override {
        return lazy_variant_.get();
      }

      ModelType *copy() const override { return new Command(command_); }

     private:
      // ------------------------------| fields |-------------------------------

      // proto
      const iroha::protocol::Command command_;

      // lazy
      LazyVariantType lazy_variant_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_SHARED_MODEL_PROTO_COMMAND_HPP
