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

#include "client.hpp"
#include <utility>
#include "model/converters/json_common.hpp"
#include "model/converters/json_query_factory.hpp"
#include "model/converters/json_transaction_factory.hpp"
#include "model/converters/pb_query_factory.hpp"
#include "model/converters/pb_transaction_factory.hpp"

namespace iroha_cli {

  CliClient::CliClient(std::string target_ip, int port)
      : command_client_(target_ip, port), query_client_(target_ip, port) {}

  CliClient::Response<CliClient::TxStatus> CliClient::sendTx(
      std::string json_tx) {
    CliClient::Response<CliClient::TxStatus> response;
    iroha::model::converters::JsonTransactionFactory serializer;
    auto doc = iroha::model::converters::stringToJson(std::move(json_tx));
    if (not doc.has_value()) {
      response.status = grpc::Status::OK;
      response.answer = WRONG_FORMAT;
      return response;
    }
    auto tx_opt = serializer.deserialize(doc.value());
    if (not tx_opt.has_value()) {
      response.status = grpc::Status::OK;
      response.answer = WRONG_FORMAT;
      return response;
    }
    auto model_tx = tx_opt.value();
    // Convert to protobuf
    iroha::model::converters::PbTransactionFactory factory;
    auto pb_tx = factory.serialize(model_tx);
    // Send to iroha:
    iroha::protocol::ToriiResponse toriiResponse;
    response.status = command_client_.Torii(pb_tx, toriiResponse);
    response.answer = toriiResponse.validation() ==
                              iroha::protocol::STATELESS_VALIDATION_SUCCESS
                          ? OK
                          : NOT_VALID;
    return response;
  }

  CliClient::Response<iroha::protocol::QueryResponse> CliClient::sendQuery(
      std::string json_query) {
    CliClient::Response<iroha::protocol::QueryResponse> response;
    iroha::model::converters::JsonQueryFactory serializer;

    auto query_opt = serializer.deserialize(std::move(json_query));

    iroha::protocol::QueryResponse query_response;

    if (not query_opt) {
      iroha::protocol::ErrorResponse er;
      er.set_reason(iroha::protocol::ErrorResponse::WRONG_FORMAT);
      query_response.mutable_error_response()->CopyFrom(er);
      response.status = grpc::Status::OK;
      response.answer = query_response;
      return response;
    }
    iroha::model::converters::PbQueryFactory proto_serializer;
    auto pb_query = proto_serializer.serialize(query_opt);
    response.status = query_client_.Find(pb_query.value(), query_response);
    response.answer = query_response;
    return response;
  }

};  // namespace iroha_cli
