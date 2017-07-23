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

#include "validation/impl/chain_validator_impl.hpp"

namespace iroha {
  namespace validation {

    bool ChainValidatorImpl::validate_block(
        const model::Block& block, ametsuchi::MutableStorage& storage) {
      auto apply_block = [](const auto& current_block, auto& executor,
                            auto& query, auto& top_block) {
        for (const auto& tx : current_block.transactions) {
          for (const auto& command : tx.commands) {
            if (!command->execute(query, executor)) {
              return false;
            }
          }
        }
        return true;
      };
      // TODO: check super-majority and crypto
      return storage.apply(block, apply_block);
    }

    bool ChainValidatorImpl::validate_chain(
        rxcpp::observable<model::Block>& blocks,
        ametsuchi::MutableStorage& storage) {
      auto result = false;
      blocks
          .take_while([&result, this, &storage](auto block) {
            return this->validate_block(block, storage);
          })
          .subscribe([](auto block) {});
      return result;
    }
  }
}