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

%module irohalib

#define DEPRECATED
#pragma SWIG nowarn=401, 509, 516

%include "std_string.i"
%include "stdint.i"

%rename(prototx) shared_model::proto::Transaction;
%rename(_interface) interface;
%rename(b_equal) shared_model::crypto::Blob::operator==;
%rename(kp_equal) shared_model::crypto::Keypair::operator==;

%{
#include "builders/protobuf/proto_transaction_builder.hpp"
#include "cryptography/crypto_provider/crypto_signer.hpp"
#include "cryptography/ed25519_sha3_impl/crypto_provider.hpp"
%}

%include "interfaces/common_objects/types.hpp"
%include "cryptography/blob.hpp"
%include "cryptography/seed.hpp"
%include "cryptography/signed.hpp"
%include "cryptography/public_key.hpp"
%include "cryptography/private_key.hpp"
%include "cryptography/keypair.hpp"
%include "interfaces/transaction.hpp"
%include "interfaces/hashable.hpp"
%include "backend/protobuf/transaction.hpp"
%include "builders/protobuf/proto_transaction_builder.hpp"
%include "cryptography/crypto_provider/crypto_signer.hpp"
%include "cryptography/ed25519_sha3_impl/crypto_provider.hpp"

namespace shared_model {
  namespace proto {
    class TransactionBuilder;
  }
  namespace crypto {
    %template(cs) shared_model::crypto::CryptoSigner<shared_model::crypto::DefaultCryptoAlgorithmType>;
    class CryptoProviderEd25519Sha3;
  }
}
