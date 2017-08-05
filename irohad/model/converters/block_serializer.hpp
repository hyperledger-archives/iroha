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

#ifndef IROHA_BLOCK_SERIALIZER_HPP
#define IROHA_BLOCK_SERIALIZER_HPP

#include <functional>
#include <unordered_map>
#include <typeindex>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <model/block.hpp>

#include "model/converters/json_command_factory.hpp"
#include "model/converters/json_transaction_factory.hpp"
#include "model/converters/json_block_factory.hpp"

#include "common/types.hpp"

namespace iroha {
  namespace ametsuchi {

    using namespace rapidjson;

    class BlockSerializer {
     public:
      /**
       * Serialize block to blob
       * @param block
       * @return
       */
      std::vector<uint8_t> serialize(model::Block block);
      /**
       * Deserialize blob block to model Block
       * @param bytes
       * @return
       */
      nonstd::optional<model::Block> deserialize(
          const std::vector<uint8_t>& bytes);

      /**
       * Deserialize transactions from json doc to vector<>
       * @param doc
       * @param transactions
       * @return False: if json is ill-formed
       */
      bool deserialize(Document& doc,
                       std::vector<model::Transaction>&
                           transactions);  // its' also used in iroha-cli

      /**
       * Deserialize json transaction to iroha model Transaction
       * @param json_tx
       * @return Will return nullopt if transaction is ill-formed
       */
      nonstd::optional<model::Transaction> deserialize(
          const std::string json_tx);

     private:

      model::converters::JsonCommandFactory commandFactory;
      model::converters::JsonTransactionFactory transactionFactory;
      model::converters::JsonBlockFactory blockFactory;
    };
  }
}

#endif  // IROHA_BLOCK_SERIALIZER_HPP
