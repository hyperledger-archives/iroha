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

#ifndef IROHA_MUTABLESTATE_HPP
#define IROHA_MUTABLESTATE_HPP

#include <ametsuchi/query_api.hpp>
#include <dao/block.hpp>
#include <dao/transaction.hpp>

namespace iroha {

  namespace ametsuchi {

    class StorageException;

    /**
     * Reflects a temporary state of storage
     * Allows queries on the temporary state
     */
    class MutableState : public QueryApi {

      /**
       * The context used in @see apply transaction method
       */
      class TransactionContext {
        /**
         * Attempts to apply a command to current mutable state
         * Throws exception in case of internal error
         * @tparam T Command type
         * @param command Command to be applied
         * @throws StorageException
         */
        template<class T>
        virtual void try_apply(T command) = 0;
      };

      /**
       * Applies a block to current mutable state
       * @param block Block to be applied
       */
      virtual void apply(dao::Block block) = 0;

      /**
       * Applies a transaction to current mutable state
       * using logic specified in function
       * @param transaction Transaction to be applied
       * @param function Function that specifies the logic used to apply
       * @return True if transaction was successfully applied, false otherwise
       */
      virtual bool apply(dao::Transaction transaction,
                         std::function<void(dao::Transaction,
                                            TransactionContext)> function) = 0;
    };

  }  // namespace ametsuchi

} // namespace iroha

#endif //IROHA_MUTABLESTATE_HPP
