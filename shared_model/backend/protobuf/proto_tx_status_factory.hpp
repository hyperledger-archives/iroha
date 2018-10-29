/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_PROTO_TX_STATUS_FACTORY_HPP
#define IROHA_PROTO_TX_STATUS_FACTORY_HPP

#include "backend/protobuf/transaction_responses/proto_tx_response.hpp"
#include "interfaces/iroha_internal/tx_status_factory.hpp"

namespace shared_model {
  namespace proto {
    class ProtoTxStatusFactory : public interface::TxStatusFactory {
     public:
      using FactoryReturnType = interface::TxStatusFactory::FactoryReturnType;

      // ------------------------| Stateless statuses |-------------------------

      FactoryReturnType makeStatelessFail(TransactionHashType,
                                          ConstRefErrorMessage) override;

      FactoryReturnType makeStatelessValid(TransactionHashType,
                                           ConstRefErrorMessage) override;

      // ------------------------| Stateful statuses |--------------------------

      FactoryReturnType makeStatefulFail(TransactionHashType,
                                         ConstRefErrorMessage) override;
      FactoryReturnType makeStatefulValid(TransactionHashType,
                                          ConstRefErrorMessage) override;

      // --------------------------| Final statuses |---------------------------

      FactoryReturnType makeCommitted(TransactionHashType,
                                      ConstRefErrorMessage) override;

      FactoryReturnType makeRejected(TransactionHashType,
                                     ConstRefErrorMessage) override;

      // --------------------------| Rest statuses |----------------------------

      FactoryReturnType makeMstExpired(TransactionHashType,
                                       ConstRefErrorMessage) override;

      FactoryReturnType makeNotReceived(TransactionHashType,
                                        ConstRefErrorMessage) override;

      FactoryReturnType makeEnoughSignaturesCollected(
          TransactionHashType, ConstRefErrorMessage) override;

      FactoryReturnType makeMstPending(TransactionHashType,
                                       ConstRefErrorMessage) override;
    };
  }  // namespace proto
}  // namespace shared_model
#endif  // IROHA_PROTO_TX_STATUS_FACTORY_HPP
