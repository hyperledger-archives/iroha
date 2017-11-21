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

%module crypto
%include "std_string.i"

%{
#include "cryptography/ed25519_sha3_impl/crypto_provider.hpp"
%}

%include "interfaces/model_primitive.hpp"
%include "interfaces/primitive.hpp"
%include "cryptography/blob.hpp"
%include "cryptography/seed.hpp"
%include "cryptography/signed.hpp"
%include "cryptography/public_key.hpp"
%include "cryptography/private_key.hpp"
%include "cryptography/keypair.hpp"
%include "cryptography/ed25519_sha3_impl/crypto_provider.hpp"

namespace shared_model {
  namespace interface {
    %template(imodelprim) shared_model::interface::ModelPrimitive<shared_model::crypto::Blob>;
    %template(ikeypair) ModelPrimitive<crypto::Keypair>;
    %template(iprim) shared_model::interface::Primitive<crypto::Keypair, iroha::keypair_t>;
  }

  namespace crypto {
    class Blob;
    class Keypair;
    class Seed;
    class Signed;
    class PublicKey;
    class PrivateKey;
    class CryptoProvider;
  }
}
