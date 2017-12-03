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
#pragma SWIG nowarn=325, 401, 509, 516

%include "std_string.i"
%include "stdint.i"

%rename(prototx) shared_model::proto::Transaction;
%rename(_interface) interface;
%rename(b_equal) shared_model::crypto::Blob::operator==;
%rename(kp_equal) shared_model::crypto::Keypair::operator==;

%{
#include "bindings/simple_builder.hpp"
#include "bindings/simple_signer.hpp"
#include "bindings/simple_transaction_proto.hpp"
%}

%include "interfaces/common_objects/types.hpp"
%include "interfaces/signable.hpp"
%include "cryptography/blob.hpp"
%include "cryptography/public_key.hpp"
%include "cryptography/private_key.hpp"
%include "cryptography/keypair.hpp"
%include "cryptography/signed.hpp"
%include "backend/protobuf/transaction.hpp"
%include "builders/protobuf/proto_transaction_builder.hpp"
%include "bindings/simple_builder.hpp"
%include "bindings/simple_signer.hpp"
%include "bindings/simple_transaction_proto.hpp"


namespace shared_model {
  namespace proto {
    class SimpleBuilder;
    class SimpleSigner;
    class SimpleTransactionProto;
  }
}

namespace iroha {
  namespace protocol {
    class Transaction {
    public:
        size_t ByteSizeLong();
    };
  }
}
