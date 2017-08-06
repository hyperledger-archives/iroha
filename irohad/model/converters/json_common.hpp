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

#ifndef IROHA_JSON_COMMON_HPP
#define IROHA_JSON_COMMON_HPP

#include <rapidjson/document.h>
#include <initializer_list>
#include <nonstd/optional.hpp>
#include <string>
#include "model/signature.hpp"

namespace iroha {
  namespace model {
    namespace converters {

      bool verifyRequiredMembers(
          const rapidjson::Document& document,
          const std::initializer_list<std::string>& members);

      rapidjson::Document serializeSignature(const Signature& signature);

      nonstd::optional<Signature> deserializeSignature(
          const rapidjson::Document& document);

      nonstd::optional<rapidjson::Document> stringToJson(
          const std::string& string);

      std::string jsonToString(const rapidjson::Document& document);

      nonstd::optional<rapidjson::Document> vectorToJson(
          const std::vector<uint8_t>& vector);

      std::vector<uint8_t> jsonToVector(const rapidjson::Document& document);

    }  // namespace converters
  }    // namespace model
}  // namespace iroha

#endif  // IROHA_JSON_COMMON_HPP
