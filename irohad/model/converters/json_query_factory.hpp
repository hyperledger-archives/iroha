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

#ifndef IROHA_JSON_QUERY_FACTORY_HPP
#define IROHA_JSON_QUERY_FACTORY_HPP

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <memory>
#include <nonstd/optional.hpp>
#include <unordered_map>
#include "model/query.hpp"
#include "queries.pb.h"

#include "logger/logger.hpp"

namespace iroha {
  namespace model {
    namespace converters {
      using namespace rapidjson;

      class JsonQueryFactory {
       public:
        JsonQueryFactory();

        /**
         * get query from string json
         * @param query_json string representation of query
         * @return deserialized query
         */
        nonstd::optional<iroha::protocol::Query> deserialize(
            const std::string query_json);

       private:
        using Deserializer = bool (JsonQueryFactory::*)(
            GenericValue<UTF8<char>>::Object &, iroha::protocol::Query &);

        std::unordered_map<std::string, Deserializer> deserializers_;

        bool deserializeGetAccount(GenericValue<UTF8<char>>::Object &obj_query,
                                   iroha::protocol::Query &pb_query);

        bool deserializeGetSignatories(
            GenericValue<UTF8<char>>::Object &obj_query,
            iroha::protocol::Query &pb_query);

        bool deserializeGetAccountTransactions(
            GenericValue<UTF8<char>>::Object &obj_query,
            iroha::protocol::Query &pb_query);

        bool deserializeGetAccountAssetTransactions(
            GenericValue<UTF8<char>>::Object &obj_query,
            iroha::protocol::Query &pb_query);

        bool deserializeGetAccountAssets(
            GenericValue<UTF8<char>>::Object &obj_query,
            iroha::protocol::Query &pb_query);
        // Logger
        std::shared_ptr<spdlog::logger> log_;

      };
    }  // namespace converters
  }    // namespace model
}  // namespace iroha

#endif  // IROHA_JSON_QUERY_FACTORY_HPP
