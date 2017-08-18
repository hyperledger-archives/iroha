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

#include "model/converters/json_transaction_factory.hpp"
#include "model/converters/json_common.hpp"

using namespace rapidjson;

namespace iroha {
  namespace model {
    namespace converters {

      Document JsonTransactionFactory::serialize(
          const Transaction &transaction) {
        Document document;
        auto& allocator = document.GetAllocator();
        document.SetObject();

        Value signatures;
        signatures.SetArray();
        for (const auto &signature : transaction.signatures) {
          signatures.PushBack(
              Document(&allocator)
                  .CopyFrom(serializeSignature(signature), allocator),
              allocator);
        }
        document.AddMember("signatures", signatures, allocator);

        document.AddMember("created_ts", transaction.created_ts, allocator);
        document.AddMember("creator_account_id", transaction.creator_account_id, allocator);
        document.AddMember("tx_counter", transaction.tx_counter, allocator);


        Value commands;
        commands.SetArray();
        for (auto &&command : transaction.commands) {
          commands.PushBack(
              Document(&allocator)
                  .CopyFrom(factory_.serializeAbstractCommand(command),
                            allocator),
              allocator);
        }

        document.AddMember("commands", commands, allocator);

        return document;
      }

      nonstd::optional<Transaction> JsonTransactionFactory::deserialize(
          const Document &document) {
        model::Transaction tx{};

        if (not verifyRequiredMembers(
                document, {"creator_account_id", "tx_counter", "commands",
                           "signatures", "created_ts"})) {
          return nonstd::nullopt;
        }

        for (auto it = document["signatures"].Begin(); it != document["signatures"].End(); ++it) {
          Document signature_document;
          auto &allocator = signature_document.GetAllocator();
          signature_document.CopyFrom(*it, allocator);
          auto signature = deserializeSignature(signature_document);
          if (not signature.has_value()) {
            return nonstd::nullopt;
          }
          tx.signatures.emplace_back(signature.value());
        }

        tx.created_ts = document["created_ts"].GetUint64();

        tx.creator_account_id = document["creator_account_id"].GetString();

        tx.tx_counter = document["tx_counter"].GetUint64();

        for (auto it = document["commands"].Begin(); it != document["commands"].End(); ++it) {
          Document command_document;
          auto &allocator = command_document.GetAllocator();
          command_document.CopyFrom(*it, allocator);
          auto command = factory_.deserializeAbstractCommand(command_document);
          if (not command.has_value()) {
            return nonstd::nullopt;
          }
          tx.commands.emplace_back(command.value());
        }

        return tx;
      }

    }  // namespace converters
  }    // namespace model
}  // namespace iroha
