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

#include "model_crypto_provider_impl.hpp"
#include "crypto/crypto.hpp"
#include "crypto/hash.hpp"

#include "model/queries/get_account.hpp"
#include "model/queries/get_account_assets.hpp"
#include "model/queries/get_asset_info.hpp"
#include "model/queries/get_roles.hpp"
#include "model/queries/get_signatories.hpp"
#include "model/queries/get_transactions.hpp"

namespace iroha {
  namespace model {
    ModelCryptoProviderImpl::ModelCryptoProviderImpl(const keypair_t &keypair)
        : keypair_(keypair) {}

    bool ModelCryptoProviderImpl::verify(const Transaction &tx) const {
      return std::all_of(tx.signatures.begin(),
                         tx.signatures.end(),
                         [tx](const Signature &sig) {
                           return iroha::verify(iroha::hash(tx).to_string(),
                                                sig.pubkey,
                                                sig.signature);
                         });
    }

    bool ModelCryptoProviderImpl::verify(
        std::shared_ptr<const Query> query) const {
      return iroha::verify(iroha::hash(*query).to_string(),
                           query->signature.pubkey,
                           query->signature.signature);
    }

    bool ModelCryptoProviderImpl::verify(const Block &block) const {
      return std::all_of(
          block.sigs.begin(), block.sigs.end(), [block](const Signature &sig) {
            return iroha::verify(
                iroha::hash(block).to_string(), sig.pubkey, sig.signature);
          });
    }

    Block ModelCryptoProviderImpl::sign(const Block &block) const {
      auto signature = iroha::sign(
          iroha::hash(block).to_string(), keypair_.pubkey, keypair_.privkey);
      auto signed_block = block;
      signed_block.sigs.push_back(Signature{signature, keypair_.pubkey});
      return signed_block;
    }

    Transaction ModelCryptoProviderImpl::sign(
        const Transaction &transaction) const {
      auto signature = iroha::sign(iroha::hash(transaction).to_string(),
                                   keypair_.pubkey,
                                   keypair_.privkey);
      auto signed_transaction = transaction;
      signed_transaction.signatures.push_back(
          Signature{signature, keypair_.pubkey});
      return signed_transaction;
    }

    std::shared_ptr<const Query> ModelCryptoProviderImpl::sign(
        const Query &query) const {
      auto signature = iroha::sign(
          iroha::hash(query).to_string(), keypair_.pubkey, keypair_.privkey);

      // TODO something is wrong with the model?
      std::shared_ptr<Query> signed_query;
      if (instanceof <GetAccount>(query)) {
        signed_query = std::make_shared<GetAccount>(
            static_cast<const GetAccount &>(query));
      } else if (instanceof <GetAccountAssets>(query)) {
        signed_query = std::make_shared<GetAccountAssets>(
            static_cast<const GetAccountAssets &>(query));
      } else if (instanceof <GetAssetInfo>(query)) {
        signed_query = std::make_shared<GetAssetInfo>(
            static_cast<const GetAssetInfo &>(query));
      } else if (instanceof <GetRoles>(query)) {
        signed_query =
            std::make_shared<GetRoles>(static_cast<const GetRoles &>(query));
      } else if (instanceof <GetRolePermissions>(query)) {
        signed_query = std::make_shared<GetRolePermissions>(
            static_cast<const GetRolePermissions &>(query));
      } else if (instanceof <GetSignatories>(query)) {
        signed_query = std::make_shared<GetSignatories>(
            static_cast<const GetSignatories &>(query));
      } else if (instanceof <GetAccountAssetTransactions>(query)) {
        signed_query = std::make_shared<GetAccountAssetTransactions>(
            static_cast<const GetAccountAssetTransactions &>(query));
      } else if (instanceof <GetAccountAssets>(query)) {
        signed_query = std::make_shared<GetAccountAssets>(
            static_cast<const GetAccountAssets &>(query));
      }

      signed_query->signature =
          Signature{.signature = signature, .pubkey = keypair_.pubkey};
      return signed_query;
    }
  }
}
