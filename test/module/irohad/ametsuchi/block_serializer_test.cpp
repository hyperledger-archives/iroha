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

#include <gtest/gtest.h>
#include "ametsuchi/block_serializer.hpp"
#include "common/types.hpp"

iroha::model::Signature create_signature();
iroha::model::Transaction create_transaction();
iroha::model::Proposal create_proposal();
iroha::model::Block create_block();

iroha::model::Signature create_signature() {
  iroha::model::Signature signature{};
  std::fill(signature.signature.begin(), signature.signature.end(), 0x123);
  std::fill(signature.pubkey.begin(), signature.pubkey.end(), 0x123);
  return signature;
}

iroha::model::Transaction create_transaction() {
  iroha::model::Transaction tx{};
  tx.creator_account_id = "123";

  tx.tx_counter = 0;
  tx.created_ts = 0;

  tx.signatures.push_back(create_signature());
  tx.signatures.push_back(create_signature());

  //  tx.commands

  // Add peer
  iroha::model::AddPeer add_peer;
  add_peer.address = "localhost";
  std::fill(add_peer.peer_key.begin(), add_peer.peer_key.end(), 0x123);
  tx.commands.push_back(std::make_shared<iroha::model::AddPeer>(add_peer));

  // AddAssetQuantity
  iroha::model::AddAssetQuantity add_asset_qty;
  add_asset_qty.account_id = "123";
  add_asset_qty.asset_id = "123";
  iroha::Amount amount;
  amount.int_part = 10;
  amount.frac_part = 10;
  add_asset_qty.amount = amount;
  tx.commands.push_back(std::make_shared<iroha::model::AddAssetQuantity>(add_asset_qty));

  // AddSignatory
  iroha::model::AddSignatory add_signatory;
  add_signatory.account_id = "123";
  std::fill(add_signatory.pubkey.begin(), add_signatory.pubkey.end(), 0x123);
  tx.commands.push_back(std::make_shared<iroha::model::AddSignatory>(add_signatory));

  //AssignMasterKey
  iroha::model::AssignMasterKey assign_master_key;
  std::fill(assign_master_key.pubkey.begin(), assign_master_key.pubkey.end(), 0x123);
  assign_master_key.account_id = "123";
  tx.commands.push_back(std::make_shared<iroha::model::AssignMasterKey>(assign_master_key));

  //CreateAccount
  iroha::model::CreateAccount create_account;
  std::fill(create_account.pubkey.begin(), create_account.pubkey.end(), 0x123);
  create_account.account_name = "123";
  create_account.domain_id = "123";
  tx.commands.push_back(std::make_shared<iroha::model::CreateAccount>(create_account));

  //CreateAsset
  iroha::model::CreateAsset create_asset;
  create_asset.domain_id = "123";
  create_asset.asset_name = "123";
  create_asset.precision = 2;
  tx.commands.push_back(std::make_shared<iroha::model::CreateAsset>(create_asset));

  //CreateDomain
  iroha::model::CreateDomain create_domain;
  create_domain.domain_name = "123";
  tx.commands.push_back(std::make_shared<iroha::model::CreateDomain>(create_domain));

  //RemoveSignatory
  iroha::model::RemoveSignatory remove_signatory;
  remove_signatory.account_id = "123";
  std::fill(remove_signatory.pubkey.begin(), remove_signatory.pubkey.end(), 0x123);
  tx.commands.push_back(std::make_shared<iroha::model::RemoveSignatory>(remove_signatory));

  //SetPermissions
  iroha::model::SetAccountPermissions set_account_permissions;
  set_account_permissions.account_id = "123";
  set_account_permissions.new_permissions.can_transfer = true;
  tx.commands.push_back(std::make_shared<iroha::model::SetAccountPermissions>(set_account_permissions));

  //SetQuorum
  iroha::model::SetQuorum set_quorum;
  set_quorum.account_id = "123";
  set_quorum.new_quorum = 123;
  tx.commands.push_back(std::make_shared<iroha::model::SetQuorum>(set_quorum));

  //TransferAsset
  iroha::model::TransferAsset transfer_asset;
  transfer_asset.src_account_id = "123";
  transfer_asset.dest_account_id = "321";
  transfer_asset.amount = amount;
  transfer_asset.asset_id = "123";
  tx.commands.push_back(std::make_shared<iroha::model::TransferAsset>(transfer_asset));
  return tx;
}

iroha::model::Proposal create_proposal() {
  std::vector<iroha::model::Transaction> txs;
  txs.push_back(create_transaction());
  txs.push_back(create_transaction());

  iroha::model::Proposal proposal(txs);
  return proposal;
}

iroha::model::Block create_block() {
  iroha::model::Block block{};
  std::fill(block.hash.begin(), block.hash.end(), 0x1);
  block.sigs.push_back(create_signature());
  block.created_ts = 0;
  block.height = 0;
  std::fill(block.prev_hash.begin(), block.prev_hash.end(), 0x5);
  block.txs_number = 0;
  std::fill(block.merkle_root.begin(), block.merkle_root.end(), 0x123);
  block.transactions.push_back(create_transaction());
  return block;
}


template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
  return typeid(Base) == typeid(*ptr);
}

TEST(block_serialize, block_serialize_test){

  auto block = create_block();

  iroha::ametsuchi::BlockSerializer blockSerializer;

  std::cout << unsigned(block.hash[0]) << std::endl;

  auto bytes = blockSerializer.serialize(block);
  std::string str(bytes.begin(), bytes.end());
  std::cout << str << std::endl;
  // deserialize

  auto res = blockSerializer.deserialize(bytes);
  if (res){
    auto deserialized = res.value();
    ASSERT_EQ(block.hash, deserialized.hash);
    ASSERT_EQ(block.created_ts, deserialized.created_ts);

    ASSERT_TRUE(block.sigs.size() > 0);
    for (size_t i = 0; i < block.sigs.size(); i++){
      ASSERT_EQ(block.sigs[i].signature, deserialized.sigs[i].signature);
      ASSERT_EQ(block.sigs[i].pubkey, deserialized.sigs[i].pubkey);
    }

    ASSERT_EQ(block.prev_hash, deserialized.prev_hash);
    ASSERT_EQ(block.created_ts, deserialized.created_ts);
    ASSERT_EQ(block.merkle_root, deserialized.merkle_root);

    ASSERT_TRUE(block.transactions.size() > 0);
    for (size_t i = 0; i < block.transactions.size(); i++){
      auto tx = block.transactions[i];
      auto des_tx = deserialized.transactions[i]; // deserialized tx

      ASSERT_TRUE(tx.signatures.size() > 0);
      for (size_t j = 0; j < tx.signatures.size(); j++){
        ASSERT_EQ(tx.signatures[j].pubkey, des_tx.signatures[j].pubkey);
        ASSERT_EQ(tx.signatures[j].signature, des_tx.signatures[j].signature);
      }

      ASSERT_EQ(tx.created_ts, des_tx.created_ts);
      ASSERT_EQ(tx.creator_account_id, des_tx.creator_account_id);
      ASSERT_EQ(tx.tx_counter, des_tx.tx_counter);

      for (size_t j = 0; j < tx.commands.size(); j++){
        if (instanceof<iroha::model::AddPeer>(tx.commands[j].get())){
          auto add_peer = static_cast<const iroha::model::AddPeer&>(*tx.commands[j].get());
          auto des_add_peer = static_cast<const iroha::model::AddPeer&>(*des_tx.commands[j].get());
          ASSERT_EQ(add_peer.address, des_add_peer.address);
          ASSERT_EQ(add_peer.peer_key, des_add_peer.peer_key);
        }
        else if (instanceof<iroha::model::AddAssetQuantity>(tx.commands[j].get())){
          auto add_asset_quantity = static_cast<const iroha::model::AddAssetQuantity&>(*tx.commands[j].get());
          auto des_add_asset_quantity = static_cast<const iroha::model::AddAssetQuantity&>(*des_tx.commands[j].get());

          ASSERT_EQ(add_asset_quantity.amount, des_add_asset_quantity.amount);
          ASSERT_EQ(add_asset_quantity.asset_id, des_add_asset_quantity.asset_id);
          ASSERT_EQ(add_asset_quantity.account_id, des_add_asset_quantity.account_id);
        }
        else if (instanceof<iroha::model::AddSignatory>(tx.commands[j].get())){
          auto add_signatory = static_cast<const iroha::model::AddSignatory&>(*tx.commands[j].get());
          auto des_add_signatory = static_cast<const iroha::model::AddSignatory&>(*des_tx.commands[j].get());
          ASSERT_EQ(add_signatory.account_id, des_add_signatory.account_id);
          ASSERT_EQ(add_signatory.pubkey, des_add_signatory.pubkey);
        }
        else if (instanceof<iroha::model::AssignMasterKey>(tx.commands[j].get())){
          auto assign_master_key = static_cast<const iroha::model::AssignMasterKey&>(*tx.commands[j].get());
          auto des_assign_master_key = static_cast<const iroha::model::AssignMasterKey&>(*des_tx.commands[j].get());
          ASSERT_EQ(assign_master_key.account_id, des_assign_master_key.account_id);
          ASSERT_EQ(assign_master_key.pubkey, des_assign_master_key.pubkey);
        }
        else if (instanceof<iroha::model::CreateAccount>(tx.commands[j].get())){
          auto create_account = static_cast<const iroha::model::CreateAccount&>(*tx.commands[j].get());
          auto des_create_account = static_cast<const iroha::model::CreateAccount&>(*des_tx.commands[j].get());
          ASSERT_EQ(create_account.account_name, des_create_account.account_name);
          ASSERT_EQ(create_account.domain_id, des_create_account.domain_id);
          ASSERT_EQ(create_account.pubkey, des_create_account.pubkey);
        }
        else if (instanceof<iroha::model::CreateAsset>(tx.commands[j].get())) {
          auto create_asset = static_cast<const iroha::model::CreateAsset&>(*tx.commands[j].get());
          auto des_create_asset = static_cast<const iroha::model::CreateAsset&>(*des_tx.commands[j].get());
          ASSERT_EQ(create_asset.asset_name, des_create_asset.asset_name);
          ASSERT_EQ(create_asset.domain_id, des_create_asset.domain_id);
          ASSERT_EQ(create_asset.precision, des_create_asset.precision);
        }
        else if (instanceof<iroha::model::CreateDomain>(tx.commands[j].get())) {
          auto create_domain = static_cast<const iroha::model::CreateDomain&>(*tx.commands[j].get());
          auto des_create_domain = static_cast<const iroha::model::CreateDomain&>(*des_tx.commands[j].get());
          ASSERT_EQ(create_domain.domain_name, des_create_domain.domain_name);
        }
        else if (instanceof<iroha::model::RemoveSignatory>(tx.commands[j].get())) {
          auto remove_signatory = static_cast<const iroha::model::RemoveSignatory&>(*tx.commands[j].get());
          auto des_remove_signatory = static_cast<const iroha::model::RemoveSignatory&>(*des_tx.commands[j].get());
          ASSERT_EQ(remove_signatory.account_id, des_remove_signatory.account_id);
          ASSERT_EQ(remove_signatory.pubkey, des_remove_signatory.pubkey);
        }
        else if (instanceof<iroha::model::SetAccountPermissions>(tx.commands[j].get())) {
          auto set_account_permissions = static_cast<const iroha::model::SetAccountPermissions&>(*tx.commands[j].get());
          auto des_set_account_permissions = static_cast<const iroha::model::SetAccountPermissions&>(*des_tx.commands[j].get());
          ASSERT_EQ(set_account_permissions.account_id, des_set_account_permissions.account_id);
          ASSERT_EQ(set_account_permissions.new_permissions.add_signatory, des_set_account_permissions.new_permissions.add_signatory);
          ASSERT_EQ(set_account_permissions.new_permissions.can_transfer, des_set_account_permissions.new_permissions.can_transfer);
          ASSERT_EQ(set_account_permissions.new_permissions.create_accounts, des_set_account_permissions.new_permissions.create_accounts);
          ASSERT_EQ(set_account_permissions.new_permissions.create_assets, des_set_account_permissions.new_permissions.create_assets);
          ASSERT_EQ(set_account_permissions.new_permissions.create_domains, des_set_account_permissions.new_permissions.create_domains);
          ASSERT_EQ(set_account_permissions.new_permissions.issue_assets, des_set_account_permissions.new_permissions.issue_assets);
          ASSERT_EQ(set_account_permissions.new_permissions.read_all_accounts, des_set_account_permissions.new_permissions.read_all_accounts);
          ASSERT_EQ(set_account_permissions.new_permissions.remove_signatory, des_set_account_permissions.new_permissions.remove_signatory);
          ASSERT_EQ(set_account_permissions.new_permissions.set_permissions, des_set_account_permissions.new_permissions.set_permissions);
          ASSERT_EQ(set_account_permissions.new_permissions.set_quorum, des_set_account_permissions.new_permissions.set_quorum);
        }
        else if (instanceof<iroha::model::SetQuorum>(tx.commands[j].get())) {
          auto set_quorum = static_cast<const iroha::model::SetQuorum&>(*tx.commands[j].get());
          auto des_set_quorum = static_cast<const iroha::model::SetQuorum&>(*des_tx.commands[j].get());
          ASSERT_EQ(set_quorum.account_id, des_set_quorum.account_id);
          ASSERT_EQ(set_quorum.new_quorum, des_set_quorum.new_quorum);
        }
        else if (instanceof<iroha::model::TransferAsset>(tx.commands[j].get())){
          auto transfer_asset = static_cast<const iroha::model::TransferAsset&>(*tx.commands[j].get());
          auto des_transfer_asset = static_cast<const iroha::model::TransferAsset&>(*des_tx.commands[j].get());
          ASSERT_EQ(transfer_asset.src_account_id, des_transfer_asset.src_account_id);
          ASSERT_EQ(transfer_asset.dest_account_id, des_transfer_asset.dest_account_id);
          ASSERT_EQ(transfer_asset.asset_id, des_transfer_asset.asset_id);
          ASSERT_EQ(transfer_asset.amount, des_transfer_asset.amount);
        }
      }

    }

  }
}
