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

#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/reader.h>
#include <algorithm>
#include <model/converters/block_serializer.hpp>
#include <common/types.hpp>
#include <iostream>
#include <sstream>

namespace iroha {
  namespace ametsuchi {

    using namespace rapidjson;

    /* Serialize */

    std::vector<uint8_t> BlockSerializer::serialize(const model::Block block) {
      rapidjson::StringBuffer sb;
      rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
      serialize(writer, block);
      auto str = sb.GetString();
      std::vector<uint8_t> bytes{str, str + sb.GetLength()};
      return bytes;
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    const model::Block& block) {
      writer.StartObject();
      writer.String("hash");
      writer.String(block.hash.to_hexstring());

      writer.String("signatures");
      writer.StartArray();
      for (auto sig : block.sigs) {
        serialize(writer, sig);
      }
      writer.EndArray();

      writer.String("created_ts");
      writer.Uint64(block.created_ts);

      writer.String("height");
      writer.Uint64(block.height);

      writer.String("prev_hash");
      writer.String(block.prev_hash.to_hexstring());

      writer.String("txs_number");
      writer.Uint(block.txs_number);

      writer.String("merkle_root");
      writer.String(block.merkle_root.to_hexstring());

      writer.String("transactions");
      writer.StartArray();
      for (auto tx : block.transactions) {
        serialize(writer, tx);
      }
      writer.EndArray();

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    const model::Signature& signature) {
      writer.StartObject();

      writer.String("pubkey");
      writer.String(signature.pubkey.to_hexstring());

      writer.String("signature");
      writer.String(signature.signature.to_hexstring());

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    const model::Transaction& transaction) {
      writer.StartObject();

      writer.String("signatures");
      writer.StartArray();
      for (auto sig : transaction.signatures) {
        serialize(writer, sig);
      }
      writer.EndArray();

      writer.String("created_ts");
      writer.Uint64(transaction.created_ts);

      writer.String("creator_account_id");
      writer.String(transaction.creator_account_id);

      writer.String("tx_counter");
      writer.Uint64(transaction.tx_counter);

      writer.String("commands");
      writer.StartArray();
      for (auto command : transaction.commands) {
        serialize(writer, *command);
      }
      writer.EndArray();

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    const model::Command& command) {
      auto ptr = std::shared_ptr<model::Command>(
          const_cast<model::Command*>(&command), [](auto){});
      auto json = commandFactory.serializeAbstractCommand(ptr);
      json.Accept(writer);
    }

    /* Deserialize */

    nonstd::optional<model::Block> BlockSerializer::deserialize(
        const std::vector<uint8_t>& bytes) {
      std::string block_json(bytes.begin(), bytes.end());
      rapidjson::Document doc;
      if (doc.Parse(block_json).HasParseError()) {
        return nonstd::nullopt;
      }

      auto req_fields = {"hash",   "signatures", "created_ts",
                         "height", "prev_hash",  "txs_number"};
      auto verify_member = [&doc](auto&& field) {
        return not doc.HasMember(field);
      };

      auto json_sigs = doc["signatures"].GetArray();

      if (std::any_of(req_fields.begin(), req_fields.end(), verify_member)) {
        return nonstd::nullopt;
      }

      model::Block block{};

      // Check if hash is present
      deserialize(doc["hash"].GetString(), block.hash);

      // Signatures are critical part of a Block, if there are none - Block is
      // invalid
      if (not deserialize(json_sigs, block.sigs)) {
        return nonstd::nullopt;
      }

      // created_ts
      block.created_ts = doc["created_ts"].GetUint64();

      // height
      block.height = doc["height"].GetUint64();

      // prev_hash
      deserialize(doc["prev_hash"].GetString(), block.prev_hash);

      // txs number
      block.txs_number =
          static_cast<decltype(block.txs_number)>(doc["txs_number"].GetUint());
      // merkle_root is optional
      if (doc.HasMember("merkle_root")) {
        deserialize(doc["merkle_root"].GetString(), block.merkle_root);
      }

      // Deserialize transactions
      if (not deserialize(doc, block.transactions)) {
        return nonstd::nullopt;
      };

      return block;
    }

    nonstd::optional<model::Transaction> BlockSerializer::deserialize(
        GenericValue<rapidjson::UTF8<char>>::Object& json_tx) {
      model::Transaction tx{};

      auto req_fields = {"creator_account_id", "tx_counter", "commands",
                         "signatures", "created_ts"};
      auto verify_member = [&json_tx](auto&& field) {
        return not json_tx.HasMember(field);
      };
      if (std::any_of(req_fields.begin(), req_fields.end(), verify_member)) {
        return nonstd::nullopt;
      }

      if (not deserialize(json_tx["signatures"].GetArray(), tx.signatures)) {
        return nonstd::nullopt;
      }
      // Created ts
      tx.created_ts = json_tx["created_ts"].GetUint64();
      // creator_account_id
      tx.creator_account_id = json_tx["creator_account_id"].GetString();
      // tx_counter
      tx.tx_counter = json_tx["tx_counter"].GetUint64();
      // deserialize commands
      if (not deserialize(json_tx, tx.commands)) {
        return nonstd::nullopt;
      }
      return tx;
    }

    nonstd::optional<model::Transaction> BlockSerializer::deserialize(
        const std::string json_tx) {
      Document doc;
      if (doc.Parse(json_tx).HasParseError()) {
        return nonstd::nullopt;
      }
      auto obj_tx = doc.GetObject();
      return deserialize(obj_tx);
    }

    bool BlockSerializer::deserialize(
        Document& doc, std::vector<model::Transaction>& transactions) {
      if (not doc.HasMember("transactions")) {
        return false;
      }
      auto json_txs = doc["transactions"].GetArray();
      for (auto&& iter : json_txs) {
        auto json_obj = iter.GetObject();
        auto tx_opt = deserialize(json_obj);
        if (not tx_opt.has_value()) {
          return false;
        }
        auto tx = tx_opt.value();
        transactions.push_back(tx);
      }
      return true;
    }

    bool BlockSerializer::deserialize(
        GenericValue<rapidjson::UTF8<char>>::Array json_sigs,
        std::vector<model::Signature>& sigs) {
      for (auto&& sig_iter : json_sigs) {
        auto json_sig = sig_iter.GetObject();
        model::Signature signature{};

        if (not sig_iter.HasMember("pubkey") or
            not sig_iter.HasMember("signature")) {
          return false;
        }

        deserialize(json_sig["pubkey"].GetString(), signature.pubkey);

        deserialize(json_sig["signature"].GetString(), signature.signature);

        sigs.push_back(signature);
      }
      return true;
    }

    bool BlockSerializer::deserialize(
        GenericValue<UTF8<char>>::Object& json_tx,
        std::vector<std::shared_ptr<model::Command>>& commands) {
      auto json_commands = json_tx["commands"].GetArray();

      auto deserialize_command = [this, &commands](auto&& value) {
        auto json_command = value.GetObject();
        if (not json_command.HasMember("command_type")) {
          return false;
        }
        std::string command_type = json_command["command_type"].GetString();
        Document document;
        auto& allocator = document.GetAllocator();
        document.CopyFrom(value, allocator);
        std::shared_ptr<model::Command> cmd;
        if (cmd = commandFactory.deserializeAbstractCommand(document)) {
          commands.push_back(cmd);
          return true;
        }
        return false;
      };

      return std::all_of(json_commands.begin(), json_commands.end(),
                         deserialize_command);
    }
  }  // namespace ametsuchi
}  // namespace iroha
