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

#ifndef IROHA_PROTO_CREATE_DOMAIN_HPP
#define IROHA_PROTO_CREATE_DOMAIN_HPP

#include "interfaces/commands/create_domain.hpp"

namespace shared_model {
  namespace proto {

    class CreateDomain final : public interface::CreateDomain {
     private:
      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

     public:
      explicit CreateDomain(const iroha::protocol::Command &command)
          : CreateDomain(command.create_domain()) {
        if (not command.has_create_domain()) {
          // TODO 11/11/17 andrei create generic exception message
          throw std::invalid_argument("Object does not contain create_asset");
        }
      }

      const interface::types::DomainIdType &domainId() const override {
        return create_domain_.domain_id();
      }

      const interface::types::RoleIdType & userDefaultRole() const override {
        return create_domain_.default_role();
      }

      const HashType &hash() const override { return hash_.get(); }

      ModelType *copy() const override {
        return new CreateDomain(create_domain_);
      }

     private:
      // ----------------------------| private API |----------------------------
      explicit CreateDomain(const iroha::protocol::CreateDomain &create_domain)
          : create_domain_(create_domain),
            hash_([this] {
              // TODO 10/11/2017 muratovv replace with effective implementation
              return crypto::StubHash();
            }) {}

      iroha::protocol::CreateDomain create_domain_;
      Lazy<crypto::Hash> hash_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_CREATE_DOMAIN_HPP
