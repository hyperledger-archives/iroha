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

#ifndef IROHA_PROTO_ADD_SIGNATORY_HPP
#define IROHA_PROTO_ADD_SIGNATORY_HPP

#include "interfaces/commands/add_signatory.hpp"

namespace shared_model {
  namespace proto {

    class AddSignatory : public interface::AddSignatory {
     private:
      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

     public:
      explicit AddSignatory(const iroha::protocol::Command &command)
          : AddSignatory(command.add_signatory()) {
        if (not command.has_add_signatory()) {
          throw std::invalid_argument(
              "Object does not contain add_asset_quantity");
        }
      }

      const interface::types::AccountIdType &accountId() const override {
        return add_signatory_.account_id();
      }

      const interface::types::PubkeyType &pubkey() const override {
        return pubkey_.get();
      }

      ModelType *copy() const override {
        return new AddSignatory(add_signatory_);
      }

      const HashType &hash() const override { return hash_.get(); }

     private:
      explicit AddSignatory(const iroha::protocol::AddSignatory &add_signatory)
          : add_signatory_(add_signatory),
            pubkey_([this] {
              return interface::types::PubkeyType(add_signatory_.public_key());
            }),
            hash_([this] {
              // TODO 14/11/2017 kamilsa replace with effective implementation
              return crypto::StubHash();
            }) {}

      iroha::protocol::AddSignatory add_signatory_{};
      Lazy<interface::types::PubkeyType> pubkey_;
      Lazy<HashType> hash_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_ADD_SIGNATORY_HPP
