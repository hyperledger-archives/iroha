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

#include "main/raw_block_insertion.hpp"
#include "ametsuchi/block_serializer.hpp"
#include <fstream>
#include <utility>
#include "common/types.hpp"

namespace iroha {
  namespace main {

    BlockInserter::BlockInserter(std::shared_ptr<ametsuchi::MutableFactory> factory)
        : factory_(std::move(factory)) {
    };

    nonstd::optional<model::Block> BlockInserter::parseBlock(std::string data) {
      auto blob = stringToBytes(data);
      ametsuchi::BlockSerializer serializer;
      return serializer.deserialize(blob);
    };

    void BlockInserter::applyToLedger(std::vector<model::Block> blocks) {
      auto storage = factory_->createMutableStorage();

      std::cout << "mutable storage created" << std::endl;

      for (auto &&block : blocks) {
        auto result = storage->apply(block,
                               [](const auto& current_block, auto& executor,
                                  auto& query, const auto& top_hash) {
                                 for (const auto& tx : current_block.transactions) {
                                   for (const auto& command : tx.commands) {
                                     if (not command->execute(query, executor)) {
                                       std::cout << "command execution failed" << std::endl;
                                       return false;
                                     }
                                   }
                                 }
                                 return true;
                               });
        std::cout << "block application result: " << result << std::endl;
      }

      std::cout << "blocks applied" << std::endl;

      factory_->commit(std::move(storage));

      std::cout << "storage committed" << std::endl;
    };

    nonstd::optional<std::string> BlockInserter::loadFile(std::string path) {
      std::ifstream file(path);
      std::string str((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
      return str;
    };

  } // namespace main
} // namespace iroha