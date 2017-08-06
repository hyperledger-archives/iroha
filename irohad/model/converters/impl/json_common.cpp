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

#include "model/converters/json_common.hpp"
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <algorithm>
#include "common/types.hpp"

using namespace rapidjson;

namespace iroha {
  namespace model {
    namespace converters {

      bool verifyRequiredMembers(
          const Document& document,
          const std::initializer_list<std::string>& members) {
        auto verify_member = [&document](const auto& field) {
          return document.HasMember(field);
        };
        return std::all_of(members.begin(), members.end(), verify_member);
      }

      Document serializeSignature(const Signature& signature) {
        Document document;
        auto& allocator = document.GetAllocator();
        document.SetObject();

        document.AddMember("pubkey", signature.pubkey.to_hexstring(),
                           allocator);
        document.AddMember("signature", signature.signature.to_hexstring(),
                           allocator);

        return document;
      }

      nonstd::optional<Signature> deserializeSignature(
          const Document& document) {
        model::Signature signature{};

        if (not document.HasMember("pubkey") or
            not document.HasMember("signature")) {
          return nonstd::nullopt;
        }

        hexstringToArray(document["pubkey"].GetString(), signature.pubkey);

        hexstringToArray(document["signature"].GetString(),
                         signature.signature);

        return signature;
      }

      nonstd::optional<Document> stringToJson(const std::string& string) {
        Document document;
        document.Parse(string);
        if (document.HasParseError()) {
          return nonstd::nullopt;
        }
        return document;
      }

      std::string jsonToString(const Document& document) {
        StringBuffer sb;
        PrettyWriter<StringBuffer> writer(sb);
        document.Accept(writer);
        return sb.GetString();
      }

      nonstd::optional<Document> vectorToJson(
          const std::vector<uint8_t>& vector) {
        return stringToJson(bytesToString(vector));
      }

      std::vector<uint8_t> jsonToVector(const Document& document) {
        return stringToBytes(jsonToString(document));
      }

    }  // namespace converters
  }    // namespace model
}  // namespace iroha
