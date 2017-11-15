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

#ifndef IROHA_PROTO_CREATE_ACCOUNT_HPP
#define IROHA_PROTO_CREATE_ACCOUNT_HPP

#include "interfaces/commands/create_account.hpp"

namespace shared_model {
namespace proto {

class CreateAccount final : public interface::CreateAccount {
 private:
  template <typename Value>
  using Lazy = detail::LazyInitializer<Value>;

 public:
  explicit CreateAccount(const iroha::protocol::Command &command)
      : CreateAccount(command.create_account()) {
    if (not command.has_create_account()) {
      // TODO 11/11/17 andrei create generic exception message
      throw std::invalid_argument(
          "Object does not contain create_account");
    }
  }

  const interface::types::PubkeyType & pubkey() const override {
    return pubkey_.get();
  }

  const AccountNameType& accountName() const override {
    return create_account_.account_name();
  }

  const interface::types::DomainIdType & domainId() const override {
    return create_account_.domain_id();
  }

  const HashType& hash() const override {
    return hash_.get();
  }

  ModelType* copy() const override {
    return new CreateAccount(create_account_);
  }

 private:
  // ----------------------------| private API |----------------------------
  explicit CreateAccount(
      const iroha::protocol::CreateAccount &create_account)
      : create_account_(create_account),
        pubkey_([this] {
          return interface::types::PubkeyType(create_account_.main_pubkey());
        }),
        hash_([this] {
          // TODO 10/11/2017 muratovv replace with effective implementation
          return crypto::StubHash();
        }) {}

  iroha::protocol::CreateAccount create_account_;
  Lazy<interface::types::PubkeyType> pubkey_;
  Lazy<crypto::Hash> hash_;
};

}  // namespace proto
}  // namespace shared_model

#endif //IROHA_PROTO_CREATE_ACCOUNT_HPP
