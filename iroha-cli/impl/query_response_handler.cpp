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

#include "query_response_handler.hpp"

using namespace iroha::protocol;
namespace iroha_cli {

  QueryResponseHandler::QueryResponseHandler()
      : log_(logger::log("QueryResponseHandler")) {
    handler_map_[typeid(ErrorResponse)] =
        &QueryResponseHandler::handleErrorResponse;
    handler_map_[typeid(AccountResponse)] =
        &QueryResponseHandler::handleAccountResponse;
    handler_map_[typeid(AccountAssetResponse)] =
        &QueryResponseHandler::handleAccountAssetsResponse;
    handler_map_[typeid(TransactionsResponse)] =
        &QueryResponseHandler::handleTransactionsResponse;
    handler_map_[typeid(SignatoriesResponse)] =
        &QueryResponseHandler::handleSignatoriesResponse;
  }

  void QueryResponseHandler::handle(
      const iroha::protocol::QueryResponse &response) {
    auto it = handler_map_.find(typeid(response));
    if (it != handler_map_.end()) {
      (this->*it->second)(response);
    } else {
      log_->error("Response Handle not Implemented");
    }
  }

  void QueryResponseHandler::handleErrorResponse(
      const iroha::protocol::QueryResponse &response) {
    switch (response.error_response().reason()) {
      case ErrorResponse::STATEFUL_INVALID:
        log_->error("Query is stateful invalid");
        break;
      case ErrorResponse::STATELESS_INVALID:
        log_->error("Query is stateless invalid");
        break;
      case ErrorResponse::NO_ACCOUNT:
        log_->error("Account not found");
        break;
      case ErrorResponse::NO_ACCOUNT_ASSETS:
        log_->error("Account assets not found");
        break;
      case ErrorResponse::NO_SIGNATORIES:
        log_->error("No signatories found");
        break;
      case ErrorResponse::NOT_SUPPORTED:
        log_->error("Query not supported");
        break;
      case ErrorResponse::WRONG_FORMAT:
        log_->error("Query has wrong format");
        break;
      case ErrorResponse_Reason_ErrorResponse_Reason_INT_MIN_SENTINEL_DO_NOT_USE_:
        break;
      case ErrorResponse_Reason_ErrorResponse_Reason_INT_MAX_SENTINEL_DO_NOT_USE_:
        break;
    }
  }

  void QueryResponseHandler::handleAccountResponse(
      const iroha::protocol::QueryResponse &response) {
    auto account = response.account_response().account();
    log_->info("[Account]:");
    log_->info("-Id:- {}", account.account_id());
    // TODO : print permissions
    log_->info("-Domain- {}", account.domain_name());
  }

  void QueryResponseHandler::handleAccountAssetsResponse(
      const iroha::protocol::QueryResponse &response) {
    auto acc_assets = response.account_assets_response().account_asset();
    log_->info("[Account Assets]");
    log_->info("-Account Id- {}", acc_assets.account_id());
    log_->info("-Asset Id- {}", acc_assets.asset_id());
    log_->info("-Balance- {}", acc_assets.balance());
  }

  void QueryResponseHandler::handleSignatoriesResponse(
      const iroha::protocol::QueryResponse &response) {
    auto signatories = response.signatories_response().keys();
    log_->info("[Signatories]");
    std::for_each(
        signatories.begin(), signatories.end(),
        [this](auto signatory) { log_->info("-Signatory- {}", signatory); });
  }

  void QueryResponseHandler::handleTransactionsResponse(
      const iroha::protocol::QueryResponse &response) {
    auto txs = response.transactions_response().transactions();
    log_->info("[Transactions]");
    std::for_each(txs.begin(), txs.end(), [this](auto tx) {
      log_->info("-[tx]-");
      log_->info("--[Creator Id] -- {}", tx.meta().creator_account_id());
      // TODO: add other fields
    });
  }

}  // namespace iroha_cli
