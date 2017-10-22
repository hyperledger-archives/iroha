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

#include "interactive/interactive_custom_action_cli.hpp"
#include <client.hpp>

namespace iroha_cli {
  namespace interactive {

    InteractiveCustomActionCli::InteractiveCustomActionCli() {
      createActionsMenu();
      createResultMenu();
    }

    void InteractiveCustomActionCli::createActionsMenu() {
      descriptionMap_ = {{GET_TX_INFO, "Get status of transaction"}};
      const auto tx_id = "Requested tx hash";

      requestParamsDescriptions_ = {{GET_TX_INFO, {tx_id}}};
      actionHandlers_ = {
          {GET_TX_INFO, &InteractiveCustomActionCli::parseGetHash}};

      menuPoints_ = formMenu(
          actionHandlers_, requestParamsDescriptions_, descriptionMap_);
      addBackOption(menuPoints_);
    }

    void InteractiveCustomActionCli::createResultMenu() {
      resultHandlers_ = {
          {SEND_CODE, &InteractiveCustomActionCli::parseSendToIroha},
          {SAVE_CODE, &InteractiveCustomActionCli::parseSaveFile}};
      resultParamsDescriptions_ = getCommonParamsMap();

      resultPoints_ = formMenu(resultHandlers_,
                               resultParamsDescriptions_,
                               getCommonDescriptionMap());
      addBackOption(resultPoints_);
    }

    void InteractiveCustomActionCli::run() {
      bool isParsing = true;
      currentContext_ = MAIN;
      printMenu("Choose action: ", menuPoints_);
      while (isParsing) {
        auto line = promtString("> ");
        switch (currentContext_) {
          case MAIN:
            isParsing = parseAction(line);
            break;
          case RESULT:
            isParsing = parseResult(line);
            break;
        }
      }
    }

    bool InteractiveCustomActionCli::parseAction(std::string &line) {
      if (isBackOption(line)) {
        return false;
      }

      auto res = handleParse<std::string>(
          this, line, actionHandlers_, requestParamsDescriptions_);
      if (not res.has_value()) {
        // Continue parsing
        return true;
      }

      txHash_ = res.value();
      currentContext_ = RESULT;
      printMenu("Tx hash is saved. Choose what to do:", resultPoints_);
      // Continue parsing
      return true;
    }

    bool InteractiveCustomActionCli::parseResult(std::string &line) {
      if (isBackOption(line)) {
        // Give up the last query and start a new one
        currentContext_ = MAIN;
        printEnd();
        printMenu("Choose action: ", menuPoints_);
        // Continue parsing
        return true;
      }

      auto res = handleParse<bool>(
          this, line, resultHandlers_, resultParamsDescriptions_);

      return not res.has_value() ? true : res.value();
    }

    bool InteractiveCustomActionCli::parseSendToIroha(ActionParams line) {
      auto address = parseIrohaPeerParams(line);
      if (not address.has_value()) {
        return true;
      }

      auto status = iroha::protocol::TxStatus::NOT_RECEIVED;
      if (iroha::hexstringToBytestring(txHash_)) {
        status = CliClient(address.value().first, address.value().second)
                     .getTxStatus(*(iroha::hexstringToBytestring(txHash_)))
                     .answer.tx_status();
      }

      std::string message;

      // not happy about this switch
      switch (status) {
        case iroha::protocol::TxStatus::STATELESS_VALIDATION_FAILED:
          message = "Transaction has not passed stateless validation.";
          break;
        case iroha::protocol::TxStatus::STATELESS_VALIDATION_SUCCESS:
          message = "Transaction has successfully passed stateless validation.";
          break;
        case iroha::protocol::TxStatus::STATEFUL_VALIDATION_FAILED:
          message = "Transaction has not passed stateful validation.";
          break;
        case iroha::protocol::TxStatus::STATEFUL_VALIDATION_SUCCESS:
          message = "Transaction has successfully passed stateful validation.";
          break;
        case iroha::protocol::TxStatus::COMMITTED:
          message = "Transaction was successfully committed.";
          break;
        case iroha::protocol::TxStatus::ON_PROCESS:
          message = "Transaction is being processed at the moment.";
          break;
        case iroha::protocol::TxStatus::NOT_RECEIVED:
          message = "Transaction was not found in the system.";
          break;
        default:
          message =
              "A problem detected while retrieving transaction status. Please "
              "try again later.";
          break;
      }

      std::cout << message << std::endl;

      printEnd();
      return false;
    }

    bool InteractiveCustomActionCli::parseSaveFile(ActionParams line) {
      std::cout << "Not implemented yet" << std::endl;
      return true;
    }

    std::string InteractiveCustomActionCli::parseGetHash(ActionParams params) {
      auto hash = params[0];
      return hash;
    }
  }
}
