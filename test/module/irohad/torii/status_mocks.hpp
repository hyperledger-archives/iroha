/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_STATUS_MOCKS_HPP
#define IROHA_STATUS_MOCKS_HPP

#include <gmock/gmock.h>
#include "interfaces/iroha_internal/tx_status_factory.hpp"

// -------------------------| Concrete data type mocks |------------------------

class StatefulFailedTxResponseMock
        : public shared_model::interface::StatefulFailedTxResponse {};

class StatefulValidTxResponseMock
        : public shared_model::interface::StatefulValidTxResponse {};

class CommittedTxResponseMock
        : public shared_model::interface::CommittedTxResponse {};

// -----------------------------| Container mock |------------------------------
std::ostream &operator<<(
        std::ostream &out,
        const shared_model::interface::TransactionResponse::ResponseVariantType
        &rhs) {
  return out;
}

class TransactionResponseMock
        : public shared_model::interface::TransactionResponse {
public:
  MOCK_CONST_METHOD0(transactionHash, const TransactionHashType &());
  MOCK_CONST_METHOD0(get, const ResponseVariantType &());
  MOCK_CONST_METHOD0(errorMessage, const ErrorMessageType &());

  friend std::ostream &operator<<(
          std::ostream &out,
          const shared_model::interface::TransactionResponse::ResponseVariantType
          &rhs);
};

// ------------------------------| Factory mock |-------------------------------

class TxStatusFactoryMock : public shared_model::interface::TxStatusFactory {
public:
  // --------------------------| Stateless statuses |---------------------------
  MOCK_METHOD2(makeStatelessFail,
          FactoryReturnType(TransactionHashType, ErrorMessageType));
  MOCK_METHOD2(makeStatelessValid,
          FactoryReturnType(TransactionHashType, ErrorMessageType));

  // --------------------------| Stateful statuses |----------------------------
  MOCK_METHOD2(makeStatefulFail,
          FactoryReturnType(TransactionHashType, ErrorMessageType));
  MOCK_METHOD2(makeStatefulValid,
          FactoryReturnType(TransactionHashType, ErrorMessageType));

  // ----------------------------| Final statuses |-----------------------------
  MOCK_METHOD2(makeCommitted,
          FactoryReturnType(TransactionHashType, ErrorMessageType));
  MOCK_METHOD2(makeRejected,
          FactoryReturnType(TransactionHashType, ErrorMessageType));

  // ----------------------------| Rest statuses |------------------------------
  MOCK_METHOD2(makeMstExpired,
          FactoryReturnType(TransactionHashType, ErrorMessageType));
  MOCK_METHOD2(makeNotReceived,
          FactoryReturnType(TransactionHashType, ErrorMessageType));
  MOCK_METHOD2(makeEnoughSignaturesCollected,
          FactoryReturnType(TransactionHashType, ErrorMessageType));
};

#endif //IROHA_STATUS_MOCKS_HPP
