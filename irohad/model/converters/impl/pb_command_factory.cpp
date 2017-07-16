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

#include "model/converters/pb_command_factory.hpp"

#include <string>

namespace iroha {
  namespace model {
    namespace converters {

      // asset quantity
      protocol::AddAssetQuantity
      PbCommandFactory::serializeAddAssetQuantity(
          model::AddAssetQuantity &add_asset_quantity) {
        protocol::AddAssetQuantity pb_add_asset_quantity;
        pb_add_asset_quantity.set_account_id(add_asset_quantity.account_id);
        pb_add_asset_quantity.set_asset_id(add_asset_quantity.asset_id);
        // todo set amount
      }

      model::AddAssetQuantity
      PbCommandFactory::deserializeAddAssetQuantity(
          protocol::AddAssetQuantity &pb_add_asset_quantity) {
        model::AddAssetQuantity add_asset_quantity;
        add_asset_quantity.account_id = pb_add_asset_quantity.account_id();
        add_asset_quantity.asset_id = pb_add_asset_quantity.asset_id();
        // todo set amount
        return add_asset_quantity;
      }

      // add peer
      protocol::AddPeer
      PbCommandFactory::serializeAddPeer(model::AddPeer &add_peer) {
        protocol::AddPeer pb_add_peer;
        pb_add_peer.set_address(add_peer.address);
        pb_add_peer.set_peer_key(add_peer.peer_key.data(),
                                 add_peer.peer_key.size());
        return pb_add_peer;
      }
      model::AddPeer
      PbCommandFactory::deserializeAddPeer(protocol::AddPeer &pb_add_peer) {
        model::AddPeer add_peer;
        add_peer.address = pb_add_peer.address();
        std::copy(pb_add_peer.peer_key().begin(), pb_add_peer.peer_key().end(),
                  add_peer.peer_key.begin());
        return add_peer;
      }

      // add signatory
      protocol::AddSignatory
      PbCommandFactory::serializeAddSignatory(model::AddSignatory &addSignatory) {
        protocol::AddSignatory add_signatory;

        return add_signatory;
      }
      model::AddSignatory PbCommandFactory::deserializeAddSignatory(protocol::AddSignatory &addSignatory) {

      }

      // assign master key
      protocol::AssignMasterKey PbCommandFactory::serializeAssignMasterKey(model::AssignMasterKey &assignMasterKey) {}
      model::AssignMasterKey PbCommandFactory::deserializeAssignMasterKey(
          protocol::AssignMasterKey &assignMasterKey) {}

      // create asset
      protocol::CreateAsset PbCommandFactory::serializeCreateAsset(model::CreateAsset &createAsset) {}
      model::CreateAsset PbCommandFactory::deserializeCreateAsset(protocol::CreateAsset &createAsset) {}

      // create account
      protocol::CreateAccount PbCommandFactory::serializeCreateAccount(model::CreateAccount &createAccount) {}
      model::CreateAccount PbCommandFactory::deserializeCreateAccount(protocol::CreateAccount &createAccount) {}

      // create domain
      protocol::CreateDomain PbCommandFactory::serializeCreateDomain(model::CreateDomain &createDomain) {}
      model::CreateDomain PbCommandFactory::deserializeCreateDomain(protocol::CreateDomain &createDomain) {}

      // remove signatory
      protocol::RemoveSignatory PbCommandFactory::serializeRemoveSignatory(model::RemoveSignatory &removeSignatory) {}
      model::RemoveSignatory PbCommandFactory::deserializeRemoveSignatory(
          protocol::RemoveSignatory &removeSignatory) {}

      // set account permissions
      protocol::SetAccountPermissions PbCommandFactory::serializeSetAccountPermissions(
          model::SetAccountPermissions &setAccountPermissions) {}
      model::SetAccountPermissions PbCommandFactory::deserializeSetAccountPermissions(
          protocol::SetAccountPermissions &setAccountPermissions) {}

      // set account quorum
      protocol::SetAccountQuorum PbCommandFactory::serializeSetQuorum(model::SetQuorum &setAccountQuorum) {}
      model::SetQuorum PbCommandFactory::deserializeSetQuorum(protocol::SetAccountQuorum &setAccountQuorum) {}

      // transfer asset
      protocol::TransferAsset PbCommandFactory::serializeTransferAsset(model::TransferAsset &subtractAssetQuantity) {}
      model::TransferAsset PbCommandFactory::deserializeTransferAsset(protocol::TransferAsset &subtractAssetQuantity) {}

    } // namespace converters
  }  // namespace model
}  // namespace iroha