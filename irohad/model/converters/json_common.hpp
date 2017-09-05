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
#include "model/block.hpp"
#include <algorithm>
#include "model/common.hpp"

namespace iroha {
  // Deserialize hex string to array
  template <size_t size>
  nonstd::optional<blob_t<size>> hexstringToArray(const std::string& string) {
    if (size * 2 != string.size()) {
      return nonstd::nullopt;
    }
    blob_t<size> array;
    auto bytes = hex2bytes(string);
    std::copy(bytes.begin(), bytes.end(), array.begin());
    return array;
  }

  namespace model {
    namespace converters {
      template<typename T, typename Transform>
      auto operator|(T t, Transform f) -> decltype(f(*t)) {
        if (t) {
          return f(*t);
        }
        return {};
      }

      template <typename T, typename V>
      struct Transform {
        auto operator()(T x) {
          return nonstd::optional<V>(x);
        }
      };

      template <typename T>
      struct Transform<T, Block::HashType> {
        auto operator()(T x) {
          return hexstringToArray<Block::HashType::size()>(x);
        }
      };

      template <typename T>
      struct Transform<T, Signature::SignatureType> {
        auto operator()(T x) {
          return hexstringToArray<Signature::SignatureType::size()>(x);
        }
      };

      template <typename T, typename D>
      nonstd::optional<T> deserializeField(const D& document,
                                           const std::string& field,
                                           bool (rapidjson::Value::*verify)()
                                               const,
                                           T (rapidjson::Value::*get)() const) {
        if (document.HasMember(field.c_str()) and
            (document[field.c_str()].*verify)()) {
          return (document[field.c_str()].*get)();
        }
        return nonstd::nullopt;
      }

      template <typename T, typename V, typename B, typename D,
                typename Transform = Transform<T, V>>
      nonstd::optional<B> deserializeField(B block, V B::*member,
                                           const D& document,
                                           const std::string& field,
                                           bool (rapidjson::Value::*verify)()
                                               const,
                                           T (rapidjson::Value::*get)() const,
                                           Transform transform = Transform()) {
        return deserializeField(document, field, verify, get) | transform |
                   [&block, &member](auto transformed) -> nonstd::optional<B> {
          block.*member = transformed;
          return block;
        };
      }

      template <typename T, typename V, typename B, typename D,
          typename Transform = Transform<T, V>>
      nonstd::optional<B> deserializeOptionalField(B block, V B::*member,
                                           const D& document,
                                           const std::string& field,
                                           bool (rapidjson::Value::*verify)()
                                           const,
                                           T (rapidjson::Value::*get)() const,
                                           Transform transform = Transform()) {
        auto optfield = deserializeField(document, field, verify, get);
        if (optfield) {
          optfield | transform | [&block, &member](auto transformed) {
            block.*member = transformed;
          };
        }
        return block;
      }

      template <typename T, typename V, typename B, typename D,
          typename Transform = Transform<T, V>>
      optional_ptr<B> deserializeField(std::shared_ptr<B> block, V B::*member,
                                           const D& document,
                                           const std::string& field,
                                           bool (rapidjson::Value::*verify)()
                                           const,
                                           T (rapidjson::Value::*get)() const,
                                           Transform transform = Transform()) {
        return deserializeField(document, field, verify, get) | transform |
            [&block, &member](auto transformed) -> optional_ptr<B> {
              (*block).*member = transformed;
              return block;
            };
      }

      template <typename T, typename V, typename B, typename D,
          typename Transform = Transform<T, V>>
      optional_ptr<B> deserializeOptionalField(std::shared_ptr<B> block, V B::*member,
                                                   const D& document,
                                                   const std::string& field,
                                                   bool (rapidjson::Value::*verify)()
                                                   const,
                                                   T (rapidjson::Value::*get)() const,
                                                   Transform transform = Transform()) {
        auto optfield = deserializeField(document, field, verify, get);
        if (optfield) {
          optfield | transform | [&block, &member](auto transformed) {
            (*block).*member = transformed;
          };
        }
        return block;
      }

      template <typename D>
      auto deserializeSignature(const D& value) {
        return nonstd::make_optional<Signature>() |
            [&value](auto signature) {
              return deserializeField(
                  signature, &Signature::pubkey, value, "pubkey",
                  &rapidjson::Value::IsString, &rapidjson::Value::GetString);
            } | [&value](auto signature) {
          return deserializeField(
              signature, &Signature::signature, value, "signature",
              &rapidjson::Value::IsString, &rapidjson::Value::GetString);
        };
      }

      template <typename T>
      struct Transform<T, Signature> {
        auto operator()(T x) {
          return deserializeSignature(x);
        }
      };

      template <typename T>
      struct Transform<T, Block::SignaturesType> {
        auto operator()(T x) {
          return std::accumulate(x.begin(), x.end(),
                                 nonstd::make_optional<Block::SignaturesType>(),
                                 [](auto init, auto& x) {
                                   return deserializeSignature(x) |
                                          [&init](auto signature) {
                                            init.value().push_back(signature);
                                            return init;
                                          };
                                 });
        }
      };

      rapidjson::Value serializeSignature(const Signature& signature, rapidjson::Document::AllocatorType &allocator);

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
