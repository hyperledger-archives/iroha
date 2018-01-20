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

#ifndef IROHA_CONF_LOADER_HPP
#define IROHA_CONF_LOADER_HPP

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/rapidjson.h>
#include <fstream>
#include <string>
#include "common/assert_config.hpp"

namespace config_members {
  const char *BlockStorePath = "block_store_path";
  const char *ToriiPort = "torii_port";
  const char *InternalPort = "internal_port";
  const char *KeyPairPath = "key_pair_path";
  const char *PgOpt = "pg_opt";
  const char *RedisHost = "redis_host";
  const char *RedisPort = "redis_port";
  const char *MaxProposalSize = "max_proposal_size";
  const char *ProposalDelay = "proposal_delay";
  const char *VoteDelay = "vote_delay";
  const char *LoadDelay = "load_delay";
}  // namespace config_members

/**
 * parse and assert trusted peers json in `iroha.conf`
 * @param conf_path is a path to iroha's config
 * @return rapidjson::Document is a parsed equivalent of that file
 */
inline rapidjson::Document parse_iroha_config(const std::string &conf_path) {
  using namespace assert_config;
  namespace mbr = config_members;
  rapidjson::Document doc;
  std::ifstream ifs_iroha(conf_path);
  rapidjson::IStreamWrapper isw(ifs_iroha);
  const std::string kStrType = "string";
  const std::string kUintType = "uint";
  doc.ParseStream(isw);
  assert_fatal(not doc.HasParseError(), "JSON parse error: " + conf_path);

  assert_fatal(doc.HasMember(mbr::BlockStorePath),
               no_member_error(mbr::BlockStorePath));
  assert_fatal(doc[mbr::BlockStorePath].IsString(),
               type_error(mbr::BlockStorePath, kStrType));

  assert_fatal(doc.HasMember(mbr::ToriiPort), no_member_error(mbr::ToriiPort));
  assert_fatal(doc[mbr::ToriiPort].IsUint(),
               type_error(mbr::ToriiPort, kUintType));

  assert_fatal(doc.HasMember(mbr::InternalPort),
               no_member_error(mbr::InternalPort));
  assert_fatal(doc[mbr::InternalPort].IsUint(),
               type_error(mbr::InternalPort, kUintType));

  assert_fatal(doc.HasMember(mbr::PgOpt), no_member_error(mbr::PgOpt));
  assert_fatal(doc[mbr::PgOpt].IsString(), type_error(mbr::PgOpt, kStrType));

  assert_fatal(doc.HasMember(mbr::RedisHost), no_member_error(mbr::RedisHost));
  assert_fatal(doc[mbr::RedisHost].IsString(),
               type_error(mbr::RedisHost, kStrType));

  assert_fatal(doc.HasMember(mbr::RedisPort), no_member_error(mbr::RedisPort));
  assert_fatal(doc[mbr::RedisPort].IsUint(),
               type_error(mbr::RedisPort, kUintType));

  assert_fatal(doc.HasMember(mbr::MaxProposalSize),
               no_member_error(mbr::MaxProposalSize));
  assert_fatal(doc[mbr::MaxProposalSize].IsUint(),
               type_error(mbr::MaxProposalSize, kUintType));

  assert_fatal(doc.HasMember(mbr::ProposalDelay),
               no_member_error(mbr::ProposalDelay));
  assert_fatal(doc[mbr::ProposalDelay].IsUint(),
               type_error(mbr::ProposalDelay, kUintType));

  assert_fatal(doc.HasMember(mbr::VoteDelay), no_member_error(mbr::VoteDelay));
  assert_fatal(doc[mbr::VoteDelay].IsUint(),
               type_error(mbr::VoteDelay, kUintType));

  assert_fatal(doc.HasMember(mbr::LoadDelay), no_member_error(mbr::LoadDelay));
  assert_fatal(doc[mbr::LoadDelay].IsUint(),
               type_error(mbr::LoadDelay, kUintType));
  return doc;
}

#endif  // IROHA_CONF_LOADER_HPP
