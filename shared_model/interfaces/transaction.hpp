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

#ifndef IROHA_TRANSACTION_HPP
#define IROHA_TRANSACTION_HPP

#include <unordered_set>
#include <vector>
#include "interfaces/commands/base_command.hpp"
#include "interfaces/common_objects/signature.hpp"
#include "interfaces/primitive.hpp"

namespace shared_model {
  namespace interface {

    class Transaction : public Primitive {
      using SignaturesType = std::unordered_set<Signature>;
      // TODO rework with immutable collection
      virtual SignaturesType &signatures() const = 0;

      using TimestampType = uint64_t;
      // TODO rework with time type
      virtual TimestampType created_time() const = 0;

      using CreatorIdType = std::string;
      // TODO rework with immutable
      virtual CreatorIdType &creator_account_id() const = 0;

      using TxCounterType = uint64_t;
      virtual TxCounterType transaction_counter() const = 0;

      // TODO replace with boost::variant<commands...>
      using CommnadsType = std::vector<BaseCommand>;
      // TODO rework with immutable collection
      virtual CommnadsType &commands() const = 0;

     private:
      bool equals(const Primitive &primitive) const override {
        // TODO implement as model comparison (without signatures)
        // TODO cond. no reason to implement function in lower cast classes
        return true;
      }
    };

  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_TRANSACTION_HPP
