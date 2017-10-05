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

#include <gtest/gtest.h>
#include <crypto/crypto.hpp>
#include <crypto/hash.hpp>
#include <model/model_crypto_provider_impl.hpp>
#include <model/queries/get_asset_info.hpp>
#include "model/generators/query_generator.hpp"

iroha::model::Transaction create_transaction() {
  iroha::model::Transaction tx{};
  tx.creator_account_id = "test";

  tx.tx_counter = 0;
  tx.created_ts = 0;
  return tx;
}

TEST(CryptoProvider, SignAndVerify) {
  // generate privkey/pubkey keypair
  auto seed = iroha::create_seed();
  auto keypair = iroha::create_keypair(seed);

  auto model_tx = create_transaction();

  iroha::model::ModelCryptoProviderImpl crypto_provider(keypair);
  model_tx = crypto_provider.sign(model_tx);
  ASSERT_TRUE(crypto_provider.verify(model_tx));

  // now modify transaction's meta, so verify should fail
  model_tx.creator_account_id = "test1";
  ASSERT_FALSE(crypto_provider.verify(model_tx));

  // same for query
  // we can't work with generic queries so I've selected one of them
  // TODO: do we need checks for others?
  auto query = iroha::model::generators::QueryGenerator().generateGetAccount(
      0, "test", 0, "test");
  auto signed_query = crypto_provider.sign(*query);
  query->signature = signed_query->signature;
  ASSERT_TRUE(crypto_provider.verify(query));
  query->account_id = "kappa";
  ASSERT_FALSE(crypto_provider.verify(query));
}

TEST(CryptoProvider, SameHashAfterSign) {
  auto keypair = iroha::create_keypair();
  auto query = iroha::model::generators::QueryGenerator().generateGetAccount(
      0, "test", 0, "test");

  auto hash = iroha::hash(*query);

  iroha::model::ModelCryptoProviderImpl crypto_provider(keypair);
  auto signed_query = crypto_provider.sign(*query);

  auto hash_signed = iroha::hash(*signed_query);

  ASSERT_EQ(hash_signed, hash);
}
