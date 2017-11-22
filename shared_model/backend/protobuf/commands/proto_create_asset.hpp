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

#include "interfaces/commands/create_asset.hpp"

#ifndef IROHA_PROTO_CREATE_ASSET_HPP
#define IROHA_PROTO_CREATE_ASSET_HPP

namespace shared_model {
  namespace proto {

    class CreateAsset final : public interface::CreateAsset {
     private:
      using RefCreateAsset =
          detail::ReferenceHolder<iroha::protocol::Command,
                                  const iroha::protocol::CreateAsset &>;

     public:
      explicit CreateAsset(const iroha::protocol::Command &command)
          : CreateAsset(
                RefCreateAsset(command,
                               detail::makeReferenceGetter(
                                   &iroha::protocol::Command::create_asset))) {}

      explicit CreateAsset(iroha::protocol::Command &&command)
          : CreateAsset(
                RefCreateAsset(std::move(command),
                               detail::makeReferenceGetter(
                                   &iroha::protocol::Command::create_asset))) {}

      const AssetNameType &assetName() const override {
        return create_asset_->asset_name();
      }

      const interface::types::DomainIdType &domainId() const override {
        return create_asset_->domain_id();
      }

      const PrecisionType &precision() const override { return *precision_; }

      ModelType *copy() const override {
        iroha::protocol::Command command;
        *command.mutable_create_asset() = *create_asset_;
        return new CreateAsset(std::move(command));
      }

     private:
      // ----------------------------| private API |----------------------------
      explicit CreateAsset(RefCreateAsset &&ref)
          : create_asset_(std::move(ref)),
            precision_([this] { return create_asset_->precision(); }) {}

      RefCreateAsset create_asset_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

      Lazy<PrecisionType> precision_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_CREATE_ASSET_HPP
