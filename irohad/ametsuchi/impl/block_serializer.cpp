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

#define RAPIDJSON_HAS_STDSTRING 1

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/reader.h>
#include <algorithm>
#include <ametsuchi/block_serializer.hpp>
#include <common/types.hpp>
#include <iostream>
#include <sstream>

namespace iroha {
  namespace ametsuchi {

    using namespace rapidjson;

    BlockSerializer::BlockSerializer() {
      serializers_[typeid(model::AddPeer)] =
          &BlockSerializer::serialize_add_peer;
      serializers_[typeid(model::AddAssetQuantity)] =
          &BlockSerializer::serialize_add_asset_quantity;
      serializers_[typeid(model::AddSignatory)] =
          &BlockSerializer::serialize_add_signatory;
      serializers_[typeid(model::AssignMasterKey)] =
          &BlockSerializer::serialize_assign_master_key;
      serializers_[typeid(model::CreateAccount)] =
          &BlockSerializer::serialize_create_account;
      serializers_[typeid(model::CreateAsset)] =
          &BlockSerializer::serialize_create_asset;
      serializers_[typeid(model::CreateDomain)] =
          &BlockSerializer::serialize_create_domain;
      serializers_[typeid(model::RemoveSignatory)] =
          &BlockSerializer::serialize_remove_signatory;
      serializers_[typeid(model::SetAccountPermissions)] =
          &BlockSerializer::serialize_set_account_permissions;
      serializers_[typeid(model::SetQuorum)] =
          &BlockSerializer::serialize_set_quorum;
      serializers_[typeid(model::TransferAsset)] =
          &BlockSerializer::serialize_transfer_asset;

      deserializers_["AddPeer"] = &BlockSerializer::deserialize_add_peer;
      deserializers_["AddAssetQuantity"] =
          &BlockSerializer::deserialize_add_asset_quantity;
      deserializers_["AddSignatory"] =
          &BlockSerializer::deserialize_add_signatory;
      deserializers_["AssignMasterKey"] =
          &BlockSerializer::deserialize_assign_master_key;
      deserializers_["CreateAccount"] =
          &BlockSerializer::deserialize_create_account;
      deserializers_["CreateAsset"] =
          &BlockSerializer::deserialize_create_asset;
      deserializers_["CreateDomain"] =
          &BlockSerializer::deserialize_create_domain;
      deserializers_["RemoveSignatory"] =
          &BlockSerializer::deserialize_remove_signatory;
      deserializers_["SetAccountPermissions"] =
          &BlockSerializer::deserialize_set_account_permissions;
      deserializers_["SetQuorum"] = &BlockSerializer::deserialize_set_quorum;
      deserializers_["TransferAsset"] =
          &BlockSerializer::deserialize_transfer_asset;
    }

    /* Serialize */

    std::vector<uint8_t> BlockSerializer::serialize(const model::Block block) {
      rapidjson::StringBuffer sb;
      rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
      serialize(writer, block);
      auto str = sb.GetString();
      std::vector<uint8_t> bytes{str, str + sb.GetLength()};
      return bytes;
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    const model::Block& block) {
      writer.StartObject();
      writer.String("hash");
      writer.String(block.hash.to_hexstring());

      writer.String("signatures");
      writer.StartArray();
      for (auto sig : block.sigs) {
        serialize(writer, sig);
      }
      writer.EndArray();

      writer.String("created_ts");
      writer.Uint64(block.created_ts);

      writer.String("height");
      writer.Uint64(block.height);

      writer.String("prev_hash");
      writer.String(block.prev_hash.to_hexstring());

      writer.String("txs_number");
      writer.Uint(block.txs_number);

      writer.String("merkle_root");
      writer.String(block.merkle_root.to_hexstring());

      writer.String("transactions");
      writer.StartArray();
      for (auto tx : block.transactions) {
        serialize(writer, tx);
      }
      writer.EndArray();

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    const model::Signature& signature) {
      writer.StartObject();

      writer.String("pubkey");
      writer.String(signature.pubkey.to_hexstring());

      writer.String("signature");
      writer.String(signature.signature.to_hexstring());

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    const model::Transaction& transaction) {
      writer.StartObject();

      writer.String("signatures");
      writer.StartArray();
      for (auto sig : transaction.signatures) {
        serialize(writer, sig);
      }
      writer.EndArray();

      writer.String("created_ts");
      writer.Uint64(transaction.created_ts);

      writer.String("creator_account_id");
      writer.String(transaction.creator_account_id);

      writer.String("tx_counter");
      writer.Uint64(transaction.tx_counter);

      writer.String("commands");
      writer.StartArray();
      for (auto command : transaction.commands) {
        serialize(writer, *command);
      }
      writer.EndArray();

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    const model::Command& command) {
      auto it = serializers_.find(typeid(command));
      if (it != serializers_.end()) {
        (this->*it->second)(writer, command);
      }
    }

    void BlockSerializer::serialize_add_peer(PrettyWriter<StringBuffer>& writer,
                                    const model::Command& command) {
      auto add_peer = static_cast<const model::AddPeer&>(command);

      writer.StartObject();

      writer.String("command_type");
      writer.String("AddPeer");

      writer.String("address");
      writer.String(add_peer.address);

      writer.String("peer_key");
      writer.String(add_peer.peer_key.to_hexstring());

      writer.EndObject();
    }

    void BlockSerializer::serialize_add_asset_quantity(
        PrettyWriter<StringBuffer>& writer,
        const model::Command& command) {
      auto add_asset_quantity =
          static_cast<const model::AddAssetQuantity&>(command);

      writer.StartObject();

      writer.String("command_type");
      writer.String("AddAssetQuantity");

      writer.String("account_id");
      writer.String(add_asset_quantity.account_id);

      writer.String("asset_id");
      writer.String(add_asset_quantity.asset_id);

      writer.String("amount");
      writer.StartObject();

      writer.String("int_part");
      writer.Uint64(add_asset_quantity.amount.int_part);
      writer.String("frac_part");
      writer.Uint64(add_asset_quantity.amount.frac_part);
      writer.EndObject();

      writer.EndObject();
    }

    void BlockSerializer::serialize_add_signatory(
        PrettyWriter<StringBuffer>& writer, const model::Command& command) {
      auto add_signatory = static_cast<const model::AddSignatory&>(command);

      writer.StartObject();

      writer.String("command_type");
      writer.String("AddSignatory");

      writer.String("account_id");
      writer.String(add_signatory.account_id);

      writer.String("pubkey");
      writer.String(add_signatory.pubkey.to_hexstring());

      writer.EndObject();
    }

    void BlockSerializer::serialize_assign_master_key(
        PrettyWriter<StringBuffer>& writer,
        const model::Command& command) {
      auto assign_master_key =
          static_cast<const model::AssignMasterKey&>(command);

      writer.StartObject();

      writer.String("command_type");
      writer.String("AssignMasterKey");

      writer.String("account_id");
      writer.String(assign_master_key.account_id);

      writer.String("pubkey");
      writer.String(assign_master_key.pubkey.to_hexstring());

      writer.EndObject();
    }

    void BlockSerializer::serialize_create_account(
        PrettyWriter<StringBuffer>& writer,
        const model::Command& command) {
      auto create_account = static_cast<const model::CreateAccount&>(command);

      writer.StartObject();

      writer.String("command_type");
      writer.String("CreateAccount");

      writer.String("domain_id");
      writer.String(create_account.domain_id);

      writer.String("account_name");
      writer.String(create_account.account_name);

      writer.String("pubkey");
      writer.String(create_account.pubkey.to_hexstring());

      writer.EndObject();
    }

    void BlockSerializer::serialize_create_asset(
        PrettyWriter<StringBuffer>& writer, const model::Command& command) {
      auto create_asset = static_cast<const model::CreateAsset&>(command);

      writer.StartObject();

      writer.String("command_type");
      writer.String("CreateAsset");

      writer.String("asset_name");
      writer.String(create_asset.asset_name);

      writer.String("domain_id");
      writer.String(create_asset.domain_id);

      writer.String("precision");
      writer.Uint(create_asset.precision);

      writer.EndObject();
    }

    void BlockSerializer::serialize_create_domain(
        PrettyWriter<StringBuffer>& writer, const model::Command& command) {
      auto create_domain = static_cast<const model::CreateDomain&>(command);

      writer.StartObject();

      writer.String("command_type");
      writer.String("CreateDomain");

      writer.String("domain_name");
      writer.String(create_domain.domain_name);

      writer.EndObject();
    }

    void BlockSerializer::serialize_remove_signatory(
        PrettyWriter<StringBuffer>& writer,
        const model::Command& command) {
      auto remove_signatory =
          static_cast<const model::RemoveSignatory&>(command);

      writer.StartObject();

      writer.String("command_type");
      writer.String("RemoveSignatory");

      writer.String("account_id");
      writer.String(remove_signatory.account_id);

      writer.String("pubkey");
      writer.String(remove_signatory.pubkey.to_hexstring());

      writer.EndObject();
    }

    void BlockSerializer::serialize_set_account_permissions(
        PrettyWriter<StringBuffer>& writer,
        const model::Command& command) {
      auto set_account_permissions =
          static_cast<const model::SetAccountPermissions&>(command);

      writer.StartObject();

      writer.String("command_type");
      writer.String("SetAccountPermissions");

      writer.String("account_id");
      writer.String(set_account_permissions.account_id);

      writer.String("new_permissions");
      writer.StartObject();

      writer.String("add_signatory");
      writer.Bool(set_account_permissions.new_permissions.add_signatory);

      writer.String("can_transfer");
      writer.Bool(set_account_permissions.new_permissions.can_transfer);

      writer.String("create_accounts");
      writer.Bool(set_account_permissions.new_permissions.create_accounts);

      writer.String("create_assets");
      writer.Bool(set_account_permissions.new_permissions.create_assets);

      writer.String("create_domains");
      writer.Bool(set_account_permissions.new_permissions.create_domains);

      writer.String("issue_assets");
      writer.Bool(set_account_permissions.new_permissions.issue_assets);

      writer.String("read_all_accounts");
      writer.Bool(set_account_permissions.new_permissions.read_all_accounts);

      writer.String("remove_signatory");
      writer.Bool(set_account_permissions.new_permissions.remove_signatory);

      writer.String("set_permissions");
      writer.Bool(set_account_permissions.new_permissions.set_permissions);

      writer.String("set_quorum");
      writer.Bool(set_account_permissions.new_permissions.set_quorum);

      writer.EndObject();

      writer.EndObject();
    }

    void BlockSerializer::serialize_set_quorum(
        PrettyWriter<StringBuffer>& writer, const model::Command& command) {
      auto set_quorum = static_cast<const model::SetQuorum&>(command);

      writer.StartObject();

      writer.String("command_type");
      writer.String("SetQuorum");

      writer.String("account_id");
      writer.String(set_quorum.account_id);

      writer.String("new_quorum");
      writer.Uint(set_quorum.new_quorum);

      writer.EndObject();
    }

    void BlockSerializer::serialize_transfer_asset(
        PrettyWriter<StringBuffer>& writer,
        const model::Command& command) {
      auto transfer_asset = static_cast<const model::TransferAsset&>(command);

      writer.StartObject();

      writer.String("command_type");
      writer.String("TransferAsset");

      writer.String("dest_account_id");
      writer.String(transfer_asset.dest_account_id);

      writer.String("src_account_id");
      writer.String(transfer_asset.src_account_id);

      writer.String("asset_id");
      writer.String(transfer_asset.asset_id);

      writer.String("amount");
      writer.StartObject();
      writer.String("int_part");
      writer.Uint64(transfer_asset.amount.int_part);
      writer.String("frac_part");
      writer.Uint64(transfer_asset.amount.frac_part);
      writer.EndObject();

      writer.EndObject();
    }

    /* Deserialize */

    nonstd::optional<model::Block> BlockSerializer::deserialize(
        const std::vector<uint8_t>& bytes) {
      std::string block_json(bytes.begin(), bytes.end());
      rapidjson::Document doc;
      if (doc.Parse(block_json).HasParseError()) {
        return nonstd::nullopt;
      }

      auto req_fields = {"hash", "signatures", "created_ts", "height",
                         "prev_hash", "txs_number"};
      auto verify_member = [&doc](auto&& field) {
        return not doc.HasMember(field);
      };

      auto json_sigs = doc["signatures"].GetArray();

      if (std::any_of(req_fields.begin(), req_fields.end(), verify_member)) {
        return nonstd::nullopt;
      }

      model::Block block{};

      // Check if hash is present
      deserialize(doc["hash"].GetString(), block.hash);

      // Signatures are critical part of a Block, if there are none - Block is
      // invalid
      if (not deserialize(json_sigs, block.sigs)) {
        return nonstd::nullopt;
      }

      // created_ts
      block.created_ts = doc["created_ts"].GetUint64();

      // height
      block.height = doc["height"].GetUint64();

      // prev_hash
      deserialize(doc["prev_hash"].GetString(), block.prev_hash);

      // txs number
      block.txs_number =
          static_cast<decltype(block.txs_number)>(doc["txs_number"].GetUint());
      // merkle_root is optional
      if (doc.HasMember("merkle_root")) {
        deserialize(doc["merkle_root"].GetString(), block.merkle_root);
      }

      // Deserialize transactions
      if (not deserialize(doc, block.transactions)) {
        return nonstd::nullopt;
      };

      return block;
    }

    nonstd::optional<model::Transaction> BlockSerializer::deserialize(
        GenericValue<rapidjson::UTF8<char>>::Object& json_tx) {
      model::Transaction tx{};

      auto req_fields = {"creator_account_id", "tx_counter", "commands"};
      auto verify_member = [&json_tx](auto&& field) {
        return not json_tx.HasMember(field);
      };
      if (std::any_of(req_fields.begin(), req_fields.end(), verify_member)) {
        return nonstd::nullopt;
      }

      if (json_tx.HasMember("signatures")) {
        deserialize(json_tx["signatures"].GetArray(), tx.signatures);
      }
      if (json_tx.HasMember("created_ts")) {
        // created_ts
        tx.created_ts = json_tx["created_ts"].GetUint64();
      }
      // creator_account_id
      tx.creator_account_id = json_tx["creator_account_id"].GetString();

      // tx_counter
      tx.tx_counter = json_tx["tx_counter"].GetUint64();

      // deserialize commands
      if (not deserialize(json_tx, tx.commands)) {
        return nonstd::nullopt;
      }
      return tx;
    }

    nonstd::optional<model::Transaction> BlockSerializer::deserialize(
        const std::string json_tx) {
      Document doc;
      if (doc.Parse(json_tx).HasParseError()) {
        return nonstd::nullopt;
      }
      auto obj_tx = doc.GetObject();
      return deserialize(obj_tx);
    }

    bool BlockSerializer::deserialize(
        Document& doc, std::vector<model::Transaction>& transactions) {
      if (not doc.HasMember("transactions")) {
        return false;
      }
      auto json_txs = doc["transactions"].GetArray();
      for (auto&& iter : json_txs) {
        auto json_obj = iter.GetObject();
        auto tx_opt = deserialize(json_obj);
        if (not tx_opt.has_value()) {
          return false;
        }
        auto tx = tx_opt.value();
        transactions.push_back(tx);
      }
      return true;
    }

    bool BlockSerializer::deserialize(
        GenericValue<rapidjson::UTF8<char>>::Array json_sigs,
        std::vector<model::Signature>& sigs) {
      for (auto&& sig_iter : json_sigs) {
        auto json_sig = sig_iter.GetObject();
        model::Signature signature{};

        if (not sig_iter.HasMember("pubkey") or
            not sig_iter.HasMember("signature")) {
          return false;
        }

        deserialize(json_sig["pubkey"].GetString(), signature.pubkey);

        deserialize(json_sig["signature"].GetString(), signature.signature);

        sigs.push_back(signature);
      }
      return true;
    }

    bool BlockSerializer::deserialize(
        GenericValue<UTF8<char>>::Object& json_tx,
        std::vector<std::shared_ptr<model::Command>>& commands) {
      auto json_commands = json_tx["commands"].GetArray();

      auto deserialize_command = [this, &commands](auto &&value) {
        auto json_command = value.GetObject();
        if (not json_command.HasMember("command_type")) {
          return false;
        }
        std::string command_type = json_command["command_type"].GetString();
        auto it = deserializers_.find(command_type);
        std::shared_ptr<model::Command> cmd;
        if (it != deserializers_.end() and
            (cmd = (this->*it->second)(json_command))) {
          commands.push_back(cmd);
          return true;
        }
        return false;
      };

      return std::all_of(json_commands.begin(), json_commands.end(),
                         deserialize_command);
    }

    std::shared_ptr<model::Command> BlockSerializer::deserialize_add_peer(
        GenericValue<rapidjson::UTF8<char>>::Object& json_command) {
      // TODO: make this function return nullopt when some field is missed
      auto add_peer = std::make_shared<model::AddPeer>();

      // peer_key
      deserialize(json_command["peer_key"].GetString(), add_peer->peer_key);

      // address
      add_peer->address = json_command["address"].GetString();
      return add_peer;
    }

    std::shared_ptr<model::Command>
    BlockSerializer::deserialize_add_asset_quantity(
        GenericValue<rapidjson::UTF8<char>>::Object& json_command) {
      // TODO: make this function return nullopt when some field is missed
      auto add_asset_quantity = std::make_shared<model::AddAssetQuantity>();

      // account_id
      add_asset_quantity->account_id = json_command["account_id"].GetString();

      // asset_id
      add_asset_quantity->asset_id = json_command["asset_id"].GetString();

      // amount
      auto json_amount = json_command["amount"].GetObject();
      Amount amount;
      amount.int_part = json_amount["int_part"].GetUint64();
      amount.frac_part = json_amount["frac_part"].GetUint64();
      add_asset_quantity->amount = amount;

      return add_asset_quantity;
    }

    std::shared_ptr<model::Command>
    BlockSerializer::deserialize_add_signatory(
        GenericValue<rapidjson::UTF8<char>>::Object& json_command) {
      // TODO: make this function return nullopt when some field is missed
      auto add_signatory = std::make_shared<model::AddSignatory>();

      // account_id
      add_signatory->account_id = json_command["account_id"].GetString();

      // pubkey
      deserialize(json_command["pubkey"].GetString(), add_signatory->pubkey);

      return add_signatory;
    }

    std::shared_ptr<model::Command>
    BlockSerializer::deserialize_assign_master_key(
        GenericValue<rapidjson::UTF8<char>>::Object& json_command) {
      // TODO: make this function return nullopt when some field is missed
      auto assign_master_key = std::make_shared<model::AssignMasterKey>();

      // account_id
      assign_master_key->account_id = json_command["account_id"].GetString();

      // pubkey
      deserialize(json_command["pubkey"].GetString(),
                  assign_master_key->pubkey);

      return assign_master_key;
    }

    std::shared_ptr<model::Command>
    BlockSerializer::deserialize_create_account(
        GenericValue<rapidjson::UTF8<char>>::Object& json_command) {
      // TODO: make this function return nullopt when some field is missed
      auto create_account = std::make_shared<model::CreateAccount>();

      // domain_id
      create_account->domain_id = json_command["domain_id"].GetString();

      // account_name
      create_account->account_name = json_command["account_name"].GetString();

      // pubkey
      deserialize(json_command["pubkey"].GetString(), create_account->pubkey);

      return create_account;
    }

    std::shared_ptr<model::Command>
    BlockSerializer::deserialize_create_asset(
        GenericValue<rapidjson::UTF8<char>>::Object& json_command) {
      // TODO: make this function return nullopt when some field is missed
      auto createAsset = std::make_shared<model::CreateAsset>();

      // asset_name
      createAsset->asset_name = json_command["asset_name"].GetString();

      // domain_id
      createAsset->domain_id = json_command["domain_id"].GetString();

      // precision
      createAsset->precision = json_command["precision"].GetUint();

      return createAsset;
    }

    std::shared_ptr<model::Command>
    BlockSerializer::deserialize_create_domain(
        GenericValue<rapidjson::UTF8<char>>::Object& json_command) {
      // TODO: make this function return nullopt when some field is missed
      auto createDomain = std::make_shared<model::CreateDomain>();

      // domain_name
      createDomain->domain_name = json_command["domain_name"].GetString();

      return createDomain;
    }

    std::shared_ptr<model::Command>
    BlockSerializer::deserialize_remove_signatory(
        GenericValue<rapidjson::UTF8<char>>::Object& json_command) {
      // TODO: make this function return nullopt when some field is missed
      auto removeSignatory = std::make_shared<model::RemoveSignatory>();

      // account_id
      removeSignatory->account_id = json_command["account_id"].GetString();

      // pubkey
      deserialize(json_command["pubkey"].GetString(), removeSignatory->pubkey);

      return removeSignatory;
    }

    std::shared_ptr<model::Command>
    BlockSerializer::deserialize_set_account_permissions(
        GenericValue<rapidjson::UTF8<char>>::Object& json_command) {
      auto setAccountPermissions =
          std::make_shared<model::SetAccountPermissions>();

      // account_id
      setAccountPermissions->account_id =
          json_command["account_id"].GetString();

      // permissions
      auto new_permissions = json_command["new_permissions"].GetObject();
      setAccountPermissions->new_permissions.add_signatory =
          new_permissions["add_signatory"].GetBool();
      setAccountPermissions->new_permissions.can_transfer =
          new_permissions["can_transfer"].GetBool();
      setAccountPermissions->new_permissions.create_accounts =
          new_permissions["create_accounts"].GetBool();
      setAccountPermissions->new_permissions.create_assets =
          new_permissions["create_assets"].GetBool();
      setAccountPermissions->new_permissions.create_domains =
          new_permissions["create_domains"].GetBool();
      setAccountPermissions->new_permissions.issue_assets =
          new_permissions["issue_assets"].GetBool();
      setAccountPermissions->new_permissions.read_all_accounts =
          new_permissions["read_all_accounts"].GetBool();
      setAccountPermissions->new_permissions.remove_signatory =
          new_permissions["remove_signatory"].GetBool();
      setAccountPermissions->new_permissions.set_permissions =
          new_permissions["set_permissions"].GetBool();
      setAccountPermissions->new_permissions.set_quorum =
          new_permissions["set_quorum"].GetBool();

      return setAccountPermissions;
    }

    std::shared_ptr<model::Command> BlockSerializer::deserialize_set_quorum(
        GenericValue<rapidjson::UTF8<char>>::Object& json_command) {
      auto setQuorum = std::make_shared<model::SetQuorum>();

      // account_id
      setQuorum->account_id = json_command["account_id"].GetString();

      // new_quorum
      setQuorum->new_quorum = json_command["new_quorum"].GetUint();

      return setQuorum;
    }

    std::shared_ptr<model::Command>
    BlockSerializer::deserialize_transfer_asset(
        GenericValue<rapidjson::UTF8<char>>::Object& json_command) {
      auto transferAsset = std::make_shared<model::TransferAsset>();

      // src_account_id
      transferAsset->src_account_id =
          json_command["src_account_id"].GetString();

      // dest_account_id
      transferAsset->dest_account_id =
          json_command["dest_account_id"].GetString();

      // asset_id
      transferAsset->asset_id = json_command["asset_id"].GetString();

      // amount
      auto json_amount = json_command["amount"].GetObject();
      Amount amount;
      amount.int_part = json_amount["int_part"].GetUint64();
      amount.frac_part = json_amount["frac_part"].GetUint64();
      transferAsset->amount = amount;

      return transferAsset;
    }
  }  // namespace ametsuchi
}  // namespace iroha
