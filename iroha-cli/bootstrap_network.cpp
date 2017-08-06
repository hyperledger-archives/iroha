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
#include <string>
#include <vector>
#include "model/commands/add_peer.hpp"
#include "common/assert_config.hpp"
#include "common/types.hpp"
#include "ip_tools/ip_tools.hpp"
#include "main/genesis_block_server/genesis_block_server.hpp"  // GenesisBlockServicePort
#include "model/block.hpp"
#include "model/model_hash_provider_impl.hpp"
#include "model/peer.hpp"

using namespace assert_config;

namespace iroha_cli {

  /**
   * parse trusted peers in `target.conf`
   * @param target_conf_path
   * @return std::vector<iroha::model::Peer>
   */
  std::vector<iroha::model::Peer> BootstrapNetwork::parse_trusted_peers(
      std::string const &target_conf_path) {
    std::ifstream ifs(target_conf_path);
    assert_fatal(ifs.is_open(), "Cannot open: '" + target_conf_path + "'");

    rapidjson::Document doc;
    rapidjson::IStreamWrapper isw(ifs);
    doc.ParseStream(isw);
    assert_fatal(not doc.HasParseError(), "JSON Parse error: " + target_conf_path);

    const char *MemberPeers = "peers";
    assert_fatal(doc.HasMember(MemberPeers), no_member_error(MemberPeers));
    auto json_peers = doc[MemberPeers].GetArray();

    std::vector<iroha::model::Peer> ret;
    for (const auto &json_peer : json_peers) {
      iroha::model::Peer peer;

      assert_fatal(
          json_peer.IsObject(),
          type_error("an element in" + std::string(MemberPeers), "object"));
      const char *MemberPubkey = "pubkey";
      assert_fatal(json_peer.HasMember(MemberPubkey),
                   no_member_error(MemberPubkey));
      assert_fatal(json_peer[MemberPubkey].IsString(),
                   type_error(MemberPubkey, "string"));
      const auto pkbytes =
          iroha::hex2bytes(json_peer[MemberPubkey].GetString());

      std::copy(pkbytes.begin(), pkbytes.end(), peer.pubkey.begin());

      const char *MemberIp = "ip";
      assert_fatal(json_peer.HasMember(MemberIp), no_member_error(MemberIp));
      assert_fatal(json_peer[MemberIp].IsString(),
                   type_error(MemberIp, "string"));
      const std::string address = json_peer[MemberIp].GetString();
      assert_fatal(iroha::ip_tools::isIpValid(address),
                   std::string(MemberIp) + ": '" + address + "' is invalid.");
      peer.address = address;

      ret.push_back(peer);
    }
    return ret;
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
    assert_fatal(not doc.HasParseError(), parse_error(genesis_json_path));
    assert_fatal(doc.IsObject(), type_error("JSON", "object"));

    // parse transactions
    std::vector<iroha::model::Transaction> txs;

    auto& transactions = doc["transactions"];
    for (auto it = transactions.Begin(); it != transactions.End(); ++it) {
      rapidjson::Document transaction_document;
      auto &allocator = transaction_document.GetAllocator();
      transaction_document.CopyFrom(*it, allocator);
      auto transaction = factory_.deserialize(transaction_document);
      if (not transaction) {
        // TODO log transaction parsing failure
      }
      txs.emplace_back(transaction.value());
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
   * merges trusted peers AddPeer tx with given block
   * @param block
   * @param trusted_peers
   * @return iroha::model::Block
   */
  iroha::model::Block BootstrapNetwork::merge_tx_add_trusted_peers(
      const iroha::model::Block &block,
      std::vector<iroha::model::Peer> const &trusted_peers) {
    iroha::model::Transaction add_peers_tx;
    for (const auto &peer : trusted_peers) {
      auto add_peer = std::make_shared<iroha::model::AddPeer>();
      add_peer->peer_key = peer.pubkey;
      add_peer->address = peer.address;
      add_peers_tx.commands.push_back(add_peer);
    }
    std::vector<iroha::model::Transaction> txs;
    txs.push_back(add_peers_tx);
    for (const auto &tx : block.transactions) {
      txs.push_back(tx);
    }

    auto ret = block;
    ret.transactions = txs;
    ret.txs_number = ret.transactions.size();

    iroha::model::HashProviderImpl hash_provider;
    ret.hash = hash_provider.get_hash(ret);
    return ret;
  }

  /**
   * aborts bootstrapping network.
   * @param trusted_peers
   * @param block
   */
  void BootstrapNetwork::abort_network(
      std::vector<iroha::model::Peer> const &trusted_peers,
      iroha::model::Block const &block) {
    for (const auto &peer : trusted_peers) {
      client_.set_channel(peer.address, iroha::GenesisBlockServicePort);
      client_.send_abort_genesis_block(block);
    }
  }

  /**
   * bootstraps network of trusted peers.
   */
  void BootstrapNetwork::run_network(
      std::vector<iroha::model::Peer> const &trusted_peers,
      iroha::model::Block const &genesis_block) {
    // send block to trusted peers.
    for (const auto &peer : trusted_peers) {
      client_.set_channel(peer.address, iroha::GenesisBlockServicePort);
      iroha::protocol::ApplyGenesisBlockResponse response;
      auto stat = client_.send_genesis_block(genesis_block, response);
      if (not stat.ok() || response.applied() == iroha::protocol::APPLY_FAILURE) {
        abort_network(trusted_peers, genesis_block);
        assert_fatal(false,
                     "Failure of creating genesis block. {\"address\":\"" +
                         peer.address + ":" +
                         std::to_string(iroha::GenesisBlockServicePort) +
                         "\" \"stat\":\"" + (stat.ok() ? "true" : "false") +
                         "\", \"response\":\"" +
                         (response.applied() ? "success" : "failure") + "\"}");
      }
    }
  }
}  // namespace iroha_cli
