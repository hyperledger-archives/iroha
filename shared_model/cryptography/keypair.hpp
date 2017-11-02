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

#ifndef IROHA_SHARED_MODEL_KEYPAIR_HPP
#define IROHA_SHARED_MODEL_KEYPAIR_HPP

#include "cryptography/private_key.hpp"
#include "cryptography/public_key.hpp"
#include "interfaces/model_primitive.hpp"
#include "utils/string_builder.hpp"

namespace shared_model {
  namespace crypto {
    /**
     * Class for holding a keypair: public key and private key
     */
    class Keypair : public interface::ModelPrimitive<Keypair> {
     public:
      /// Type of public key
      using PublicKeyType = PublicKey;

      /**
       * @return public key
       */
      virtual const PublicKeyType &publicKey() const = 0;

      /// Type of private key
      using PrivateKeyType = PrivateKey;

      /**
       * @return private key
       */
      virtual const PrivateKeyType &privateKey() const = 0;

      bool operator==(const Keypair &keypair) const override {
        return publicKey() == keypair.publicKey()
            and privateKey() == keypair.privateKey();
      }

      std::string toString() const override {
        return detail::PrettyStringBuilder()
            .init("Keypair")
            .append("publicKey", publicKey().toString())
            .append("privateKey", privateKey().toString())
            .finalize();
      }
    };
  }
};

#endif  // IROHA_SHARED_MODEL_KEYPAIR_HPP
