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

#include "backend/protobuf/commands/proto_command.hpp"

#include <gtest/gtest.h>

// Quite hacky way to extract the class name
// probably not stable and should be improved
class ClassNameVisitor : public boost::static_visitor<const std::string> {
 public:
  template <typename T>
  const std::string operator()(const T &t) const {
    auto s = t->toString();
    // skip everything except class name
    return std::string(s.begin(), s.begin() + s.find(':'));
  }
};

class ProtoCommand : public testing::Test {
 public:
  void SetUp() override {}
  template <typename T>
  void SetUp(T &&command) {
    (r.*command)();
    proto = std::make_shared<shared_model::proto::Command>(r);
  }

  iroha::protocol::Command r;
  std::shared_ptr<shared_model::proto::Command> proto;
  ClassNameVisitor visitor;
};

/**
 * The following test ensures that the command is deserialized properly
 */

TEST_F(ProtoCommand, AddAssetQuantityLoad) {
  SetUp(&iroha::protocol::Command::mutable_add_asset_quantity);
  ASSERT_STREQ("AddAssetQuantity",
               boost::apply_visitor(visitor, proto->get()).c_str());
}
