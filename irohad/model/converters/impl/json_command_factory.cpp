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

#include "model/converters/json_command_factory.hpp"
#include "model/commands/add_asset_quantity.hpp"
#include "model/commands/add_peer.hpp"
#include "model/commands/add_signatory.hpp"
#include "model/commands/assign_master_key.hpp"
#include "model/commands/create_account.hpp"
#include "model/commands/create_asset.hpp"
#include "model/commands/create_domain.hpp"
#include "model/commands/remove_signatory.hpp"
#include "model/commands/set_permissions.hpp"
#include "model/commands/set_quorum.hpp"
#include "model/commands/transfer_asset.hpp"

using namespace rapidjson;

namespace iroha {
  namespace model {
    namespace converters {

      JsonCommandFactory::JsonCommandFactory() {
        serializers_[typeid(AddAssetQuantity)] =
            &JsonCommandFactory::serializeAddAssetQuantity;
        serializers_[typeid(AddPeer)] = &JsonCommandFactory::serializeAddPeer;
        serializers_[typeid(AddSignatory)] =
            &JsonCommandFactory::serializeAddSignatory;
        serializers_[typeid(AssignMasterKey)] =
            &JsonCommandFactory::serializeAssignMasterKey;
        serializers_[typeid(CreateAccount)] =
            &JsonCommandFactory::serializeCreateAccount;
        serializers_[typeid(CreateAsset)] =
            &JsonCommandFactory::serializeCreateAsset;
        serializers_[typeid(CreateDomain)] =
            &JsonCommandFactory::serializeCreateDomain;
        serializers_[typeid(RemoveSignatory)] =
            &JsonCommandFactory::serializeRemoveSignatory;
        serializers_[typeid(SetAccountPermissions)] =
            &JsonCommandFactory::serializeSetAccountPermissions;
        serializers_[typeid(SetQuorum)] =
            &JsonCommandFactory::serializeSetQuorum;
        serializers_[typeid(TransferAsset)] =
            &JsonCommandFactory::serializeTransferAsset;

        deserializers_["AddAssetQuantity"] =
            &JsonCommandFactory::deserializeAddAssetQuantity;
        deserializers_["AddPeer"] = &JsonCommandFactory::deserializeAddPeer;
        deserializers_["AddSignatory"] =
            &JsonCommandFactory::deserializeAddSignatory;
        deserializers_["AssignMasterKey"] =
            &JsonCommandFactory::deserializeAssignMasterKey;
        deserializers_["CreateAccount"] =
            &JsonCommandFactory::deserializeCreateAccount;
        deserializers_["CreateAsset"] =
            &JsonCommandFactory::deserializeCreateAsset;
        deserializers_["CreateDomain"] =
            &JsonCommandFactory::deserializeCreateDomain;
        deserializers_["RemoveSignatory"] =
            &JsonCommandFactory::deserializeRemoveSignatory;
        deserializers_["SetAccountPermissions"] =
            &JsonCommandFactory::deserializeSetAccountPermissions;
        deserializers_["SetQuorum"] = &JsonCommandFactory::deserializeSetQuorum;
        deserializers_["TransferAsset"] =
            &JsonCommandFactory::deserializeTransferAsset;
      }

      // AddAssetQuantity
      Document JsonCommandFactory::serializeAddAssetQuantity(
          std::shared_ptr<Command> command) {
        auto add_asset_quantity =
            static_cast<AddAssetQuantity *>(command.get());

        Document document;
        auto &allocator = document.GetAllocator();

        document.SetObject();
        document.AddMember("command_type", "AddAssetQuantity", allocator);
        document.AddMember("account_id", add_asset_quantity->account_id,
                           allocator);
        document.AddMember("asset_id", add_asset_quantity->asset_id, allocator);

        Value amount;
        amount.SetObject();
        amount.AddMember("int_part", add_asset_quantity->amount.int_part,
                         allocator);
        amount.AddMember("frac_part", add_asset_quantity->amount.frac_part,
                         allocator);

        document.AddMember("amount", amount, allocator);

        return document;
      }

      std::shared_ptr<Command> JsonCommandFactory::deserializeAddAssetQuantity(
          const Document &command) {
        auto add_asset_quantity = std::make_shared<AddAssetQuantity>();

        // account_id
        add_asset_quantity->account_id = command["account_id"].GetString();

        // asset_id
        add_asset_quantity->asset_id = command["asset_id"].GetString();

        // amount
        auto amount = command["amount"].GetObject();
        add_asset_quantity->amount.int_part = amount["int_part"].GetUint64();
        add_asset_quantity->amount.frac_part = amount["frac_part"].GetUint64();

        return add_asset_quantity;
      }

      // AddPeer
      Document JsonCommandFactory::serializeAddPeer(
          std::shared_ptr<Command> command) {
        auto add_peer = static_cast<AddPeer *>(command.get());

        Document document;
        auto &allocator = document.GetAllocator();

        document.SetObject();
        document.AddMember("command_type", "AddPeer", allocator);
        document.AddMember("address", add_peer->address, allocator);
        document.AddMember("peer_key", add_peer->peer_key.to_hexstring(),
                           allocator);

        return document;
      }

      std::shared_ptr<Command> JsonCommandFactory::deserializeAddPeer(
          const Document &command) {
        auto add_peer = std::make_shared<AddPeer>();

        // peer key
        hexstringToArray(command["peer_key"].GetString(), add_peer->peer_key);

        // address
        add_peer->address = command["address"].GetString();
        return add_peer;
      }

      // AddSignatory
      Document JsonCommandFactory::serializeAddSignatory(
          std::shared_ptr<Command> command) {
        auto add_signatory = static_cast<AddSignatory *>(command.get());

        Document document;
        auto &allocator = document.GetAllocator();

        document.SetObject();
        document.AddMember("command_type", "AddSignatory", allocator);
        document.AddMember("account_id", add_signatory->account_id, allocator);
        document.AddMember("pubkey", add_signatory->pubkey.to_hexstring(),
                           allocator);

        return document;
      }

      std::shared_ptr<Command> JsonCommandFactory::deserializeAddSignatory(
          const Document &command) {
        auto add_signatory = std::make_shared<AddSignatory>();

        // account_id
        add_signatory->account_id = command["account_id"].GetString();

        // pubkey
        hexstringToArray(command["pubkey"].GetString(), add_signatory->pubkey);

        return add_signatory;
      }

      // AssignMasterKey
      Document JsonCommandFactory::serializeAssignMasterKey(
          std::shared_ptr<Command> command) {
        auto assign_master_key = static_cast<AssignMasterKey *>(command.get());

        Document document;
        auto &allocator = document.GetAllocator();

        document.SetObject();
        document.AddMember("command_type", "AssignMasterKey", allocator);
        document.AddMember("account_id", assign_master_key->account_id,
                           allocator);
        document.AddMember("pubkey", assign_master_key->pubkey.to_hexstring(),
                           allocator);

        return document;
      }

      std::shared_ptr<Command> JsonCommandFactory::deserializeAssignMasterKey(
          const Document &command) {
        auto assign_master_key = std::make_shared<AssignMasterKey>();

        // account_id
        assign_master_key->account_id = command["account_id"].GetString();

        // pubkey
        hexstringToArray(command["pubkey"].GetString(),
                         assign_master_key->pubkey);

        return assign_master_key;
      }

      // CreateAccount
      Document JsonCommandFactory::serializeCreateAccount(
          std::shared_ptr<Command> command) {
        auto create_account = static_cast<CreateAccount *>(command.get());

        Document document;
        auto &allocator = document.GetAllocator();

        document.SetObject();
        document.AddMember("command_type", "CreateAccount", allocator);
        document.AddMember("account_name", create_account->account_name,
                           allocator);
        document.AddMember("domain_id", create_account->domain_id, allocator);
        document.AddMember("pubkey", create_account->pubkey.to_hexstring(),
                           allocator);

        return document;
      }

      std::shared_ptr<Command> JsonCommandFactory::deserializeCreateAccount(
          const Document &command) {
        auto create_account = std::make_shared<CreateAccount>();

        // account_name
        create_account->account_name = command["account_name"].GetString();

        // domain_id
        create_account->domain_id = command["domain_id"].GetString();

        // pubkey
        hexstringToArray(command["pubkey"].GetString(), create_account->pubkey);

        return create_account;
      }

      // CreateAsset
      Document JsonCommandFactory::serializeCreateAsset(
          std::shared_ptr<Command> command) {
        auto create_asset = static_cast<CreateAsset *>(command.get());

        Document document;
        auto &allocator = document.GetAllocator();

        document.SetObject();
        document.AddMember("command_type", "CreateAsset", allocator);
        document.AddMember("asset_name", create_asset->asset_name, allocator);
        document.AddMember("domain_id", create_asset->domain_id, allocator);
        document.AddMember("precision", create_asset->precision, allocator);

        return document;
      }

      std::shared_ptr<Command> JsonCommandFactory::deserializeCreateAsset(
          const Document &command) {
        auto create_asset = std::make_shared<CreateAsset>();

        // asset_name
        create_asset->asset_name = command["asset_name"].GetString();

        // domain_id
        create_asset->domain_id = command["domain_id"].GetString();

        // precision
        create_asset->precision = command["precision"].GetUint();

        return create_asset;
      }

      // CreateDomain
      Document JsonCommandFactory::serializeCreateDomain(
          std::shared_ptr<Command> command) {
        auto create_domain = static_cast<CreateDomain *>(command.get());

        Document document;
        auto &allocator = document.GetAllocator();

        document.SetObject();
        document.AddMember("command_type", "CreateDomain", allocator);
        document.AddMember("domain_name", create_domain->domain_name,
                           allocator);

        return document;
      }

      std::shared_ptr<Command> JsonCommandFactory::deserializeCreateDomain(
          const Document &command) {
        auto create_domain = std::make_shared<CreateDomain>();

        // domain_name
        create_domain->domain_name = command["domain_name"].GetString();

        return create_domain;
      }

      // RemoveSignatory
      Document JsonCommandFactory::serializeRemoveSignatory(
          std::shared_ptr<Command> command) {
        auto remove_signatory = static_cast<RemoveSignatory *>(command.get());

        Document document;
        auto &allocator = document.GetAllocator();

        document.SetObject();
        document.AddMember("command_type", "RemoveSignatory", allocator);
        document.AddMember("account_id", remove_signatory->account_id,
                           allocator);
        document.AddMember("pubkey", remove_signatory->pubkey.to_hexstring(),
                           allocator);

        return document;
      }

      std::shared_ptr<Command> JsonCommandFactory::deserializeRemoveSignatory(
          const Document &command) {
        auto remove_signatory = std::make_shared<RemoveSignatory>();

        // account_id
        remove_signatory->account_id = command["account_id"].GetString();

        // pubkey
        hexstringToArray(command["pubkey"].GetString(),
                         remove_signatory->pubkey);

        return remove_signatory;
      }

      // SetAccountPermissions
      Document JsonCommandFactory::serializeSetAccountPermissions(
          std::shared_ptr<Command> command) {
        auto set_account_permissions =
            static_cast<SetAccountPermissions *>(command.get());

        Document document;
        auto &allocator = document.GetAllocator();

        document.SetObject();
        document.AddMember("command_type", "SetAccountPermissions", allocator);
        document.AddMember("account_id", set_account_permissions->account_id,
                           allocator);

        Value new_permissions;
        new_permissions.SetObject();
        new_permissions.AddMember(
            "add_signatory",
            set_account_permissions->new_permissions.add_signatory, allocator);
        new_permissions.AddMember(
            "can_transfer",
            set_account_permissions->new_permissions.can_transfer, allocator);
        new_permissions.AddMember(
            "create_accounts",
            set_account_permissions->new_permissions.create_accounts,
            allocator);
        new_permissions.AddMember(
            "create_assets",
            set_account_permissions->new_permissions.create_assets, allocator);
        new_permissions.AddMember(
            "create_domains",
            set_account_permissions->new_permissions.create_domains, allocator);
        new_permissions.AddMember(
            "issue_assets",
            set_account_permissions->new_permissions.issue_assets, allocator);
        new_permissions.AddMember(
            "read_all_accounts",
            set_account_permissions->new_permissions.read_all_accounts,
            allocator);
        new_permissions.AddMember(
            "remove_signatory",
            set_account_permissions->new_permissions.remove_signatory,
            allocator);
        new_permissions.AddMember(
            "set_permissions",
            set_account_permissions->new_permissions.set_permissions,
            allocator);
        new_permissions.AddMember(
            "set_quorum", set_account_permissions->new_permissions.set_quorum,
            allocator);

        document.AddMember("new_permissions", new_permissions, allocator);

        return document;
      }

      std::shared_ptr<Command>
      JsonCommandFactory::deserializeSetAccountPermissions(
          const Document &command) {
        auto set_account_permissions =
            std::make_shared<SetAccountPermissions>();

        // account_id
        set_account_permissions->account_id = command["account_id"].GetString();

        // permissions
        auto new_permissions = command["new_permissions"].GetObject();
        set_account_permissions->new_permissions.add_signatory =
            new_permissions["add_signatory"].GetBool();
        set_account_permissions->new_permissions.can_transfer =
            new_permissions["can_transfer"].GetBool();
        set_account_permissions->new_permissions.create_accounts =
            new_permissions["create_accounts"].GetBool();
        set_account_permissions->new_permissions.create_assets =
            new_permissions["create_assets"].GetBool();
        set_account_permissions->new_permissions.create_domains =
            new_permissions["create_domains"].GetBool();
        set_account_permissions->new_permissions.issue_assets =
            new_permissions["issue_assets"].GetBool();
        set_account_permissions->new_permissions.read_all_accounts =
            new_permissions["read_all_accounts"].GetBool();
        set_account_permissions->new_permissions.remove_signatory =
            new_permissions["remove_signatory"].GetBool();
        set_account_permissions->new_permissions.set_permissions =
            new_permissions["set_permissions"].GetBool();
        set_account_permissions->new_permissions.set_quorum =
            new_permissions["set_quorum"].GetBool();

        return set_account_permissions;
      }

      // SetAccountQuorum
      Document JsonCommandFactory::serializeSetQuorum(
          std::shared_ptr<Command> command) {
        auto set_quorum = static_cast<SetQuorum *>(command.get());

        Document document;
        auto &allocator = document.GetAllocator();

        document.SetObject();
        document.AddMember("command_type", "SetQuorum", allocator);
        document.AddMember("account_id", set_quorum->account_id, allocator);
        document.AddMember("new_quorum", set_quorum->new_quorum, allocator);

        return document;
      }

      std::shared_ptr<Command> JsonCommandFactory::deserializeSetQuorum(
          const Document &command) {
        auto set_quorum = std::make_shared<SetQuorum>();

        // account_id
        set_quorum->account_id = command["account_id"].GetString();

        // new_quorum
        set_quorum->new_quorum = command["new_quorum"].GetUint();

        return set_quorum;
      }

      // TransferAsset
      Document JsonCommandFactory::serializeTransferAsset(
          std::shared_ptr<Command> command) {
        auto transfer_asset = static_cast<TransferAsset *>(command.get());

        Document document;
        auto &allocator = document.GetAllocator();

        document.SetObject();
        document.AddMember("command_type", "TransferAsset", allocator);
        document.AddMember("src_account_id", transfer_asset->src_account_id,
                           allocator);
        document.AddMember("dest_account_id", transfer_asset->dest_account_id,
                           allocator);
        document.AddMember("asset_id", transfer_asset->asset_id, allocator);

        Value amount;
        amount.SetObject();
        amount.AddMember("int_part", transfer_asset->amount.int_part,
                         allocator);
        amount.AddMember("frac_part", transfer_asset->amount.frac_part,
                         allocator);

        document.AddMember("amount", amount, allocator);

        return document;
      }

      std::shared_ptr<Command> JsonCommandFactory::deserializeTransferAsset(
          const Document &command) {
        auto transfer_asset = std::make_shared<TransferAsset>();

        // src_account_id
        transfer_asset->src_account_id = command["src_account_id"].GetString();

        // dest_account_id
        transfer_asset->dest_account_id =
            command["dest_account_id"].GetString();

        // asset_id
        transfer_asset->asset_id = command["asset_id"].GetString();

        // amount
        auto json_amount = command["amount"].GetObject();
        transfer_asset->amount.int_part = json_amount["int_part"].GetUint64();
        transfer_asset->amount.frac_part = json_amount["frac_part"].GetUint64();

        return transfer_asset;
      }

      // Abstract
      Document JsonCommandFactory::serializeAbstractCommand(
          std::shared_ptr<Command> command) {
        auto it = serializers_.find(typeid(*command));
        if (it != serializers_.end()) {
          return (this->*it->second)(command);
        }
        return Document();
      }

      optional_ptr<model::Command> JsonCommandFactory::deserializeAbstractCommand(
          const Document &command) {
        auto command_type = command["command_type"].GetString();

        auto it = deserializers_.find(command_type);
        if (it != deserializers_.end()) {
          return (this->*it->second)(command);
        }
        return nonstd::nullopt;
      }

    }  // namespace converters
  }    // namespace model
}  // namespace iroha
