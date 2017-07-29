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

#include "bootstrap_network.hpp"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <iostream>
#include "ametsuchi/block_serializer.hpp"
#include "assert_utils.hpp"
#include "genesis_block_client.hpp"
#include "ip_tools/ip_tools.hpp"
#include "main/genesis_block_server/genesis_block_service.hpp"
#include "model/model_hash_provider_impl.hpp"

namespace iroha_cli {

  /**
   * parse trusted peer's ip addresses in `target.conf`
   * @param target_conf_path
   * @return trusted peers' ip
   */
  std::vector<std::string> BootstrapNetwork::parse_trusted_peers(
      std::string const &target_conf_path) {
    std::vector<std::string> ret;
    std::ifstream ifs(target_conf_path);
    assert_fatal(ifs.is_open(), "Cannot open: '" + target_conf_path + "'");

    rapidjson::Document doc;
    rapidjson::IStreamWrapper isw(ifs);
    doc.ParseStream(isw);
    assert_fatal(!doc.HasParseError(), "JSON Parse error: " + target_conf_path);

    const char *MemberIp = "ip";
    assert_fatal(doc.HasMember(MemberIp),
                 "In '" + target_conf_path + "', member '" +
                     std::string(MemberIp) + "' doesn't exist.");
    auto json_ips = doc[MemberIp].GetArray();
    for (const auto &ip : json_ips) {
      assert_fatal(ip.IsString(),
                   "'" + std::string(MemberIp) + "' has not string value.");
      const std::string ip_str = ip.GetString();
      assert_fatal(iroha::ip_tools::isIpValid(ip_str),
                   "Ip '" + ip_str + "' is invalid.");
      ret.push_back(ip_str);
    }
    return ret;
  }

  // error message helpers
  std::string no_member_error(std::string const &member) {
    return "No member '" + member + "'";
  }

  std::string type_error(std::string const &value, std::string const &type) {
    return "'" + value + "' is not " + type;
  }

  std::string parse_error(std::string const &path) {
    return "Parse error. JSON file path: " + path + "'";
  }

  void validate_command(rapidjson::Value &json_val_cmd) {
    auto json_cmd = json_val_cmd.GetObject();
    const char *MemberCommandType = "command_type";
    assert_fatal(json_cmd.HasMember(MemberCommandType),
                 no_member_error(MemberCommandType));

    // TODO: validate command.
  }

  void validate_transactions(rapidjson::Document &doc) {
    const char *MemberTxs = "transactions";
    assert_fatal(doc.HasMember(MemberTxs), no_member_error(MemberTxs));
    assert_fatal(doc[MemberTxs].IsArray(), type_error(MemberTxs, "array"));
    auto json_txs = doc[MemberTxs].GetArray();

    for (auto json_tx_iter = json_txs.begin(); json_tx_iter != json_txs.end(); ++json_tx_iter) {
      auto json_tx = json_tx_iter->GetObject();
      const char *MemberTxSigs = "signatures";
      assert_fatal(json_tx.HasMember(MemberTxSigs),
                   no_member_error(MemberTxSigs));

      auto json_sigs = json_tx[MemberTxSigs].GetArray();
      for (auto json_sigs_iter = json_sigs.begin(); json_sigs_iter != json_sigs.end(); ++json_sigs_iter) {
        assert_fatal(json_sigs_iter->GetObject().HasMember("pubkey"),
                     no_member_error("pubkey"));
        assert_fatal(json_sigs_iter->GetObject().HasMember("signature"),
                     no_member_error("signature"));
      }

      const char *MemberTxCreatedTs = "created_ts";
      // FIXME: Should iroha decide default value?
      assert_fatal(json_tx.HasMember(MemberTxCreatedTs),
                   no_member_error(MemberTxCreatedTs));

      const char *MemberTxAccountId = "creator_account_id";
      assert_fatal(json_tx.HasMember(MemberTxAccountId),
                   no_member_error(MemberTxAccountId));

      const char *MemberTxCounter = "tx_counter";
      // FIXME: Should iroha decide default value?
      assert_fatal(json_tx.HasMember(MemberTxCounter),
                   no_member_error(MemberTxCounter));

      const char *MemberTxCommands = "commands";
      assert_fatal(json_tx.HasMember(MemberTxCommands),
                   no_member_error(MemberTxCommands));

      auto json_commands = json_tx[MemberTxCommands].GetArray();
      for (auto iter = json_commands.begin(); iter != json_commands.end(); ++iter) {
        validate_command(*iter);
      }
    }
  }

  /**
   * parse transactions in genesis block `genesis.json`
   * @param genesis_json_path
   * @return iroha::model::Block
   */
  iroha::model::Block BootstrapNetwork::parse_genesis_block(
      std::string const &genesis_json_path) {
    std::ifstream ifs(genesis_json_path);
    assert_fatal(ifs.is_open(), "Cannot open: '" + genesis_json_path + "'");

    rapidjson::Document doc;
    rapidjson::IStreamWrapper isw(ifs);
    doc.ParseStream(isw);

    // validate doc
    assert_fatal(!doc.HasParseError(), parse_error(genesis_json_path));
    assert_fatal(doc.IsObject(), type_error("JSON", "object"));

    validate_transactions(doc);

    // parse transactions
    auto block_serializer = iroha::ametsuchi::BlockSerializer();
    std::vector<iroha::model::Transaction> txs;

    try {
      block_serializer.deserialize(doc, txs);
    } catch (...) {
      assert_fatal(false, "Failed to parse command");
    }

    // create block
    iroha::model::Block block;
    block.transactions = txs;
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
  void BootstrapNetwork::abort_network(
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
  void BootstrapNetwork::run_network(
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
