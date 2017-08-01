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

#include <model/commands/add_asset_quantity.hpp>
#include <model/commands/add_peer.hpp>
#include <model/commands/add_signatory.hpp>
#include <model/commands/assign_master_key.hpp>
#include <model/commands/create_account.hpp>
#include <model/commands/create_asset.hpp>
#include <model/commands/create_domain.hpp>
#include <model/commands/remove_signatory.hpp>
#include <model/commands/set_permissions.hpp>
#include <model/commands/set_quorum.hpp>
#include <model/commands/transfer_asset.hpp>

#include "common/types.hpp"

namespace iroha {
  namespace ametsuchi {

    using namespace rapidjson;

    class BlockSerializer {
     public:
      BlockSerializer();
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
      using Serializer = void (BlockSerializer::*)(PrettyWriter<StringBuffer>&,
                                                   const model::Command&);
      using Deserializer = std::shared_ptr<model::Command> (BlockSerializer::*)(
          GenericValue<UTF8<char>>::Object&);

      std::unordered_map<std::type_index, Serializer> serializers_;
      std::unordered_map<std::string, Deserializer> deserializers_;

      void serialize(PrettyWriter<StringBuffer>& writer,
                     const model::Block& block);
      void serialize(PrettyWriter<StringBuffer>& writer,
                     const model::Signature& signature);
      void serialize(PrettyWriter<StringBuffer>& writer,
                     const model::Transaction& transaction);
      void serialize(PrettyWriter<StringBuffer>& writer,
                     const model::Command& command);

      void serialize_add_peer(PrettyWriter<StringBuffer>& writer,
                     const model::Command& command);
      void serialize_add_asset_quantity(PrettyWriter<StringBuffer>& writer,
                     const model::Command& command);
      void serialize_add_signatory(PrettyWriter<StringBuffer>& writer,
                     const model::Command& command);
      void serialize_assign_master_key(PrettyWriter<StringBuffer>& writer,
                     const model::Command& command);
      void serialize_create_account(PrettyWriter<StringBuffer>& writer,
                     const model::Command& command);
      void serialize_create_asset(PrettyWriter<StringBuffer>& writer,
                     const model::Command& command);
      void serialize_create_domain(PrettyWriter<StringBuffer>& writer,
                     const model::Command& command);
      void serialize_remove_signatory(PrettyWriter<StringBuffer>& writer,
                     const model::Command& command);
      void serialize_set_account_permissions(
          PrettyWriter<StringBuffer>& writer,
          const model::Command& command);
      void serialize_set_quorum(PrettyWriter<StringBuffer>& writer,
                     const model::Command& command);
      void serialize_transfer_asset(PrettyWriter<StringBuffer>& writer,
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

      // Json serilaization for each command
      std::shared_ptr<model::Command> deserialize_add_peer(
          GenericValue<UTF8<char>>::Object& json_command);
      std::shared_ptr<model::Command> deserialize_add_asset_quantity(
          GenericValue<UTF8<char>>::Object& json_command);
      std::shared_ptr<model::Command> deserialize_add_signatory(
          GenericValue<UTF8<char>>::Object& json_command);
      std::shared_ptr<model::Command> deserialize_assign_master_key(
          GenericValue<UTF8<char>>::Object& json_command);
      std::shared_ptr<model::Command> deserialize_create_account(
          GenericValue<UTF8<char>>::Object& json_command);
      std::shared_ptr<model::Command> deserialize_create_asset(
          GenericValue<UTF8<char>>::Object& json_command);
      std::shared_ptr<model::Command> deserialize_create_domain(
          GenericValue<UTF8<char>>::Object& json_command);
      std::shared_ptr<model::Command> deserialize_remove_signatory(
          GenericValue<UTF8<char>>::Object& json_command);
      std::shared_ptr<model::Command> deserialize_set_account_permissions(
          GenericValue<UTF8<char>>::Object& json_command);
      std::shared_ptr<model::Command> deserialize_set_quorum(
          GenericValue<UTF8<char>>::Object& json_command);
      std::shared_ptr<model::Command> deserialize_transfer_asset(
          GenericValue<UTF8<char>>::Object& json_command);
    };
  }
}

#endif  // IROHA_BLOCK_SERIALIZER_HPP
