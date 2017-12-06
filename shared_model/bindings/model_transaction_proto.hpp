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

#ifndef IROHA_SIMPLE_TRANSACTION_PROTO_HPP
#define IROHA_SIMPLE_TRANSACTION_PROTO_HPP

#include "builders/protobuf/transaction.hpp"
#include "builders/protobuf/unsigned_proto.hpp"
#include "cryptography/blob.hpp"

namespace shared_model {
  namespace bindings {
    /**
     * Class for proto operations for SWIG
     */
    class ModelTransactionProto {
     public:
      /**
       * Signs unsigned transaction and adds signature to its internal proto
       * object
       * @param tx - unsigned transaction
       * @param keypair - keypair to sign
       * @return blob of signed transaction
       */
      crypto::Blob signAndAddSignature(
          proto::UnsignedWrapper<proto::Transaction> &tx,
          const crypto::Keypair &keypair);
    };
  }  // namespace bindings
}  // namespace shared_model

#endif  // IROHA_SIMPLE_TRANSACTION_PROTO_HPP
