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

#include "interfaces/commands/remove_signatory.hpp"

#ifndef IROHA_PROTO_REMOVE_SIGNATORY_HPP
#define IROHA_PROTO_REMOVE_SIGNATORY_HPP

namespace shared_model {
  namespace proto {

    class RemoveSignatory final : public interface::RemoveSignatory {
     private:
      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;

     public:
      explicit RemoveSignatory(const iroha::protocol::Command &command)
          : RemoveSignatory(command.remove_sign()) {
        if (not command.has_remove_sign()) {
          // TODO 11/11/17 andrei create generic exception message
          throw std::invalid_argument("Object does not contain create_asset");
        }
      }

      const interface::types::AccountIdType &accountId()
          const override {
        return remove_signatory_.account_id();
      }

      const interface::types::PubkeyType & pubkey() const override {
        return pubkey_.get();
      }

      const HashType &hash() const override { return hash_.get(); }

      ModelType *copy() const override {
        return new RemoveSignatory(remove_signatory_);
      }

     private:
      // ----------------------------| private API |----------------------------
      explicit RemoveSignatory(
          const iroha::protocol::RemoveSignatory &remove_signatory)
          : remove_signatory_(remove_signatory),
            pubkey_([this] {
              return interface::types::PubkeyType(remove_signatory_.public_key());
            }),
            hash_([this] {
              // TODO 10/11/2017 muratovv replace with effective implementation
              return crypto::StubHash();
            }) {}

      iroha::protocol::RemoveSignatory remove_signatory_;
      Lazy<interface::types::PubkeyType> pubkey_;
      Lazy<crypto::Hash> hash_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_REMOVE_SIGNATORY_HPP
