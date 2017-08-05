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
      auto json = blockFactory.serialize(block);
      json.Accept(writer);
      auto str = sb.GetString();
      std::vector<uint8_t> bytes{str, str + sb.GetLength()};
      return bytes;
    }

    /* Deserialize */

    nonstd::optional<model::Block> BlockSerializer::deserialize(
        const std::vector<uint8_t>& bytes) {
      std::string block_json(bytes.begin(), bytes.end());
      rapidjson::Document doc;
      if (doc.Parse(block_json).HasParseError()) {
        return nonstd::nullopt;
      }

      auto block = blockFactory.deserialize(doc);

      return block;
    }


    nonstd::optional<model::Transaction> BlockSerializer::deserialize(
        const std::string json_tx) {
      Document doc;
      if (doc.Parse(json_tx).HasParseError()) {
        return nonstd::nullopt;
      }
      return transactionFactory.deserialize(doc);
    }

    bool BlockSerializer::deserialize(
        Document& doc, std::vector<model::Transaction>& transactions) {
      if (not doc.HasMember("transactions")) {
        return false;
      }
      auto json_txs = doc["transactions"].GetArray();
      for (auto&& iter : json_txs) {
        Document document;
        auto& allocator = document.GetAllocator();
        document.CopyFrom(iter, allocator);
        auto tx_opt = transactionFactory.deserialize(document);
        if (not tx_opt.has_value()) {
          return false;
        }
        auto tx = tx_opt.value();
        transactions.push_back(tx);
      }
      return true;
    }

  }  // namespace ametsuchi
}  // namespace iroha
