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
#include "common/blob_converter.hpp"

namespace iroha {
  namespace main {

    BlockInserter::BlockInserter(std::shared_ptr<ametsuchi::Storage> storage)
        : storage_(std::move(storage)) {
    };

    nonstd::optional<model::Block> BlockInserter::parseBlock(std::string &data) {
      auto blob = common::convert(data);
      ametsuchi::BlockSerializer serializer;
      return serializer.deserialize(blob);
    };

    void BlockInserter::applyToLedger(std::vector<model::Block> &blocks) {
      auto mutable_storage = storage_->createMutableStorage();
      for (auto &&block : blocks) {
        mutable_storage->apply(block,
                               [](const auto &,
                                  auto &,
                                  auto &,
                                  const auto &) {
                                 return true;
                               });
      }
      storage_->commit(std::move(mutable_storage));
    };

    nonstd::optional<std::string> BlockInserter::loadFile(std::string &path) {
      std::ifstream file(path);
      std::string str((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
      return str;
    };

  } // namespace main
} // namespace iroha