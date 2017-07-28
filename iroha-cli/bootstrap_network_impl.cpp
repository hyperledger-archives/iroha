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

#include "bootstrap_network_impl.hpp"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/rapidjson.h>
#include <fstream>
#include <iostream>
#include "ametsuchi/block_serializer.hpp"
#include "assert_utils.hpp"
#include "genesis_block_client.hpp"
#include "genesis_block_service.hpp"
#include "model/model_hash_provider_impl.hpp"

namespace iroha_cli {

  /**
   * parse trusted peer's ip addresses in `target.conf`
   * @param target_conf_path
   * @return trusted peers' ip
   */
  std::vector<std::string> BootstrapNetworkImpl::parse_trusted_peers(
      std::string const &target_conf_path) {
    std::vector<std::string> ret;
    std::ifstream ifs(target_conf_path);
    assert_fatal(ifs.is_open(), "Cannot open: '" + target_conf_path + "'");

    // loads target.conf into rapidjson::Doc
    rapidjson::Document doc;
    rapidjson::IStreamWrapper isw(ifs);
    doc.ParseStream(isw);
    assert_fatal(doc.HasParseError(), "JSON Parse error: " + target_conf_path);

    const char *MemberIp = "ip";
    assert_fatal(doc.HasMember(MemberIp),
                 "In '" + target_conf_path + "', member '" +
                     std::string(MemberIp) + "' doesn't exist.");

    for (const auto &ip : doc["ip"].GetArray()) {
      assert_fatal(ip.IsString(),
                   "'" + std::string(MemberIp) + "' has not string value.");
      ret.push_back(ip.GetString());
    }
    return ret;
  }

  /**
   * parse transactions in genesis block `genesis.json`
   * @param genesis_json_path
   * @return iroha::model::Block
   */
  iroha::model::Block BootstrapNetworkImpl::parse_genesis_block(
      std::string const &genesis_json_path) {
    std::ifstream ifs(genesis_json_path);
    assert_fatal(ifs.is_open(), "Cannot open: '" + genesis_json_path + "'");

    rapidjson::Document doc;
    rapidjson::IStreamWrapper isw(ifs);
    doc.ParseStream(isw);

    // validate doc
    assert_fatal(doc.HasParseError(), genesis_json_path + ": parse error");
    assert_fatal(doc.IsObject(), "JSON is not object.");

    const char *MemberTxs = "transactions";
    assert_fatal(doc.HasMember(MemberTxs),
                 "No member '" + std::string(MemberTxs) + "'");
    assert_fatal(doc[MemberTxs].IsArray(),
                 std::string(MemberTxs) + " is not array.");

    // parse transactions
    auto block_serializer = iroha::ametsuchi::BlockSerializer();
    auto txs = block_serializer.deserialize_transactions(doc);
    assert_fatal(txs.has_value(), "Failed to deserialize transaction");

    // create block
    iroha::model::Block block;
    block.transactions = *txs;
    block.height = 1;
    block.prev_hash.fill(0);
    block.txs_number =
        static_cast<decltype(block.txs_number)>(block.transactions.size());

    // block hash should be calculated after all members are assigned.
    auto hash_provider = iroha::model::HashProviderImpl();
    block.hash = hash_provider.get_hash(block);

    return block;
  }

  /**
   * aborts bootstrapping network.
   * @param trusted_peers
   * @param block
   */
  void BootstrapNetworkImpl::abort_network(
      std::vector<std::string> const &trusted_peers,
      iroha::model::Block const &block) {
    for (const auto &ip : trusted_peers) {
      iroha_cli::GenesisBlockClient client(ip, iroha::GenesisBlockServicePort);
      client.SendAbortGenesisBlock(block);
    }
  }

  /**
   * bootstraps network of trusted peers.
   */
  void BootstrapNetworkImpl::run_network(
      std::vector<std::string> const &trusted_peers,
      iroha::model::Block const &genesis_block) {
    // send block to trusted peers.
    for (const auto &ip : trusted_peers) {
      iroha_cli::GenesisBlockClient client(ip, iroha::GenesisBlockServicePort);
      iroha::protocol::ApplyGenesisBlockResponse response;
      auto stat = client.SendGenesisBlock(genesis_block, response);
      if (!stat.ok() || response.applied() == iroha::protocol::APPLY_FAILURE) {
        abort_network(trusted_peers, genesis_block);
        assert_fatal(false,
                     "Failure of creating genesis block in Ip: '" + ip + "'");
      }
    }
  }
}  // namespace iroha_cli
