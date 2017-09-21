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

#ifndef IROHA_PERMISSIONS_HPP
#define IROHA_PERMISSIONS_HPP

#include <string>

namespace iroha {
  namespace model {

  const std::string can_append_role = "CanAppendRole";
  const std::string can_create_role = "CanСreateRole";
  const std::string can_add_asset_qty = "CanAddAssetQuantity";
  const std::string can_add_peer = "CanAddPeer";
  const std::string can_add_signatory = "CanAddSignatory";
  const std::string can_create_account = "CanCreateAccount";
  const std::string can_create_asset = "CanCreateAsset";
  const std::string can_create_domain = "CanCreateDomain";
  const std::string can_remove_signatory = "CanRemoveSignatory";
  const std::string can_set_quorum = "CanSetQuorum";
  const std::string can_transfer = "CanTransfer";
  const std::string can_receive = "CanReceive";

  } // namespace model
}  // namespace iroha

#endif  // IROHA_PERMISSIONS_HPP
