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

#include <algorithm>
#include <string>
#include <unordered_map>

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <nonstd/optional.hpp>

#include "common/byteutils.hpp"
#include "model/block.hpp"
#include "model/common.hpp"
#include "model/signature.hpp"

namespace iroha {
  namespace model {
    namespace converters {
      /**
       * Create map get function for value retrieval by key
       * @tparam K - map key type
       * @tparam V - map value type
       * @param map - map for value retrieval
       * @return function which takes key, returns value if key exists,
       * nullopt otherwise
       */
      template <typename K, typename V>
      auto makeMap(std::unordered_map<K, V> map) {
        return [&map](auto key) -> nonstd::optional<V> {
          auto it = map.find(key);
          if (it != std::end(map)) {
            return it->second;
          }
          return nonstd::nullopt;
        };
      }

      /**
       * Functor for invoking class method by pointer to member
       * @tparam T - class for method call
       * @tparam Args - member function arguments types
       */
      template <typename T, typename... Args>
      class Invoker {
       public:
        /**
         * @param object - object of given class
         * @param args - arguments of member function
         */
        Invoker(T &object, Args &&... args) : object(object), args(args...) {}

        /**
         * Invoke function on saved object. Helper function to get
         * index sequence
         * @tparam F - function type to be called
         * @tparam Is - index sequence of arguments from tuple
         * @param f - function to be called
         * @return result of function call
         */
        template <typename F, std::size_t... Is>
        auto operator()(F f) {
          return apply(f, std::index_sequence_for<Args...>{});
        }
       private:
        /**
         * Invoke function on saved object
         * @tparam F - function type to be called
         * @tparam Is - index sequence of arguments from tuple
         * @param f - function to be called
         * @return result of function call
         */
        template <typename F, std::size_t... Is>
        auto apply(F f, std::index_sequence<Is...>) {
          return (object.*f)(std::get<Is>(args)...);
        }

        // object for function call
        T &object;
        // arguments for function call
        std::tuple<Args...> args;
      };

      /**
       * Factory method for Invoker functor
       * @tparam T - class type for method call
       * @tparam Args - member function arguments types
       * @param object - object of given class
       * @param args - arguments of member function
       * @return Invoker instance for given arguments
       */
      template <typename T, typename... Args>
      auto makeInvoker(T &object, Args &&... args) {
        return Invoker<T, Args...>(object, std::forward<Args>(args)...);
      }

      /**
       * Transform given type to optional of another type
       * @tparam T - input type
       * @tparam V - output type
       */
      template <typename T, typename V>
      struct Transform {
        auto operator()(T x) { return nonstd::optional<V>(x); }
      };

      /**
       * Transform functor which specifies output type
       * @tparam V - output type
       */
      template <typename V>
      struct Convert {
        /**
         * Transform input type to defined type
         * @tparam T - input type
         * @param x - input value
         * @return optional of output type from input value
         */
        template <typename T>
        auto operator()(T x) {
          return nonstd::optional<V>(x);
        }
      };

      /**
       * Transform given type to Block::HashType
       * @tparam T - input type
       */
      template <typename T>
      struct Transform<T, Block::HashType> {
        auto operator()(T x) {
          return hexstringToArray<Block::HashType::size()>(x);
        }
      };

      /**
       * Transform given type to Signature::SignatureType
       * @tparam T - input type
       */
      template <typename T>
      struct Transform<T, Signature::SignatureType> {
        auto operator()(T x) {
          return hexstringToArray<Signature::SignatureType::size()>(x);
        }
      };

      /**
       * Deserialize field from given document with given verification and
       * getter
       * @tparam T - getter return type
       * @tparam D - document type
       * @param document - document value for deserialization
       * @param field - field name for deserialization
       * @param verify - verification method for field
       * @param get - getter for field
       * @return deserialized field on success, nullopt otherwise
       */
      template <typename T, typename D>
      nonstd::optional<T> deserializeField(const D &document,
                                           const std::string &field) {
        if (document.HasMember(field.c_str()) and
            document[field.c_str()].template Is<T>()) {
          return document[field.c_str()].template Get<T>();
        }
        return nonstd::nullopt;
      }

      /**
       * Deserialize field from given document with given verification and
       * getter, transform the value to required type, and assign it to block
       * member
       * @tparam T - getter return type
       * @tparam V - block member type
       * @tparam B - block type
       * @tparam D - document type
       * @tparam Transform - transform function type
       * @param block - block value for member assignment
       * @param member - pointer to member in block
       * @param document - document value for deserialization
       * @param field - field name for deserialization
       * @param verify - verification method for field
       * @param get - getter for field
       * @param transform - transform function from T to V
       * @return block with deserialized member on success, nullopt otherwise
       */
      template <typename T, typename V, typename B, typename D,
                typename Transform = Transform<T, V>>
      nonstd::optional<B> deserializeField(B block, V B::*member,
                                           const D &document,
                                           const std::string &field,
                                           Transform transform = Transform()) {
        return deserializeField<T>(document, field)
            | transform
            | [&block, &member](auto transformed) {
                block.*member = transformed;
                return nonstd::make_optional(block);
              };
      }

      /**
       * Deserialize field from given document with given verification and
       * getter, transform the value to required type, and assign it to block
       * member. Block is wrapped in shared pointer
       * @tparam T - getter return type
       * @tparam V - block member type
       * @tparam B - block type
       * @tparam D - document type
       * @tparam Transform - transform function type
       * @param block - block value for member assignment
       * @param member - pointer to member in block
       * @param document - document value for deserialization
       * @param field - field name for deserialization
       * @param verify - verification method for field
       * @param get - getter for field
       * @param transform - transform function from T to V
       * @return block with deserialized member on success, nullopt otherwise
       */
      template <typename T, typename V, typename B, typename D,
                typename Transform = Transform<T, V>>
      optional_ptr<B> deserializeField(std::shared_ptr<B> block, V B::*member,
                                       const D &document,
                                       const std::string &field,
                                       Transform transform = Transform()) {
        return deserializeField<T>(document, field)
            | transform
            | [&block, &member](auto transformed) {
                (*block).*member = transformed;
                return nonstd::make_optional(block);
              };
      }

      /**
       * Functor for deserialization from given document
       * @tparam D - document type
       */
      template <typename D>
      struct FieldDeserializer {
        /**
         * @param document - document for field deserialization
         */
        explicit FieldDeserializer(const D &document) : document(document) {}

        /**
         * Create function, which will deserialize document field with given
         * verification and getter, transform the value to required type, and
         * assign it to block member
         * @tparam T - getter return type
         * @tparam V - block member type
         * @tparam B - block type
         * @tparam Transform - transform function type
         * @param member - pointer to member in block
         * @param field - field name for deserialization
         * @param verify - verification method for field
         * @param get - getter for field
         * @param transform - transform function from T to V
         * @return function, which takes block, returns block with deserialized
         * member on success, nullopt otherwise
         */
        template <typename T, typename V, typename B,
                  typename Transform = Transform<T, V>>
        auto deserialize(V B::*member, const std::string &field,
                         Transform transform = Transform()) {
          return [this, member, field, transform](auto block) {
            return deserializeField<T>(block, member, document, field,
                                       transform);
          };
        }

        /**
         * Deserialize field of Uint type to given member field of block
         * @tparam V - block member type
         * @tparam B - block type
         * @param member - pointer to member in block
         * @param field - field name for deserialization
         * @return @see deserialize
         */
        template <typename V, typename B>
        auto Uint(V B::*member, const std::string &field) {
          return deserialize<uint32_t>(member, field);
        }

        /**
         * Deserialize field of Uint64 type to given member field of block
         * @tparam V - block member type
         * @tparam B - block type
         * @param member - pointer to member in block
         * @param field - field name for deserialization
         * @return @see deserialize
         */
        template <typename V, typename B>
        auto Uint64(V B::*member, const std::string &field) {
          return deserialize<uint64_t>(member, field);
        }

        /**
         * Deserialize field of Bool type to given member field of block
         * @tparam V - block member type
         * @tparam B - block type
         * @param member - pointer to member in block
         * @param field - field name for deserialization
         * @return @see deserialize
         */
        template <typename V, typename B>
        auto Bool(V B::*member, const std::string &field) {
          return deserialize<bool>(member, field);
        }

        /**
         * Deserialize field of String type to given member field of block
         * @tparam V - block member type
         * @tparam B - block type
         * @param member - pointer to member in block
         * @param field - field name for deserialization
         * @return @see deserialize
         */
        template <typename V, typename B>
        auto String(V B::*member, const std::string &field) {
          return deserialize<std::string>(member, field);
        }

        /**
         * Deserialize field of String type
         * @param field - field name for deserialization
         * @return deserialized field on success, nullopt otherwise
         */
        auto String(const std::string &field) {
          return deserializeField<std::string>(document, field);
        }

        /**
         * Deserialize field of Array type to given member field of block
         * @tparam V - block member type
         * @tparam B - block type
         * @param member - pointer to member in block
         * @param field - field name for deserialization
         * @return @see deserialize
         */
        template <typename V, typename B>
        auto Array(V B::*member, const std::string &field) {
          return deserialize<rapidjson::Value::ConstArray>(member, field);
        }

        /**
         * Deserialize field of Array type to given member field of block,
         * using provided transform function
         * @tparam V - block member type
         * @tparam B - block type
         * @tparam Transform - transform function type
         * @param member - pointer to member in block
         * @param field - field name for deserialization
         * @param transform - transform function from Array to V
         * @return @see deserialize
         */
        template <typename V, typename B, typename Transform>
        auto Array(V B::*member, const std::string &field,
                   Transform transform) {
          return deserialize<rapidjson::Value::ConstArray>(member, field,
                                                           transform);
        }

        /**
         * Deserialize field of Object type to given member field of block
         * @tparam V - block member type
         * @tparam B - block type
         * @param member - pointer to member in block
         * @param field - field name for deserialization
         * @return @see deserialize
         */
        template <typename V, typename B>
        auto Object(V B::*member, const std::string &field) {
          return deserialize<rapidjson::Value::ConstObject>(member, field);
        }

        // document for deserialization
        const D &document;
      };

      /**
       * Factory method for FieldDeserializer functor
       * @tparam D - document type
       * @param document - document for deserialization
       * @return FieldDeserializer instance for given arguments
       */
      template <typename D>
      auto makeFieldDeserializer(const D &document) {
        return FieldDeserializer<D>(document);
      }

      /**
       * Try to deserialize signature from given document
       * @tparam D - document type
       * @param value - document object
       * @return signature on success, nullopt otherwise
       */
      template <typename D>
      auto deserializeSignature(const D &value) {
        auto des = makeFieldDeserializer(value);
        return nonstd::make_optional<Signature>()
            | des.String(&Signature::pubkey, "pubkey")
            | des.String(&Signature::signature, "signature");
      }

      /**
       * Transform given type to Signature
       * @tparam T - input type
       */
      template <typename T>
      struct Transform<T, Signature> {
        auto operator()(T x) { return deserializeSignature(x); }
      };

      /**
       * Transform given type to Block::SignaturesType
       * @tparam T - input type
       */
      template <typename T>
      struct Transform<T, Block::SignaturesType> {
        auto operator()(T x) {
          return std::accumulate(
              x.begin(), x.end(),
              nonstd::make_optional<Block::SignaturesType>(),
              [](auto init, auto &x) {
                return init
                    | [&x](auto signatures) {
                        return deserializeSignature(x)
                            | [&signatures](auto signature) {
                                signatures.push_back(signature);
                                return nonstd::make_optional(signatures);
                              };
                      };
              });
        }
      };

      /**
       * Serialize signature to JSON with given allocator
       * @param signature - signature for serialization
       * @param allocator - allocator for JSON value
       * @return
       */
      rapidjson::Value serializeSignature(
          const Signature &signature,
          rapidjson::Document::AllocatorType &allocator);

      /**
       * Try to parse JSON from string
       * @param string - string for parsing
       * @return JSON document on success, nullopt otherwise
       */
      nonstd::optional<rapidjson::Document> stringToJson(
          const std::string &string);

      /**
       * Pretty print JSON document to string
       * @param document - document for printing
       * @return pretty printed JSON document
       */
      std::string jsonToString(const rapidjson::Document &document);

      /**
       * Try to parse JSON from vector
       * @param vector - vector for parsing
       * @return JSON document on success, nullopt otherwise
       */
      nonstd::optional<rapidjson::Document> vectorToJson(
          const std::vector<uint8_t> &vector);

      /**
       * Pretty print JSON document to vector
       * @param document - document for printing
       * @return pretty printed JSON document
       */
      std::vector<uint8_t> jsonToVector(const rapidjson::Document &document);
    }  // namespace converters
  }    // namespace model
}  // namespace iroha

#endif  // IROHA_JSON_COMMON_HPP
