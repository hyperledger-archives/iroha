/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <gflags/gflags.h>
#include <grpc++/grpc++.h>
#include <rapidjson/rapidjson.h>
#include <fstream>
#include <thread>
#include "common/config.hpp"
#include "main/application.hpp"
#include "main/genesis_block_server/genesis_block_processor.hpp"
#include "main/iroha_conf_loader.hpp"
#include "main/raw_block_insertion.hpp"

bool validate_config(const char *flag_name, std::string const &path) {
  return not path.empty();
}

bool validate_genesis_path(const char *flag_name, std::string const &path) {
  return not path.empty();
}

DEFINE_string(config, "", "Specify iroha provisioning path.");
DEFINE_validator(config, &validate_config);

DEFINE_string(genesis_block, "genesis.json", "Specify file with initial block");
DEFINE_validator(genesis_block, &validate_genesis_path);

int main(int argc, char *argv[]) {
  namespace mbr = config_members;

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  auto config = parse_iroha_config(FLAGS_config);
  auto irohad =
      Irohad(config[mbr::BlockStorePath].GetString(),
             config[mbr::RedisHost].GetString(),
             config[mbr::RedisPort].GetUint(),
             config[mbr::PgOpt].GetString());

  iroha::main::BlockInserter insertor(irohad.storage);
  auto block = insertor.parseBlock(FLAGS_genesis_block);
  if (block.has_value()) {
    insertor.appyToLedger({block.value()});
  }

  // runs iroha
  irohad.run();

  return 0;
}
