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

#ifndef IROHA_TRANSACTION_RESPONSE_REGISTRATION_HPP
#define IROHA_TRANSACTION_RESPONSE_REGISTRATION_HPP

#include "common/class_handler.hpp"

// ----------| transaction responses |----------
#include "model/tx_responses/stateless_response.hpp"



/**
 * File contains registration for all transaction responses subclasses
 */

namespace iroha {
  namespace model {

    class TransactionResponseRegistry {
     public:
      TransactionResponseRegistry() {
        transaction_response_handler.register_type(typeid(TransactionStatelessResponse));
      }

      ClassHandler transaction_response_handler{};
    };

  } // namespace model
} // namespace iroha

#endif //IROHA_TRANSACTION_RESPONSE_REGISTRATION_HPP
