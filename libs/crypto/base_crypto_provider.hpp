/*
Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef IROHA_BASE_CRYPTO_PROVIDER_HPP
#define IROHA_BASE_CRYPTO_PROVIDER_HPP

#include <crypto/keypair.hpp>
namespace iroha {
  class BaseCryptoProvider {
   public:
    virtual bool verify(const std::vector<uint8_t> &msg,
                        const iroha::crypto::Keypair::pubkey_t &public_key) = 0;
    virtual iroha::crypto::Keypair::signature_t sign(
        const std::vector<uint8_t> &msg) = 0;
  };
}
#endif  // IROHA_BASE_CRYPTO_PROVIDER_HPP
