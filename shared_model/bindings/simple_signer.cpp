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

#include "bindings/simple_signer.hpp"
#include "cryptography/crypto_provider/crypto_signer.hpp"
#include "cryptography/ed25519_sha3_impl/crypto_provider.hpp"
#include "cryptography/seed.hpp"

namespace shared_model {
  namespace proto {
    crypto::Keypair SimpleSigner::generateKeypair() {
      return crypto::CryptoProviderEd25519Sha3::generateKeypair();
    }

    crypto::Signed SimpleSigner::sign(const crypto::Blob &blob,
                                      crypto::Keypair &keypair) {
      return crypto::CryptoSigner<>::sign(blob, keypair);
    }
  }  // namespace proto
}  // namespace shared_model
