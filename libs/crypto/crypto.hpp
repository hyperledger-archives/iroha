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

#ifndef IROHA_CRYPTO_HPP
#define IROHA_CRYPTO_HPP

#include <common/types.hpp>
#include <string>

namespace iroha {

  /**
   * Sign message with ed25519 crypto algorithm
   * @param msg
   * @param msgsize
   * @param pub
   * @param priv
   * @return
   */
  ed25519::sig_t sign(const uint8_t *msg, size_t msgsize,
                      const ed25519::pubkey_t &pub,
                      const ed25519::privkey_t &priv);

  /**
   * Verify signature of ed25519 crypto algorithm
   * @param msg
   * @param msgsize
   * @param pub
   * @param sig
   * @return true if signature is valid, false otherwise
   */
  bool verify(const uint8_t *msg, size_t msgsize, const ed25519::pubkey_t &pub,
              const ed25519::sig_t &sig);

  /**
   * Create new keypair
   * @param seed
   * @return
   */
  std::pair<ed25519::pubkey_t, ed25519::privkey_t> create_keypair(
      blob_t<32> seed);

  /**
   * Generate random seed reading from /dev/urandom
   */
  blob_t<32> create_seed();

  /**
   * Generate random seed as sha3_256(passphrase)
   * @param passphrase
   * @return
   */
  blob_t<32> create_seed(std::string passphrase);

  /**
   * Extend this template to sign custom objects with ed25519
   * @tparam T - custom type
   * @param msg message of type T
   * @param pub
   * @param priv
   * @return ed25519 signature
   */
  template <typename T>
  ed25519::sig_t sign(const T &msg, const ed25519::pubkey_t &pub,
                      const ed25519::privkey_t &priv);

  /**
   * Extend this template to verify ed25519 signatures of custom objects
   * @tparam T
   * @param msg
   * @param pub
   * @param sig
   * @return true if signature is valid, false otherwise
   */
  template <typename T>
  bool verify(const T &msg, const ed25519::pubkey_t &pub,
              const ed25519::sig_t &sig);
}
#endif  // IROHA_CRYPTO_HPP
