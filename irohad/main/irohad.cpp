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
#include <cstring>
#include <fstream>
#include <thread>
#include "common/assert_config.hpp"
#include "common/config.hpp"
#include "main/application.hpp"
#include "main/genesis_block_server/genesis_block_processor_impl.hpp"
#include "main/genesis_block_server/genesis_block_server.hpp"

rapidjson::Document parse_iroha_config(std::string const& iroha_conf_path);
bool validate_config(const char*, std::string const& path) {
  return !path.empty();
}

DEFINE_string(config, "", "Specify iroha provisioning path.");
DEFINE_validator(config, &validate_config);

namespace config_members {
  const char* Ip = "ip";
  const char* BlockStorePath = "block_store_path";
  // const char* ToriiPort = "torii_port"; // TODO: Needs AddPeer.
  const char* KeyPairPath = "key_pair_path";
  const char* PgOpt = "pg_opt";
  const char* RedisHost = "redis_host";
  const char* RedisPort = "redis_port";
}  // namespace config_members

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  auto config = parse_iroha_config(FLAGS_config);

  auto irohad = Irohad(config[config_members::BlockStorePath].GetString(),
                       config[config_members::RedisHost].GetString(),
                       config[config_members::RedisPort].GetUint(),
                       config[config_members::PgOpt].GetString());

  // TODO: Check if I have a ledger already.

  iroha::GenesisBlockProcessorImpl gen_proc(*irohad.storage);

  // shuts down automatically when received genesis block
  iroha::GenesisBlockServerRunner(gen_proc).run("0.0.0.0",
                                                iroha::GenesisBlockServicePort);

  // runs iroha
  irohad.run();

  return 0;
}

/**
 * parse trusted peers in `iroha.conf`
 * @param iroha_conf_path
 * @return rapidjson::Document
 */
rapidjson::Document parse_iroha_config(std::string const& iroha_conf_path) {
  using namespace assert_config;
  namespace mbr = config_members;
  rapidjson::Document doc;
  std::ifstream ifs_iroha(iroha_conf_path);
  rapidjson::IStreamWrapper isw(ifs_iroha);
  doc.ParseStream(isw);
  assert_fatal(!doc.HasParseError(), "JSON parse error: " + iroha_conf_path);

  assert_fatal(doc.HasMember(mbr::Ip), no_member_error(mbr::Ip));
  assert_fatal(doc[mbr::Ip].IsArray(), type_error(mbr::Ip, "array"));
  auto json_ips = doc[mbr::Ip].GetArray();
  for (auto iter = json_ips.begin(); iter != json_ips.end(); ++iter) {
    assert_fatal(iter->IsString(), type_error("a member of " + std::string(mbr::Ip), "string"));
  }

  assert_fatal(doc.HasMember(mbr::BlockStorePath),
               no_member_error(mbr::BlockStorePath));
  assert_fatal(doc[mbr::BlockStorePath].IsString(),
               type_error(mbr::BlockStorePath, "string"));

  assert_fatal(doc.HasMember(mbr::KeyPairPath),
               no_member_error(mbr::KeyPairPath));
  assert_fatal(doc[mbr::KeyPairPath].IsString(),
               type_error(mbr::KeyPairPath, "string"));

  assert_fatal(doc.HasMember(mbr::PgOpt), no_member_error(mbr::PgOpt));
  assert_fatal(doc[mbr::PgOpt].IsString(), type_error(mbr::PgOpt, "string"));

  assert_fatal(doc.HasMember(mbr::RedisHost), no_member_error(mbr::RedisHost));
  assert_fatal(doc[mbr::RedisHost].IsString(),
               type_error(mbr::RedisHost, "string"));

  assert_fatal(doc.HasMember(mbr::RedisPort), no_member_error(mbr::RedisPort));
  assert_fatal(doc[mbr::RedisPort].IsUint(),
               type_error(mbr::RedisPort, "uint"));
  return doc;
}
