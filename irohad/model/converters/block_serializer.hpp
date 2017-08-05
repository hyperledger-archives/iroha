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

      void serialize(PrettyWriter<StringBuffer>& writer,
                     const model::Block& block);
      void serialize(PrettyWriter<StringBuffer>& writer,
                     const model::Signature& signature);
      void serialize(PrettyWriter<StringBuffer>& writer,
                     const model::Transaction& transaction);
      void serialize(PrettyWriter<StringBuffer>& writer,
                     const model::Command& command);

      // Deserialize one transaction
      nonstd::optional<model::Transaction> deserialize(
          GenericValue<rapidjson::UTF8<char>>::Object& json_tx);

      // Deserialize hex string to array
      template <size_t size>
      void deserialize(const std::string& string, blob_t<size>& array) {
        auto bytes = hex2bytes(string);
        std::copy(bytes.begin(), bytes.end(), array.begin());
      }

      // Deserialize signatures
      bool deserialize(GenericValue<rapidjson::UTF8<char>>::Array json_sigs,
                       std::vector<model::Signature>& sigs);

      // Deserialize commands withing trasaction json_tx
      // Return false if json is ill-formed
      bool deserialize(GenericValue<rapidjson::UTF8<char>>::Object& json_tx,
                       std::vector<std::shared_ptr<model::Command>>& commands);
    };
  }
}

#endif  // IROHA_BLOCK_SERIALIZER_HPP
