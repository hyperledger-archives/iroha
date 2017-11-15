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
      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

     public:
      explicit CreateAsset(const iroha::protocol::Command &command)
          : CreateAsset(command.create_asset()) {
        if (not command.has_create_asset()) {
          // TODO 11/11/17 andrei create generic exception message
          throw std::invalid_argument("Object does not contain create_asset");
        }
      }

      const AssetNameType &assetName() const override {
        return create_asset_.asset_name();
      }

      const interface::types::DomainIdType &domainId() const override {
        return create_asset_.domain_id();
      }

      const PrecisionType &precision() const override {
        return precision_.get();
      }

      const HashType &hash() const override { return hash_.get(); }

      ModelType *copy() const override {
        return new CreateAsset(create_asset_);
      }

     private:
      // ----------------------------| private API |----------------------------
      explicit CreateAsset(const iroha::protocol::CreateAsset &create_asset)
          : create_asset_(create_asset),
            hash_([this] {
              // TODO 10/11/2017 muratovv replace with effective implementation
              return crypto::StubHash();
            }),
            precision_([this] { return create_asset_.precision(); }) {}

      iroha::protocol::CreateAsset create_asset_;
      Lazy<crypto::Hash> hash_;
      Lazy<PrecisionType> precision_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_CREATE_ASSET_HPP
