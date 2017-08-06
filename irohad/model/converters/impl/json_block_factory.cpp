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
          signatures.PushBack(
              Document(&allocator)
                  .CopyFrom(serializeSignature(signature), allocator),
              allocator);
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
        model::Block block{};

        if (not verifyRequiredMembers(
                document, {"hash", "signatures", "created_ts", "height",
                           "prev_hash", "txs_number"})) {
          return nonstd::nullopt;
        }

        auto &signatures = document["signatures"];
        for (auto it = signatures.Begin(); it != signatures.End(); ++it) {
          Document signature_document;
          auto &allocator = signature_document.GetAllocator();
          signature_document.CopyFrom(*it, allocator);
          auto signature = deserializeSignature(signature_document);
          if (not signature.has_value()) {
            log_->error("Signature parsing failure");
            return nonstd::nullopt;
          }
          block.sigs.emplace_back(signature.value());
        }

        block.created_ts = document["created_ts"].GetUint64();
        block.height = document["height"].GetUint64();
        block.txs_number = static_cast<decltype(block.txs_number)>(
            document["txs_number"].GetUint());
        hexstringToArray(document["hash"].GetString(), block.hash);
        hexstringToArray(document["prev_hash"].GetString(), block.prev_hash);
        hexstringToArray(document["merkle_root"].GetString(),
                         block.merkle_root);

        auto& transactions = document["transactions"];
        for (auto it = transactions.Begin(); it != transactions.End(); ++it) {
          Document transaction_document;
          auto &allocator = transaction_document.GetAllocator();
          transaction_document.CopyFrom(*it, allocator);
          auto transaction = factory_.deserialize(transaction_document);
          if (not transaction.has_value()) {
            log_->error("Transaction parsing failure");
            return nonstd::nullopt;
          }
          block.transactions.emplace_back(transaction.value());
        }

        return block;
      }

    }  // namespace converters
  }    // namespace model
}  // namespace iroha
