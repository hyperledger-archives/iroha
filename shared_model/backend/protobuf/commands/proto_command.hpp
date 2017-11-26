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

template <typename... T, typename Archive>
auto load(Archive &&ar) {
  int which = ar.command_case() - 1;
  return shared_model::detail::variant_impl<T...>::
      template load<shared_model::interface::Command::CommandVariantType>(
          std::forward<Archive>(ar), which);
}

namespace shared_model {
  namespace proto {
    class Command final : public interface::Command {
     private:
      /// polymorphic wrapper type shortcut
      template <typename Value>
      using w = detail::PolymorphicWrapper<Value>;

      /// lazy variant shortcut
      template <typename T>
      using Lazy = detail::LazyInitializer<T>;

      using LazyVariantType = Lazy<CommandVariantType>;

     public:
      /// type of proto variant
      using ProtoCommandVariantType = boost::variant<w<AddAssetQuantity>>;

      /// list of types in proto variant
      using ProtoCommandListType = ProtoCommandVariantType::types;

      template <typename CommandType>
      explicit Command(CommandType &&command)
          : command_(std::forward<CommandType>(command)),
            variant_([this] { return load<ProtoCommandListType>(*command_); }),
            blob_([this] { return BlobType(command_->SerializeAsString()); }) {}

      Command(const Command &o) : Command(*o.command_) {}

      Command(Command &&o) noexcept : Command(std::move(o.command_.variant())) {
      }

      const CommandVariantType &get() const override { return *variant_; }

      const BlobType &blob() const override { return *blob_; }

      ModelType *copy() const override {
        return new Command(iroha::protocol::Command(*command_));
      }

     private:
      // ------------------------------| fields |-------------------------------

      // proto
      detail::ReferenceHolder<iroha::protocol::Command> command_;

      // lazy
      const LazyVariantType variant_;

      const Lazy<BlobType> blob_;
    };
  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_SHARED_MODEL_PROTO_COMMAND_HPP
