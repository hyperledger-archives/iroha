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

#include <main/application.hpp>

#include <fstream>
#include <gflags/gflags.h>
#include <common/config.hpp>
#include <cstring>
#include <rapidjson/rapidjson.h>
#include "logger/logger.hpp"
#include <thread>

std::vector<std::string> parse_iroha_config(std::string const& iroha_conf_path);
void run_genesis_block_server();

DEFINE_string(config, "", "irohad provisioning file path. It has trusted peer's ip addresses.");

logger::Logger Log("irohad");

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  auto irohad = Irohad();

  if (!FLAGS_config.empty()) {
    // Bootstrap iroha network.

    // TODO: Check if I have a ledger already.
    // -> Abort?

    //auto loader = common::config::ConfigLoader(FLAGS_config); // Later we use it
    auto trusted_peers = parse_iroha_config(FLAGS_config);

    std::thread
    run_genesis_block_server();
  }

  irohad.run();

  return 0;
}

/**
 * shutsdown process when some error occurs.
 * @param error - error message
 */
void fatal_error(std::string const& error) {
  Log.error(error);
  exit(-1);
}

/**
 * shutsdown process if a given condition is false.
 * @param condition
 * @param error - error message
 */
void assert_fatal(bool condition, std::string const& error) {
  if (!condition) {
    fatal_error(error);
  }
}

/**
 * parse trusted peers in `iroha.conf`
 * @param iroha_conf_path
 * @return
 */
std::vector<std::string> parse_iroha_config(std::string const& iroha_conf_path) {
  std::vector<std::string> ret;
  rapidjson::Document doc;
  std::ifstream ifs_iroha(iroha_conf_path);
  rapidjson::IStreamWrapper isw(ifs_iroha);
  doc.ParseStream(isw);
  assert_fatal(doc.HasParseError(), "JSON Parse error: " + FLAGS_config);

  const char* MemberIp = "ip";
  assert_fatal(doc.HasMember(MemberIp), "In '" + FLAGS_config + "', member '" +
                                        std::string(MemberIp) +
                                        "' doesn't exist.");
  for (const auto& ip : doc["ip"].GetArray()) {
    assert_fatal(ip.IsString(),
                 "'" + std::string(MemberIp) + "' has not string value.");
    ret.push_back(ip.GetString());
  }
  return ret;
}

void run_genesis_block_server() {
  
}