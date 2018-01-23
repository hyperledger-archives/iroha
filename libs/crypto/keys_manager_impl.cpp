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

#include "keys_manager_impl.hpp"
#include "cryptography/ed25519_sha3_impl/internal/ed25519_impl.hpp"
#include "cryptography/ed25519_sha3_impl/internal/sha3_hash.hpp"
#include "logger/logger.hpp"

#include <algorithm>
#include <fstream>
#include <random>
#include <utility>

using iroha::operator|;

namespace iroha {
  /**
   * Return function which will try to deserialize specified value to specified
   * field in given keypair
   * @tparam T - keypair field type
   * @tparam V - value type to deserialize
   * @param field - keypair field to be deserialized
   * @param value - value to be deserialized
   * @return keypair on success, otherwise nullopt
   */
  template <typename T, typename V>
  auto deserializeKeypairField(T keypair_t::*field, const V &value) {
    return [=](auto keypair) mutable {
      return hexstringToArray<T::size()>(value)
          | assignObjectField(keypair, field);
    };
  }

  std::string encrypt(const privkey_t &privkey,
                      const std::string &pass_phrase) {
    std::string ciphertext(privkey.size(), '\0');
    const auto pass_size = std::max(1ul, pass_phrase.size());

    for (auto i = 0u; i < privkey.size(); i++) {
      ciphertext[i] = privkey[i] ^ pass_phrase[i % pass_size];
    }
    return ciphertext;
  }

  auto deserializedEncrypted(const std::string &s,
                             const std::string &pass_phrase) {
    constexpr auto size = privkey_t::size();
    auto decryption = [=](auto binstr) {
      std::string key(binstr.size(), '\0');
      const auto pass_size = std::max(1ul, pass_phrase.size());

      for (auto i = 0u; i < binstr.size(); i++) {
        key[i] = binstr[i] ^ pass_phrase[i % pass_size];
      }
      return nonstd::make_optional(key);
    };

    return [=](auto keypair) mutable {
      return hexstringToBytestring(s) | decryption
          | stringToBlob<
                 size> | assignObjectField(keypair, &keypair_t::privkey);
    };
  }

  KeysManagerImpl::KeysManagerImpl(const std::string &account_name)
      : account_name_(std::move(account_name)),
        log_(logger::log("KeysManagerImpl")) {}

  bool KeysManagerImpl::validate(const keypair_t &keypair) const {
    std::string test = "12345";
    auto sig =
        sign(sha3_256(test).to_string(), keypair.pubkey, keypair.privkey);
    if (not verify(sha3_256(test).to_string(), keypair.pubkey, sig)) {
      log_->error("key validation failed");
      return false;
    }
    return true;
  }

  bool KeysManagerImpl::loadFile(const std::string &filename,
                                 std::string &res) {
    std::ifstream file(filename);
    if (not file) {
      log_->error("Cannot read '" + filename + "'");
      return false;
    }
    file >> res;
    return true;
  }

  nonstd::optional<keypair_t> KeysManagerImpl::loadKeys() {
    std::string pub_key;
    std::string priv_key;

    if (!loadFile(account_name_ + kPubExt, pub_key)
        || !loadFile(account_name_ + kPrivExt, priv_key))
      return nonstd::nullopt;

    return nonstd::make_optional<keypair_t>()
        | deserializeKeypairField(&keypair_t::pubkey, pub_key)
        | deserializeKeypairField(&keypair_t::privkey, priv_key) |
        [this](auto keypair) {
          return this->validate(keypair) ? nonstd::make_optional(keypair)
                                         : nonstd::nullopt;
        };
  }

  nonstd::optional<keypair_t> KeysManagerImpl::loadKeys(
      const std::string &pass_phrase) {
    std::string pub_key;
    std::string priv_key;

    if (!loadFile(account_name_ + kPubExt, pub_key)
        || !loadFile(account_name_ + kPrivExt, priv_key))
      return nonstd::nullopt;

    return nonstd::make_optional<keypair_t>()
        | deserializeKeypairField(&keypair_t::pubkey, pub_key)
        | deserializedEncrypted(priv_key, pass_phrase) | [this](auto keypair) {
            return this->validate(keypair) ? nonstd::make_optional(keypair)
                                           : nonstd::nullopt;
          };
  }

  keypair_t generate() {
    blob_t<32> seed;
    std::generate(seed.begin(), seed.end(), [] {
      static std::random_device rd;
      static std::uniform_int_distribution<> dist;
      return dist(rd);
    });

    return create_keypair(seed);
  }

  bool KeysManagerImpl::createKeys() {
    auto key_pairs = generate();

    auto pub = key_pairs.pubkey.to_hexstring();
    auto priv = key_pairs.privkey.to_hexstring();
    return store(pub, priv);
  }

  bool KeysManagerImpl::createKeys(const std::string &pass_phrase) {
    auto key_pairs = generate();

    auto pub = key_pairs.pubkey.to_hexstring();
    auto priv = bytestringToHexstring(encrypt(key_pairs.privkey, pass_phrase));
    return store(pub, priv);
  }

  bool KeysManagerImpl::store(const std::string &pub, const std::string &priv) {
    std::ofstream pub_file(account_name_ + kPubExt);
    std::ofstream priv_file(account_name_ + kPrivExt);
    if (not pub_file or not priv_file) {
      return false;
    }

    pub_file << pub;
    priv_file << priv;
    return pub_file.good() && priv_file.good();
  }

  const std::string KeysManagerImpl::kPubExt = ".pub";
  const std::string KeysManagerImpl::kPrivExt = ".priv";
}  // namespace iroha
