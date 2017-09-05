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

#include <algorithm>
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
          signatures.PushBack(serializeSignature(signature, allocator), allocator);
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
          const Value &document) {
        return nonstd::make_optional<Transaction>() | [&document](
                                                          auto transaction) {
          return deserializeField(transaction, &Transaction::created_ts,
                                  document, "created_ts", &Value::IsUint64,
                                  &Value::GetUint64);
        } | [&document](auto transaction) {
          return deserializeField(transaction, &Transaction::creator_account_id,
                                  document, "creator_account_id",
                                  &Value::IsString, &Value::GetString);
        } | [&document](auto transaction) {
          return deserializeField(transaction, &Transaction::tx_counter,
                                  document, "tx_counter", &Value::IsUint64,
                                  &Value::GetUint64);
        } | [&document](auto transaction) {
          return deserializeField(transaction, &Transaction::signatures,
                                  document, "signatures", &Value::IsArray,
                                  &Value::GetArray);
        } | [this, &document](auto transaction) {
          return deserializeField(
              transaction, &Transaction::commands, document, "commands",
              &Value::IsArray, &Value::GetArray, [this](auto array) {
                return std::accumulate(
                    array.begin(), array.end(),
                    nonstd::make_optional<Transaction::CommandsType>(),
                    [this](auto init, auto &x) {
                      return factory_.deserializeAbstractCommand(x) |
                             [&init](auto command) {
                               init.value().push_back(command);
                               return init;
                             };
                    });
              });
        } | [this, &document](auto transaction) {
          transaction.tx_hash = hash_provider_.get_hash(transaction);
          return nonstd::make_optional(transaction);
        };
      }

    }  // namespace converters
  }    // namespace model
}  // namespace iroha
