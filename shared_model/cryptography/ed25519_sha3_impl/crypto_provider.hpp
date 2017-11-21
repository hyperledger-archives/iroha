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

#ifndef IROHA_CRYPTOPROVIDER_HPP
#define IROHA_CRYPTOPROVIDER_HPP

#include "cryptography/ed25519_sha3_impl/signer.hpp"
#include "cryptography/ed25519_sha3_impl/verifier.hpp"
#include "cryptography/seed.hpp"

namespace shared_model {
  namespace crypto {
    /**
     * Wrapper class for signing-related stuff.
     */
    class CryptoProvider {
     public:
      /**
       * Signs the message.
       * @param blob - blob to sign
       * @param keypair - keypair
       * @return Signed object with signed data
       */
      Signed sign(const Blob &blob, const Keypair &keypair) const;

      /**
       * Verifies signature.
       * @param signedData - data to verify
       * @param orig - original message
       * @param publicKey - public key
       * @return true if verify was OK or false otherwise
       */
      bool verify(const Signed &signedData,
                  const Blob &orig,
                  const PublicKey &publicKey) const;

      /**
       * Generates new seed
       * @return Seed generated
       */
      Seed generateSeed() const;

      /**
       * Generates new seed from a provided passphrase
       * @param passphrase - passphrase to generate seed from
       * @return Seed generated
       */
      Seed generateSeed(const std::string &passphrase) const;

      /**
       * Generates new keypair with a default seed
       * @return Keypair generated
       */
      Keypair generateKeypair() const;

      /**
       * Generates new keypair from a provided seed
       * @param seed - provided seed
       * @return generated keypair
       */
      Keypair generateKeypair(const Seed &seed) const;

     private:
      Signer signer_;
      Verifier verifier_;
    };
  }  // namespace crypto
}  // namespace shared_model

#endif  // IROHA_CRYPTOPROVIDER_HPP
