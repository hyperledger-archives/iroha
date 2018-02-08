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

#include <thread>

#include "ametsuchi/block_query.hpp"
#include "common/types.hpp"
#include "endpoint.pb.h"
#include "model/converters/pb_common.hpp"
#include "model/sha3_hash.hpp"
#include "torii/command_service.hpp"

using namespace std::chrono_literals;

namespace torii {

  CommandService::CommandService(
      std::shared_ptr<iroha::model::converters::PbTransactionFactory>
          pb_factory,
      std::shared_ptr<iroha::torii::TransactionProcessor> txProcessor,
      std::shared_ptr<iroha::ametsuchi::Storage> storage)
      : pb_factory_(pb_factory),
        tx_processor_(txProcessor),
        storage_(storage),
        cache_(std::make_shared<
               iroha::cache::Cache<std::string,
                                   iroha::protocol::ToriiResponse>>()) {
    // Notifier for all clients
    tx_processor_->transactionNotifier().subscribe(
        [this](
            std::shared_ptr<iroha::model::TransactionResponse> iroha_response) {
          // Find response in cache
          auto res = cache_->findItem(iroha_response->tx_hash);
          if (not res) {
            iroha::protocol::ToriiResponse response;
            response.set_tx_hash(iroha_response->tx_hash);
            response.set_tx_status(iroha::protocol::NOT_RECEIVED);
            cache_->addItem(iroha_response->tx_hash, response);
            return;
          }
          switch (iroha_response->current_status) {
            case iroha::model::TransactionResponse::STATELESS_VALIDATION_FAILED:
              res->set_tx_status(
                  iroha::protocol::TxStatus::STATELESS_VALIDATION_FAILED);
              break;
            case iroha::model::TransactionResponse::
                STATELESS_VALIDATION_SUCCESS:
              res->set_tx_status(
                  iroha::protocol::TxStatus::STATELESS_VALIDATION_SUCCESS);
              break;
            case iroha::model::TransactionResponse::STATEFUL_VALIDATION_FAILED:
              res->set_tx_status(
                  iroha::protocol::TxStatus::STATEFUL_VALIDATION_FAILED);
              break;
            case iroha::model::TransactionResponse::STATEFUL_VALIDATION_SUCCESS:
              res->set_tx_status(
                  iroha::protocol::TxStatus::STATEFUL_VALIDATION_SUCCESS);
              break;
            case iroha::model::TransactionResponse::COMMITTED:
              res->set_tx_status(iroha::protocol::TxStatus::COMMITTED);
              break;
            case iroha::model::TransactionResponse::IN_PROGRESS:
              res->set_tx_status(iroha::protocol::TxStatus::IN_PROGRESS);
              break;
            case iroha::model::TransactionResponse::NOT_RECEIVED:
            default:
              res->set_tx_status(iroha::protocol::TxStatus::NOT_RECEIVED);
              break;
          }

          cache_->addItem(iroha_response->tx_hash, *res);
        });
  }

  void CommandService::Torii(iroha::protocol::Transaction const &tx) {
    auto iroha_tx = pb_factory_->deserialize(tx);
    auto tx_hash = iroha::hash<iroha::protocol::Transaction>(tx).to_string();

    if (cache_->findItem(tx_hash)) {
      return;
    }

    iroha::protocol::ToriiResponse response;
    response.set_tx_hash(tx_hash);
    response.set_tx_status(iroha::protocol::TxStatus::IN_PROGRESS);

    cache_->addItem(tx_hash, response);
    // Send transaction to iroha
    tx_processor_->transactionHandle(iroha_tx);
  }

  grpc::Status CommandService::Torii(
      grpc::ServerContext *context,
      const iroha::protocol::Transaction *request,
      google::protobuf::Empty *response) {
    Torii(*request);
    return grpc::Status::OK;
  }

  void CommandService::Status(iroha::protocol::TxStatusRequest const &request,
                              iroha::protocol::ToriiResponse &response) {
    auto resp = cache_->findItem(request.tx_hash());
    if (resp) {
      response.CopyFrom(*resp);
    } else {
      response.set_tx_hash(request.tx_hash());
      if (storage_->getBlockQuery()->getTxByHashSync(request.tx_hash())) {
        response.set_tx_status(iroha::protocol::TxStatus::COMMITTED);
      } else {
        response.set_tx_status(iroha::protocol::TxStatus::NOT_RECEIVED);
      }
      cache_->addItem(request.tx_hash(), response);
    }
  }

  grpc::Status CommandService::Status(
      grpc::ServerContext *context,
      const iroha::protocol::TxStatusRequest *request,
      iroha::protocol::ToriiResponse *response) {
    Status(*request, *response);
    return grpc::Status::OK;
  }

  void CommandService::StatusStream(
      iroha::protocol::TxStatusRequest const &request,
      grpc::ServerWriter<iroha::protocol::ToriiResponse> &response_writer) {
    auto resp = cache_->findItem(request.tx_hash());
    if (resp) {
      if (resp->tx_status()
              == iroha::protocol::TxStatus::STATELESS_VALIDATION_FAILED
          or resp->tx_status()
              == iroha::protocol::TxStatus::STATEFUL_VALIDATION_FAILED
          or resp->tx_status() == iroha::protocol::TxStatus::NOT_RECEIVED
          or resp->tx_status() == iroha::protocol::TxStatus::COMMITTED) {
        response_writer.WriteLast(*resp, grpc::WriteOptions());
        return;
      }
      response_writer.Write(*resp);
    }

    bool finished = false;
    auto subscription = rxcpp::composite_subscription();
    auto request_hash = request.tx_hash();

    tx_processor_->transactionNotifier()
        .filter([&request_hash](auto response) {
          return response->tx_hash == request_hash;
        })
        .subscribe(
            subscription,
            [&](std::shared_ptr<iroha::model::TransactionResponse>
                    iroha_response) {
              iroha::protocol::ToriiResponse resp_sub;
              resp_sub.set_tx_hash(request_hash);

              switch (iroha_response->current_status) {
                case iroha::model::TransactionResponse::
                    STATELESS_VALIDATION_FAILED:
                  resp_sub.set_tx_status(
                      iroha::protocol::TxStatus::STATELESS_VALIDATION_FAILED);
                  response_writer.WriteLast(resp_sub, grpc::WriteOptions());
                  subscription.unsubscribe();
                  finished = true;
                  cv_.notify_one();
                  break;
                case iroha::model::TransactionResponse::
                    STATELESS_VALIDATION_SUCCESS:
                  resp_sub.set_tx_status(
                      iroha::protocol::TxStatus::STATELESS_VALIDATION_SUCCESS);
                  response_writer.Write(resp_sub);
                  break;
                case iroha::model::TransactionResponse::
                    STATEFUL_VALIDATION_FAILED:
                  resp_sub.set_tx_status(
                      iroha::protocol::TxStatus::STATEFUL_VALIDATION_FAILED);
                  response_writer.WriteLast(resp_sub, grpc::WriteOptions());
                  subscription.unsubscribe();
                  finished = true;
                  cv_.notify_one();
                  break;
                case iroha::model::TransactionResponse::
                    STATEFUL_VALIDATION_SUCCESS:
                  resp_sub.set_tx_status(
                      iroha::protocol::TxStatus::STATEFUL_VALIDATION_SUCCESS);
                  response_writer.Write(resp_sub);
                  break;
                case iroha::model::TransactionResponse::COMMITTED:
                  resp_sub.set_tx_status(iroha::protocol::TxStatus::COMMITTED);
                  response_writer.WriteLast(resp_sub, grpc::WriteOptions());
                  subscription.unsubscribe();
                  finished = true;
                  cv_.notify_one();
                  break;
                case iroha::model::TransactionResponse::IN_PROGRESS:
                  resp_sub.set_tx_status(
                      iroha::protocol::TxStatus::IN_PROGRESS);
                  response_writer.Write(resp_sub);
                  break;
                case iroha::model::TransactionResponse::NOT_RECEIVED:
                default:
                  resp_sub.set_tx_status(
                      iroha::protocol::TxStatus::NOT_RECEIVED);
                  response_writer.WriteLast(resp_sub, grpc::WriteOptions());
                  subscription.unsubscribe();
                  finished = true;
                  cv_.notify_one();
                  break;
              }
            });

    std::unique_lock<std::mutex> lock(wait_subscription_);
    // One second should be enough to at least start tx processing.
    // Otherwise we think there is no such tx at all.
    cv_.wait_for(lock, 1s);
    if (not finished) {
      if (not resp) {
        subscription.unsubscribe();
        iroha::protocol::ToriiResponse resp_none;
        resp_none.set_tx_hash(request_hash);
        resp_none.set_tx_status(iroha::protocol::TxStatus::NOT_RECEIVED);
        response_writer.WriteLast(resp_none, grpc::WriteOptions());
      } else {
        cv_.wait_for(lock, 10s);
      }
    }
  }

  grpc::Status CommandService::StatusStream(
      grpc::ServerContext *context,
      const iroha::protocol::TxStatusRequest *request,
      grpc::ServerWriter<iroha::protocol::ToriiResponse> *response_writer) {
    StatusStream(*request, *response_writer);
    return grpc::Status::OK;
  }

}  // namespace torii
