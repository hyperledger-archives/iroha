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

#ifndef IROHA_JSON_PROTO_CONVERTER_HPP
#define IROHA_JSON_PROTO_CONVERTER_HPP

#include <google/protobuf/util/json_util.h>
#include <string>
#include "backend/protobuf/commands/proto_command.hpp"
#include "commands.pb.h"

namespace shared_model {
  namespace converters {
    namespace protobuf {

      /**
       * Converts protobuf model object into json string
       * @tparam T is the type of converting command
       * @param command is the command to be converted
       * @return json string
       */
      template <typename T>
      std::string modelToJson(T command) {
        std::string result;
        google::protobuf::util::MessageToJsonString(command.getTransport(),
                                                    &result);
        return result;
      }

      /**
       * Converts json string into arbitrary protobuf object
       * @tparam T type of model which json converts to
       * @param json is the json string
       * @return optional of protobuf object which contains value if json
       * conversion was successful and none otherwise
       */
      template <typename T>
      boost::optional<T> jsonToModel(std::string json) {
        T result;
        auto status =
            google::protobuf::util::JsonStringToMessage(json, &result);
        if (not status.ok()) {
          return boost::none;
        }
        return result;
      }

      /**
       * Converts json into transaction shared model object
       * @param json is the json string containing transaction
       * @return optional of shared model transaction object, containing the
       * object if conversion was successful and none otherwise
       */
      boost::optional<shared_model::proto::Transaction> jsonToTransaction(
          std::string json) {
        auto tx = jsonToModel<iroha::protocol::Transaction>(json);
        if (tx) {
          return shared_model::proto::Transaction(
              iroha::protocol::Transaction(tx.value()));
        } else {
          return boost::none;
        }
      }

    }  // namespace protobuf
  }    // namespace converters
}  // namespace shared_model

#endif  // IROHA_JSON_PROTO_CONVERTER_HPP
