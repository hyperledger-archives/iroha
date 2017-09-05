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

      optional_ptr<Command> JsonCommandFactory::deserializeAddAssetQuantity(
          const Value &document) {
        return make_optional_ptr<AddAssetQuantity>() |
               [&document](auto command) {
                 return deserializeField(command, &AddAssetQuantity::account_id,
                                         document, "account_id",
                                         &Value::IsString, &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &AddAssetQuantity::asset_id,
                                         document, "asset_id", &Value::IsString,
                                         &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &AddAssetQuantity::amount,
                                         document, "amount", &Value::IsObject,
                                         &Value::GetObject);
               } |
               [](auto command) { return optional_ptr<Command>(command); };
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

      optional_ptr<Command> JsonCommandFactory::deserializeAddPeer(
          const Value &document) {
        return make_optional_ptr<AddPeer>() |
               [&document](auto command) {
                 return deserializeField(command, &AddPeer::peer_key, document,
                                         "peer_key", &Value::IsString,
                                         &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &AddPeer::address, document,
                                         "address", &Value::IsString,
                                         &Value::GetString);
               } |
               [](auto command) { return optional_ptr<Command>(command); };
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

      optional_ptr<Command> JsonCommandFactory::deserializeAddSignatory(
          const Value &document) {
        return make_optional_ptr<AddSignatory>() |
               [&document](auto command) {
                 return deserializeField(command, &AddSignatory::account_id,
                                         document, "account_id",
                                         &Value::IsString, &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &AddSignatory::pubkey,
                                         document, "pubkey", &Value::IsString,
                                         &Value::GetString);
               } |
               [](auto command) { return optional_ptr<Command>(command); };
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

      optional_ptr<Command> JsonCommandFactory::deserializeCreateAccount(
          const Value &document) {
        return make_optional_ptr<CreateAccount>() |
               [&document](auto command) {
                 return deserializeField(command, &CreateAccount::account_name,
                                         document, "account_name",
                                         &Value::IsString, &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &CreateAccount::domain_id,
                                         document, "domain_id",
                                         &Value::IsString, &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &CreateAccount::pubkey,
                                         document, "pubkey", &Value::IsString,
                                         &Value::GetString);
               } |
               [](auto command) { return optional_ptr<Command>(command); };
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

      optional_ptr<Command> JsonCommandFactory::deserializeCreateAsset(
          const Value &document) {
        return make_optional_ptr<CreateAsset>() |
               [&document](auto command) {
                 return deserializeField(command, &CreateAsset::asset_name,
                                         document, "asset_name",
                                         &Value::IsString, &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &CreateAsset::domain_id,
                                         document, "domain_id",
                                         &Value::IsString, &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &CreateAsset::precision,
                                         document, "precision", &Value::IsUint,
                                         &Value::GetUint);
               } |
               [](auto command) { return optional_ptr<Command>(command); };
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

      optional_ptr<Command> JsonCommandFactory::deserializeCreateDomain(
          const Value &document) {
        return make_optional_ptr<CreateDomain>() |
               [&document](auto command) {
                 return deserializeField(command, &CreateDomain::domain_name,
                                         document, "domain_name",
                                         &Value::IsString, &Value::GetString);
               } |
               [](auto command) { return optional_ptr<Command>(command); };
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

      optional_ptr<Command> JsonCommandFactory::deserializeRemoveSignatory(
          const Value &document) {
        return make_optional_ptr<RemoveSignatory>() |
               [&document](auto command) {
                 return deserializeField(command, &RemoveSignatory::account_id,
                                         document, "account_id",
                                         &Value::IsString, &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &RemoveSignatory::pubkey,
                                         document, "pubkey", &Value::IsString,
                                         &Value::GetString);
               } |
               [](auto command) { return optional_ptr<Command>(command); };
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

      optional_ptr<Command>
      JsonCommandFactory::deserializeSetAccountPermissions(
          const Value &document) {
        return make_optional_ptr<SetAccountPermissions>() |
               [&document](auto command) {
                 return deserializeField(
                     command, &SetAccountPermissions::account_id, document,
                     "account_id", &Value::IsString, &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(
                     command, &SetAccountPermissions::new_permissions, document,
                     "new_permissions", &Value::IsObject, &Value::GetObject);
               } |
               [](auto command) { return optional_ptr<Command>(command); };
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

      optional_ptr<Command> JsonCommandFactory::deserializeSetQuorum(
          const Value &document) {
        return make_optional_ptr<SetQuorum>() |
               [&document](auto command) {
                 return deserializeField(command, &SetQuorum::account_id,
                                         document, "account_id",
                                         &Value::IsString, &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &SetQuorum::new_quorum,
                                         document, "new_quorum", &Value::IsUint,
                                         &Value::GetUint);
               } |
               [](auto command) { return optional_ptr<Command>(command); };
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
        document.AddMember("description", transfer_asset->description,
                           allocator);

        Value amount;
        amount.SetObject();
        amount.AddMember("int_part", transfer_asset->amount.int_part,
                         allocator);
        amount.AddMember("frac_part", transfer_asset->amount.frac_part,
                         allocator);

        document.AddMember("amount", amount, allocator);

        return document;
      }

      optional_ptr<Command> JsonCommandFactory::deserializeTransferAsset(
          const Value &document) {
        return make_optional_ptr<TransferAsset>() |
               [&document](auto command) {
                 return deserializeField(
                     command, &TransferAsset::src_account_id, document,
                     "src_account_id", &Value::IsString, &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(
                     command, &TransferAsset::dest_account_id, document,
                     "dest_account_id", &Value::IsString, &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &TransferAsset::asset_id,
                                         document, "asset_id", &Value::IsString,
                                         &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &TransferAsset::description,
                                         document, "description", &Value::IsString,
                                         &Value::GetString);
               } |
               [&document](auto command) {
                 return deserializeField(command, &TransferAsset::amount,
                                         document, "amount", &Value::IsObject,
                                         &Value::GetObject);
               } |
               [](auto command) { return optional_ptr<Command>(command); };
      }

      // Abstract
      Document JsonCommandFactory::serializeAbstractCommand(
          std::shared_ptr<Command> command) {
        return (this->*serializers_.at(typeid(*command)))(command);
      }

      optional_ptr<model::Command>
      JsonCommandFactory::deserializeAbstractCommand(const Value &document) {
        return deserializeField(document, "command_type", &Value::IsString,
                                &Value::GetString) |
                   [this, &document](
                       auto command_type) -> optional_ptr<model::Command> {
          auto it = deserializers_.find(command_type);
          if (it != deserializers_.end()) {
            return (this->*deserializers_.at(command_type))(document);
          }
          return nonstd::nullopt;
        };
      }
    }  // namespace converters
  }    // namespace model
}  // namespace iroha
