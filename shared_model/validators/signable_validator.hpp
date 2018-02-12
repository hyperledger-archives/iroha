/**
 * Copyright Soramitsu Co., Ltd. 2018 All Rights Reserved.
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

#ifndef IROHA_SHARED_MODEL_SIGNABLE_VALIDATOR_HPP
#define IROHA_SHARED_MODEL_SIGNABLE_VALIDATOR_HPP

#include "validators/query_validator.hpp"
#include "validators/transaction_validator.hpp"

namespace shared_model {
  namespace validation {

    template <typename FieldValidator, typename CommandValidator>
    class SignableTransactionValidator
        : public TransactionValidator<FieldValidator, CommandValidator> {
     public:
      SignableTransactionValidator(
          const FieldValidator &fieldValidator = FieldValidator())
          : fieldValidator_(fieldValidator),
            TransactionValidator<FieldValidator, CommandValidator>(
                fieldValidator_) {}

      Answer validate(
          detail::PolymorphicWrapper<interface::Transaction> tx) const {
        Answer answer =
            TransactionValidator<FieldValidator, CommandValidator>::validate(
                tx);
        std::string tx_reason_name = "Signature";
        ReasonsGroupType tx_reason(tx_reason_name, GroupedReasons());
        fieldValidator_.validateSignatures(
            tx_reason, tx->signatures(), tx->blob());
        if (not tx_reason.second.empty()) {
          answer.addReason(std::move(tx_reason));
        }
        return answer;
      }

     private:
      FieldValidator fieldValidator_;
    };

    template <typename FieldValidator, typename QueryFieldValidator>
    class SignableQueryValidator
        : public QueryValidator<FieldValidator, QueryFieldValidator> {
     public:
      SignableQueryValidator(
          const FieldValidator &fieldValidator = FieldValidator())
          : fieldValidator_(fieldValidator),
            QueryValidator<FieldValidator, QueryFieldValidator>(
                fieldValidator_) {}

      Answer validate(detail::PolymorphicWrapper<interface::Query> qry) const {
        Answer answer =
            QueryValidator<FieldValidator, QueryFieldValidator>::validate(qry);
        std::string qry_reason_name = "Signature";
        ReasonsGroupType qry_reason(qry_reason_name, GroupedReasons());
        fieldValidator_.validateSignatures(
            qry_reason, qry->signatures(), qry->blob());
        if (not qry_reason.second.empty()) {
          answer.addReason(std::move(qry_reason));
        }
        return answer;
      }

     private:
      FieldValidator fieldValidator_;
    };

  }  // namespace validation
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_SIGNABLE_VALIDATOR_HPP
