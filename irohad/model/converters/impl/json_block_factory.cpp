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

#include "model/converters/json_block_factory.hpp"
#include "model/converters/json_common.hpp"

using namespace rapidjson;

namespace iroha {
  namespace model {
    namespace converters {

      JsonBlockFactory::JsonBlockFactory() {
        log_ = logger::log("JsonBlockFactory");
      }

      Document JsonBlockFactory::serialize(const Block &block) {
        Document document;
        auto &allocator = document.GetAllocator();
        document.SetObject();

        Value signatures;
        signatures.SetArray();
        for (const auto &signature : block.sigs) {
          signatures.PushBack(serializeSignature(signature, allocator), allocator);
        }
        document.AddMember("signatures", signatures, allocator);

        document.AddMember("created_ts", block.created_ts, allocator);
        document.AddMember("hash", block.hash.to_hexstring(), allocator);
        document.AddMember("prev_hash", block.prev_hash.to_hexstring(),
                           allocator);
        document.AddMember("height", block.height, allocator);
        document.AddMember("txs_number", block.txs_number, allocator);
        document.AddMember("merkle_root", block.merkle_root.to_hexstring(),
                           allocator);

        Value commands;
        commands.SetArray();
        for (auto &&transaction : block.transactions) {
          commands.PushBack(
              Document(&allocator)
                  .CopyFrom(factory_.serialize(transaction), allocator),
              allocator);
        }
        document.AddMember("transactions", commands, allocator);

        return document;
      }

      nonstd::optional<Block> JsonBlockFactory::deserialize(
          const Document &document) {
        return nonstd::make_optional<model::Block>() | [&document](auto block) {
          return deserializeField(block, &Block::created_ts, document,
                                  "created_ts", &Value::IsUint64,
                                  &Value::GetUint64);
        } | [&document](auto block) {
          return deserializeField(block, &Block::height, document, "height",
                                  &Value::IsUint64, &Value::GetUint64);
        } | [&document](auto block) {
          return deserializeField(block, &Block::txs_number, document,
                                  "txs_number", &Value::IsUint,
                                  &Value::GetUint);
        } | [&document](auto block) {
          return deserializeField(block, &Block::hash, document, "hash",
                                  &Value::IsString, &Value::GetString);
        } | [&document](auto block) {
          return deserializeField(block, &Block::prev_hash, document,
                                  "prev_hash", &Value::IsString,
                                  &Value::GetString);
        } | [&document](auto block) {
          return deserializeField(block, &Block::merkle_root, document,
                                  "merkle_root", &Value::IsString,
                                  &Value::GetString);
        } | [&document](auto block) {
          return deserializeField(block, &Block::sigs, document, "signatures",
                                  &Value::IsArray, &Value::GetArray);
        } | [this, &document](auto transaction) {
          return deserializeField(
              transaction, &Block::transactions, document, "transactions",
              &Value::IsArray, &Value::GetArray, [this](auto array) {
                return std::accumulate(
                    array.begin(), array.end(),
                    nonstd::make_optional<Block::TransactionsType>(),
                    [this](auto init, auto &x) {
                      return factory_.deserialize(x) | [&init](auto command) {
                        init.value().push_back(command);
                        return init;
                      };
                    });
              });
        };
      }

    }  // namespace converters
  }    // namespace model
}  // namespace iroha
