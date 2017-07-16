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

#include "model/converters/pb_transaction_factory.hpp"

namespace iroha {
  namespace model {
    namespace converters {

      protocol::Transaction PbTransactionFactory::serialize(model::Transaction &tx) {
        protocol::Transaction pb_tx;

        // -----|Header|-----
        auto header = pb_tx.mutable_header();
        header->set_created_time(tx.created_ts);
        // todo set signatures

        // -----|Meta|-----
        auto meta = pb_tx.mutable_meta();
        meta->set_creator_account_id(tx.creator_account_id);
        meta->set_tx_counter(tx.tx_counter);

        // -----|Body|-----
        auto body = pb_tx.mutable_body();
        // todo set commands
        return pb_tx;
      }

      model::Transaction PbTransactionFactory::deserialize(protocol::Transaction &pb_tx) {

      }
    } // namespace converters
  }  // namespace model
}  // namespace iroha