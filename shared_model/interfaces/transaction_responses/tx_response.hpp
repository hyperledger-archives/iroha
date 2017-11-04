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

#ifndef IROHA_TX_RESPONSE_HPP
#define IROHA_TX_RESPONSE_HPP

#include <boost/variant.hpp>
#include "interfaces/hashable.hpp"
#include "interfaces/polymorphic_wrapper.hpp"
#include "interfaces/transaction_responses/committed_tx_response.hpp"
#include "interfaces/transaction_responses/stateful_failed_tx_response.hpp"
#include "interfaces/transaction_responses/stateful_valid_tx_response.hpp"
#include "interfaces/transaction_responses/stateless_failed_tx_response.hpp"
#include "interfaces/transaction_responses/stateless_valid_tx_response.hpp"
#include "interfaces/transaction_responses/unknown_tx_response.hpp"
#include "interfaces/visitor_apply_for_all.hpp"
#include "model/transaction_response.hpp"

namespace shared_model {
  namespace interface {
    /**
     * Architecture note: TransactionResponse inherit from hashable, because tx
     * response should hold hash of transaction for what it attached.
     */
    class TransactionResponse
        : public Hashable<TransactionResponse,
                          iroha::model::TransactionResponse> {
     private:
      /// PolymorphicWrapper shortcut type
      template <typename Value>
      using w = detail::PolymorphicWrapper<Value>;

     public:
      /// Type of variant, that handle all concrete tx responses in the system
      using ResponseVariantType = boost::variant<w<CommittedTxResponse>,
                                                 w<StatefulFailedTxResponse>,
                                                 w<StatefulValidTxResponse>,
                                                 w<StatelessFailedTxResponse>,
                                                 w<StatelessValidTxResponse>,
                                                 w<UnknownTxResponse>>;

      /// Type with list of types in ResponseVariantType
      using ResponseListType = ResponseVariantType::types;

      /**
       * @return attached concrete tx response
       */
      virtual const ResponseVariantType &get() const = 0;

      // ------------------------| Primitive override |-------------------------

      std::string toString() const override {
        return boost::apply_visitor(detail::ToStringVisitor(), get());
      }

      OldModelType *makeOldModel() const override {
        auto response = boost::apply_visitor(
            detail::OldModelCreatorVisitor<OldModelType *>(), get());
        response->tx_hash = hash().blob();
        return response;
      }

      bool operator==(const ModelType &rhs) const override {
        return hash() == rhs.hash() and get() == rhs.get();
      }
    };
  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_TX_RESPONSE_HPP
