/**
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
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

#include <generated/main_generated.h>
#include <infra/service/flatbuffers/autogen_extend.h>
#include <utils/expected.hpp>

#include <iostream>
#include <map>
#include <memory>
#include <string>

namespace flatbuffer_service {

/**
 * toString
 * - it returns string dump of arguments' tx like DebugString in protocol
 * buffer. ToDo If transaction scheme is changed, We changes this code.
 */
std::string toString(const iroha::Transaction& tx) {
  std::string res = "";
  if (tx.creatorPubKey() != nullptr) {
    res += "creatorPubKey:" + tx.creatorPubKey()->str() + ",\n";
  }
  if (tx.signatures() != nullptr) {
    res += "signatures:[\n";
    for (const auto& s : *tx.signatures()) {
      if (s->publicKey() != nullptr || s->signature() != nullptr) {
        res += "  [\n    publicKey:" + s->publicKey()->str() + ",\n";
        res += "    signature:" +
               std::string(s->signature()->begin(), s->signature()->end()) +
               ",\n";
        res += "    timestamp:" + std::to_string(s->timestamp()) + "\n  ]\n";
      } else {
        res += "[brolen]\n";
      }
    }
    res += "]\n";
  }
  if (tx.attachment() != nullptr) {
    assert(tx.attachment()->mime() != nullptr);
    assert(tx.attachment()->data() != nullptr);

    res += "attachment:[\n";
    res += " mime:" +
           std::string(tx.attachment()->mime()->begin(),
                       tx.attachment()->mime()->end()) +
           ",\n";
    res += " data:" +
           std::string(tx.attachment()->data()->begin(),
                       tx.attachment()->data()->end()) +
           ",\n";
    res += "]\n";
  }

  std::map<iroha::Command, std::function<std::string(const void*)>>
      command_to_strings;
  std::map<iroha::AnyAsset, std::function<std::string(const void*)>>
      any_asset_to_strings;

  any_asset_to_strings[iroha::AnyAsset_ComplexAsset] =
      [&](const void* asset) -> std::string {
    const iroha::ComplexAsset* ast =
        static_cast<const iroha::ComplexAsset*>(asset);

    std::string res = " ComplexAsset[\n";
    res += "        asset_name:" + ast->asset_name()->str() + ",\n";
    res += "        domain_name:" + ast->domain_name()->str() + ",\n";
    res += "        ledger_name:" + ast->ledger_name()->str() + ",\n";
    res += "        description:" + ast->description()->str() + "\n";
    res += "        asset:logic:WIP\n";
    res += "    ]\n";
    return res;
  };
  any_asset_to_strings[iroha::AnyAsset_Currency] =
      [&](const void* asset) -> std::string {
    const iroha::Currency* ast = static_cast<const iroha::Currency*>(asset);

    std::string res = " Currency[\n";
    res += "        currency_name:" + ast->currency_name()->str() + ",\n";
    res += "        domain_name:" + ast->domain_name()->str() + ",\n";
    res += "        ledger_name:" + ast->ledger_name()->str() + ",\n";
    res += "        description:" + ast->description()->str() + "\n";
    res += "        amount:" + std::to_string(ast->amount()) + "\n";
    res += "        precision:" + std::to_string(ast->precision()) + "\n";
    res += "    ]\n";
    return res;
  };

  command_to_strings[iroha::Command_AssetCreate] =
      [&](const void* command) -> std::string {
    const iroha::AssetCreate* cmd =
        static_cast<const iroha::AssetCreate*>(command);

    std::string res = "AssetCreate[\n";
    res += "    creatorPubKey:" + cmd->creatorPubKey()->str() + ",\n";
    res += "    ledger_name:" + cmd->ledger_name()->str() + ",\n";
    res += "    domain_name:" + cmd->domain_name()->str() + ",\n";
    res += "    asset_name:" + cmd->asset_name()->str() + "\n";
    res += "]\n";
    return res;
  };

  command_to_strings[iroha::Command_AssetCreate] =
      [&](const void* command) -> std::string {
    const iroha::AssetCreate* cmd =
        static_cast<const iroha::AssetCreate*>(command);

    std::string res = "AssetCreate[\n";
    res += "    creatorPubKey:" + cmd->creatorPubKey()->str() + ",\n";
    res += "    ledger_name:" + cmd->ledger_name()->str() + ",\n";
    res += "    domain_name:" + cmd->domain_name()->str() + ",\n";
    res += "    asset_name:" + cmd->asset_name()->str() + "\n";
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_AssetAdd] =
      [&](const void* command) -> std::string {
    const iroha::AssetAdd* cmd = static_cast<const iroha::AssetAdd*>(command);

    std::string res = "AssetAdd[\n";
    res += "    accPubKey:" + cmd->accPubKey()->str() + ",\n";
    res += "    asset:" +
           any_asset_to_strings[cmd->asset_nested_root()->asset_type()](
               cmd->asset_nested_root()->asset());
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_AssetRemove] =
      [&](const void* command) -> std::string {
    const iroha::AssetRemove* cmd =
        static_cast<const iroha::AssetRemove*>(command);

    std::string res = "AssetRemove[\n";
    res += "    accPubKey:" + cmd->accPubKey()->str() + ",\n";
    res += "    asset:" +
           any_asset_to_strings[cmd->asset_nested_root()->asset_type()](
               cmd->asset_nested_root()->asset());
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_AssetTransfer] =
      [&](const void* command) -> std::string {
    const iroha::AssetTransfer* cmd =
        static_cast<const iroha::AssetTransfer*>(command);

    std::string res = "AssetTransfer[\n";
    res += "    sender:" + cmd->sender()->str() + ",\n";
    res += "    receiver:" + cmd->receiver()->str() + ",\n";
    res += "    asset:" +
           any_asset_to_strings[cmd->asset_nested_root()->asset_type()](
               cmd->asset_nested_root()->asset());
    res += "]\n";
    return res;
  };

  command_to_strings[iroha::Command_PeerAdd] =
      [&](const void* command) -> std::string {
    const iroha::PeerAdd* cmd = static_cast<const iroha::PeerAdd*>(command);

    std::string res = "PeerAdd[\n";
    res += "    peer:publicKey:" + cmd->peer_nested_root()->publicKey()->str() +
           ",\n";
    res += "    peer:ip:" + cmd->peer_nested_root()->ip()->str() + ",\n";
    res += "    peer:active:" +
           std::to_string(cmd->peer_nested_root()->active()) + ",\n";
    res += "    peer:join_network:" +
           std::to_string(cmd->peer_nested_root()->join_network()) + ",\n";
    res += "    peer:join_validation:" +
           std::to_string(cmd->peer_nested_root()->join_validation()) + "\n";
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_PeerRemove] =
      [&](const void* command) -> std::string {
    const iroha::PeerRemove* cmd =
        static_cast<const iroha::PeerRemove*>(command);

    std::string res = "PeerRemove[\n";
    res += "    peer:publicKey:" + cmd->peer_nested_root()->publicKey()->str() +
           ",\n";
    res += "    peer:ip:" + cmd->peer_nested_root()->ip()->str() + ",\n";
    res += "    peer:active:" +
           std::to_string(cmd->peer_nested_root()->active()) + ",\n";
    res += "    peer:join_network:" +
           std::to_string(cmd->peer_nested_root()->join_network()) + ",\n";
    res += "    peer:join_validation:" +
           std::to_string(cmd->peer_nested_root()->join_validation()) + "\n";
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_PeerSetActive] =
      [&](const void* command) -> std::string {
    const iroha::PeerSetActive* cmd =
        static_cast<const iroha::PeerSetActive*>(command);

    std::string res = "PeerSetActive[\n";
    res += "    peer:peerPubKey:" + cmd->peerPubKey()->str() + ",\n";
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_PeerSetTrust] =
      [&](const void* command) -> std::string {
    const iroha::PeerSetTrust* cmd =
        static_cast<const iroha::PeerSetTrust*>(command);

    std::string res = "PeerSetTrust[\n";
    res += "    peerPubKey:" + cmd->peerPubKey()->str() + ",\n";
    res += "    trust:" + std::to_string(cmd->trust()) + ",\n";
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_PeerChangeTrust] =
      [&](const void* command) -> std::string {
    const iroha::PeerChangeTrust* cmd =
        static_cast<const iroha::PeerChangeTrust*>(command);

    std::string res = "PeerSetTrust[\n";
    res += "    peerPubKey:" + cmd->peerPubKey()->str() + ",\n";
    res += "    delta:" + std::to_string(cmd->delta()) + "\n";
    res += "]\n";
    return res;
  };

  command_to_strings[iroha::Command_AccountAdd] =
      [&](const void* command) -> std::string {
    const iroha::AccountAdd* cmd =
        static_cast<const iroha::AccountAdd*>(command);
    std::string res = "AccountAdd[\n";
    if (cmd->account_nested_root() != nullptr) {
      if (cmd->account_nested_root()->alias() != 0) {
        res += "    account:alias:" +
               cmd->account_nested_root()->alias()->str() + ",\n";
      }
      if (cmd->account_nested_root()->pubKey() != nullptr) {
        res += "    account:pubKey:" +
               cmd->account_nested_root()->pubKey()->str() + ",\n";
      }
      if (cmd->account_nested_root()->signatories() != nullptr) {
        for (const auto& s : *cmd->account_nested_root()->signatories()) {
          res += "        signature[" + s->str() + "]\n";
        }
      }
    }
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_AccountRemove] =
      [&](const void* command) -> std::string {
    const iroha::AccountRemove* cmd =
        static_cast<const iroha::AccountRemove*>(command);

    std::string res = "AccountRemove[\n";
    if (cmd->account_nested_root() != nullptr) {
      if (cmd->account_nested_root()->alias() != nullptr) {
        res += "    account:alias:" +
               cmd->account_nested_root()->alias()->str() + ",\n";
      }
      if (cmd->account_nested_root()->pubKey() != nullptr) {
        res += "    account:pubKey:" +
               cmd->account_nested_root()->pubKey()->str() + ",\n";
      }
      if (cmd->account_nested_root()->signatories() != nullptr) {
        for (const auto& s : *cmd->account_nested_root()->signatories()) {
          res += "        signature[" + s->str() + "]\n";
        }
      }
      res += "    account:useKeys:" +
             std::to_string(cmd->account_nested_root()->useKeys()) + "\n";
    }
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_AccountAddSignatory] =
      [&](const void* command) -> std::string {
    const iroha::AccountAddSignatory* cmd =
        static_cast<const iroha::AccountAddSignatory*>(command);

    std::string res = "AccountAddSignatory[\n";
    res += "    account:" + cmd->account()->str() + ",\n";
    for (const auto& s : *cmd->signatory()) {
      res += "        signature[" + s->str() + "]\n";
    }
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_AccountRemoveSignatory] =
      [&](const void* command) -> std::string {
    const iroha::AccountRemoveSignatory* cmd =
        static_cast<const iroha::AccountRemoveSignatory*>(command);

    std::string res = "AccountRemoveSignatory[\n";
    res += "    account:" + cmd->account()->str() + ",\n";
    for (const auto& s : *cmd->signatory()) {
      res += "        signature[" + s->str() + "]\n";
    }
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_AccountSetUseKeys] =
      [&](const void* command) -> std::string {
    const iroha::AccountSetUseKeys* cmd =
        static_cast<const iroha::AccountSetUseKeys*>(command);

    std::string res = "AccountSetUseKeys[\n";
    for (const auto& a : *cmd->accounts()) {
      res += "        account[" + a->str() + "]\n";
    }
    res += "    account:useKeys:" + std::to_string(cmd->useKeys()) + "\n";
    res += "]\n";
    return res;
  };

  command_to_strings[iroha::Command_ChaincodeAdd] =
      [&](const void* command) -> std::string {
    const iroha::ChaincodeAdd* cmd =
        static_cast<const iroha::ChaincodeAdd*>(command);

    std::string res = "ChaincodeAdd[\n";
    res += "]\n";
    return res;
  };
  command_to_strings[iroha::Command_ChaincodeRemove] =
      [&](const void* command) -> std::string {
    const iroha::ChaincodeRemove* cmd =
        static_cast<const iroha::ChaincodeRemove*>(command);

    std::string res = "ChaincodeRemove[\n";
    res += "]\n";
  };
  command_to_strings[iroha::Command_ChaincodeExecute] =
      [&](const void* command) -> std::string {
    const iroha::ChaincodeExecute* cmd =
        static_cast<const iroha::ChaincodeExecute*>(command);

    std::string res = "ChaincodeExecute[\n";
    res += "]\n";
  };
  res += command_to_strings[tx.command_type()](tx.command());
  return res;
}

/**
 * toConsensusEvent
 * - Encapsulate a transaction in a consensus event. Argument fromTx will be
 *   deeply copied and create new consensus event that has the copied
 * transaction.
 */
flatbuffers::unique_ptr_t toConsensusEvent(const iroha::Transaction& fromTx) {
  flatbuffers::FlatBufferBuilder fbb;
  fbb.Clear();

  std::vector<flatbuffers::Offset<::iroha::Signature>> signatures;
  std::vector<flatbuffers::Offset<::iroha::Transaction>> transactions;

  std::vector<uint8_t> _hash;
  std::vector<uint8_t> data;
  flatbuffers::Offset<::iroha::Attachment> attachment = 0;

  if (fromTx.signatures() != nullptr) {
    for (const auto& s : *fromTx.signatures()) {
      assert(s->publicKey() != nullptr);
      assert(s->signature() != nullptr);
    }
  }
  if (fromTx.attachment() != nullptr) {
    assert(fromTx.attachment()->mime() != nullptr);
    assert(fromTx.attachment()->data() != nullptr);
  }

  if (fromTx.hash() != nullptr) {
    _hash.assign(fromTx.hash()->begin(), fromTx.hash()->end());
  }
  if (fromTx.attachment() != nullptr &&
      fromTx.attachment()->data() != nullptr) {
    data.assign(fromTx.attachment()->data()->begin(),
                fromTx.attachment()->data()->end());
    attachment = iroha::CreateAttachmentDirect(
        fbb, fromTx.attachment()->mime()->c_str(), &data);
  }


  std::vector<flatbuffers::Offset<::iroha::Signature>> tx_signatures;

  if (fromTx.signatures() != nullptr) {
    for (auto&& txSig : *fromTx.signatures()) {
      std::vector<uint8_t> _data;
      if (txSig->signature() != nullptr) {
        for (auto d : *txSig->signature()) {
          _data.emplace_back(d);
        }
        tx_signatures.emplace_back(iroha::CreateSignatureDirect(
            fbb, txSig->publicKey()->c_str(), &_data));
      }
    }
  }

  // TODO: Currently, #(transaction) is one.
  transactions.push_back(::iroha::CreateTransactionDirect(
      fbb, fromTx.creatorPubKey()->c_str(), fromTx.command_type(),
      flatbuffer_service::CreateCommandDirect(fbb, fromTx.command(),
                                              fromTx.command_type()),
      &tx_signatures, &_hash, attachment));

  auto consensusEventOffset = ::iroha::CreateConsensusEventDirect(
      fbb, &signatures, &transactions, ::iroha::Code_UNDECIDED);

  fbb.Finish(consensusEventOffset);
  return fbb.ReleaseBufferPointer();
}


flatbuffers::unique_ptr_t addSignature(const iroha::ConsensusEvent& event,
                                       const std::string& publicKey,
                                       const std::string& signature) {
  flatbuffers::FlatBufferBuilder fbbConsensusEvent(16);

  // At first, peerSignatures is empty. (Is this right?)
  std::vector<flatbuffers::Offset<iroha::Signature>> peerSignatures;
  std::vector<flatbuffers::Offset<iroha::Signature>> signatures;

  // Tempolary implementation: Currently, #(tx) is one.
  auto tx = event.transactions()->Get(0);
  const auto& aSignature = tx->signatures()->Get(0);
  const auto& aPeerSignatures = event.peerSignatures();

  for (const auto& aPeerSig : *event.peerSignatures()) {
    std::vector<uint8_t> aPeerSigBlob(aPeerSig->signature()->begin(),
                                      aPeerSig->signature()->end());
    peerSignatures.push_back(::iroha::CreateSignatureDirect(
        fbbConsensusEvent, aPeerSig->publicKey()->c_str(), &aPeerSigBlob,
        1234567));
  }

  std::vector<uint8_t> aNewPeerSigBlob;
  for (auto&& c : signature) {
    aNewPeerSigBlob.push_back(c);
  }
  peerSignatures.push_back(::iroha::CreateSignatureDirect(
      fbbConsensusEvent, aSignature->publicKey()->c_str(), &aNewPeerSigBlob,
      1234567));


  std::vector<uint8_t> signatureBlob(aSignature->signature()->begin(),
                                     aSignature->signature()->end());

  signatures.push_back(::iroha::CreateSignatureDirect(
      fbbConsensusEvent, aSignature->publicKey()->c_str(), &signatureBlob,
      1234567));

  std::vector<uint8_t> hashes;
  if (tx->hash() != nullptr) {
    hashes.assign(tx->hash()->begin(), tx->hash()->end());
  }

  flatbuffers::Offset<::iroha::Attachment> attachmentOffset;
  std::vector<uint8_t> data;
  if (tx->attachment() != nullptr && tx->attachment()->data() != nullptr &&
      tx->attachment()->mime() != nullptr) {
    data.assign(tx->attachment()->data()->begin(),
                tx->attachment()->data()->end());

    attachmentOffset = ::iroha::CreateAttachmentDirect(
        fbbConsensusEvent, tx->attachment()->mime()->c_str(), &data);
  }

  std::vector<flatbuffers::Offset<iroha::Transaction>> transactions;

  // TODO: Currently, #(transaction) is one.
  transactions.push_back(::iroha::CreateTransactionDirect(
      fbbConsensusEvent, tx->creatorPubKey()->c_str(), tx->command_type(),
      flatbuffer_service::CreateCommandDirect(fbbConsensusEvent, tx->command(),
                                              tx->command_type()),
      &signatures, &hashes, attachmentOffset));

  auto consensusEventOffset = ::iroha::CreateConsensusEventDirect(
      fbbConsensusEvent, &peerSignatures, &transactions, event.code());

  fbbConsensusEvent.Finish(consensusEventOffset);
  return fbbConsensusEvent.ReleaseBufferPointer();
}

flatbuffers::unique_ptr_t makeCommit(const iroha::ConsensusEvent& event) {
  flatbuffers::FlatBufferBuilder fbbConsensusEvent(16);

  // At first, peerSignatures is empty. (Is this right?)
  std::vector<flatbuffers::Offset<iroha::Signature>> peerSignatures;
  std::vector<flatbuffers::Offset<iroha::Signature>> signatures;

  // Tempolary implementation: Currently, #(tx) is one.
  auto tx = event.transactions()->Get(0);
  const auto& aSignature = tx->signatures()->Get(0);
  const auto& aPeerSignatures = event.peerSignatures();

  for (const auto& aPeerSig : *event.peerSignatures()) {
    std::vector<uint8_t> aPeerSigBlob(aPeerSig->signature()->begin(),
                                      aPeerSig->signature()->end());
    peerSignatures.push_back(::iroha::CreateSignatureDirect(
        fbbConsensusEvent, aPeerSig->publicKey()->c_str(), &aPeerSigBlob,
        1234567));
  }

  std::vector<uint8_t> signatureBlob(aSignature->signature()->begin(),
                                     aSignature->signature()->end());

  signatures.push_back(::iroha::CreateSignatureDirect(
      fbbConsensusEvent, aSignature->publicKey()->c_str(), &signatureBlob,
      1234567));

  std::vector<uint8_t> hashes;
  if (tx->hash() != nullptr) {
    hashes.assign(tx->hash()->begin(), tx->hash()->end());
  }

  flatbuffers::Offset<::iroha::Attachment> attachmentOffset;
  std::vector<uint8_t> data;
  if (tx->attachment() != nullptr && tx->attachment()->data() != nullptr &&
      tx->attachment()->mime() != nullptr) {
    data.assign(tx->attachment()->data()->begin(),
                tx->attachment()->data()->end());

    attachmentOffset = ::iroha::CreateAttachmentDirect(
        fbbConsensusEvent, tx->attachment()->mime()->c_str(), &data);
  }

  std::vector<flatbuffers::Offset<iroha::Transaction>> transactions;

  // TODO: Currently, #(transaction) is one.
  transactions.push_back(::iroha::CreateTransactionDirect(
      fbbConsensusEvent, tx->creatorPubKey()->c_str(), tx->command_type(),
      flatbuffer_service::CreateCommandDirect(fbbConsensusEvent, tx->command(),
                                              tx->command_type()),
      &signatures, &hashes, attachmentOffset));

  auto consensusEventOffset = ::iroha::CreateConsensusEventDirect(
      fbbConsensusEvent, &peerSignatures, &transactions, iroha::Code_COMMIT);

  fbbConsensusEvent.Finish(consensusEventOffset);
  return fbbConsensusEvent.ReleaseBufferPointer();
}


Expected<int> RequirePropertyMissingException(const iroha::Transaction &tx){
  if (tx.creatorPubKey() == nullptr) {
    return makeUnexpected(exception::RequirePropertyMissingException("Transaction must need creatorPubKey", __FILE__));
  }
  if (tx.signatures() != nullptr) {
    for (const auto& s : *tx.signatures()) {
      if (s->publicKey() == nullptr || s->signature() == nullptr){
        if(s->publicKey() == nullptr) {
          return makeUnexpected(
             exception::RequirePropertyMissingException("Tx signatures must need publicKey", __FILE__)
          );
        }else{
          return makeUnexpected(
                  exception::RequirePropertyMissingException("Tx signatures must need signature", __FILE__)
          );
        }
      }
    }
  }
  if (tx.attachment() != nullptr) {
    if(tx.attachment()->mime() != nullptr){
      return makeUnexpected(
          exception::RequirePropertyMissingException("Tx attachment must need mime", __FILE__)
      );
    }
    if(tx.attachment()->data() != nullptr){
      return makeUnexpected(
          exception::RequirePropertyMissingException("Tx attachment must need data", __FILE__)
      );
    }
  }

  std::map<iroha::Command, std::function<Expected<int>(const void*)>>
          command_validators;
  std::map<iroha::AnyAsset, std::function<Expected<int>(const void*)>>
          any_asset_validators;

  auto peer_validator = [](const iroha::Peer* peer) -> Expected<int> {
    if(peer == nullptr){
      return makeUnexpected(
        exception::RequirePropertyMissingException("Peer must not nullptr", __FILE__)
      );
    }
    if(peer->ip() == nullptr){
      return makeUnexpected(
              exception::RequirePropertyMissingException("Peer ip must not nullptr", __FILE__)
      );
    }
    return 1;
  };
  auto account_validator = [](const iroha::Account* account) -> Expected<int> {
      if(account == nullptr){
        return makeUnexpected(
                exception::RequirePropertyMissingException("Account must not nullptr", __FILE__)
        );
      }
      if(account->alias() == nullptr){
        return makeUnexpected(
                exception::RequirePropertyMissingException("Account alias must not nullptr", __FILE__)
        );
      }
      if(account->pubKey() == nullptr){
        return makeUnexpected(
                exception::RequirePropertyMissingException("Account pubKey must not nullptr", __FILE__)
        );
      }
      return 1;
  };
  any_asset_validators[iroha::AnyAsset_ComplexAsset] =
    [&](const void* asset) -> Expected<int> {
        const iroha::ComplexAsset* ast =
                static_cast<const iroha::ComplexAsset*>(asset);
        if(ast == nullptr){
          return makeUnexpected(
              exception::RequirePropertyMissingException("ComplexAsset must not nullptr", __FILE__)
          );
        }
        if(ast->asset_name() == nullptr){
          return makeUnexpected(
              exception::RequirePropertyMissingException("ComplexAsset asset_name must not nullptr", __FILE__)
          );
        }
        if(ast->domain_name() == nullptr){
          return makeUnexpected(
              exception::RequirePropertyMissingException("ComplexAsset domain_name must not nullptr", __FILE__)
          );
        }
        if(ast->ledger_name() == nullptr){
          return makeUnexpected(
              exception::RequirePropertyMissingException("ComplexAsset ledger_name must not nullptr", __FILE__)
          );
        }
        if(ast->description() == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("ComplexAsset description must not nullptr", __FILE__)
          );
        }
        return 1;
    };

  any_asset_validators[iroha::AnyAsset_Currency] =
    [&](const void* asset) -> Expected<int> {
        const iroha::Currency* ast = static_cast<const iroha::Currency*>(asset);

        if(ast == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("Currency must not nullptr", __FILE__)
          );
        }
        if(ast->currency_name() == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("Currency currency_name asset_name must not nullptr", __FILE__)
          );
        }
        if(ast->domain_name() == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("Currency domain_name asset_name must not nullptr", __FILE__)
          );
        }
        if(ast->ledger_name() == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("Currency ledger_name asset_name must not nullptr", __FILE__)
          );
        }
        return 1;
    };

  command_validators[iroha::Command_AssetCreate] =
    [&](const void* command) -> Expected<int> {
        const iroha::AssetCreate* cmd =
                static_cast<const iroha::AssetCreate*>(command);
        if(cmd == nullptr){
          return makeUnexpected(
              exception::RequirePropertyMissingException("AssetCreate must not nullptr", __FILE__)
          );
        }
        if(cmd->creatorPubKey() == nullptr){
          return makeUnexpected(
              exception::RequirePropertyMissingException("AssetCreate creatorPubKey asset_name must not nullptr", __FILE__)
          );
        }
        if(cmd->ledger_name() == nullptr){
          return makeUnexpected(
              exception::RequirePropertyMissingException("AssetCreate ledger_name asset_name must not nullptr", __FILE__)
          );
        }
        if(cmd->domain_name() == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("AssetCreate domain_name asset_name must not nullptr", __FILE__)
          );
        }
        if(cmd->asset_name() == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("AssetCreate asset_name asset_name must not nullptr", __FILE__)
          );
        }
        return 1;
    };

  command_validators[iroha::Command_AssetAdd] =
    [&](const void* command) -> Expected<int> {
        const iroha::AssetAdd* cmd = static_cast<const iroha::AssetAdd*>(command);
        if(cmd == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("AssetAdd asset_name must not nullptr", __FILE__)
          );
        }
        if(cmd->accPubKey() == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("AssetAdd accPubKey must not nullptr", __FILE__)
          );
        }
        return any_asset_validators[cmd->asset_nested_root()->asset_type()]
             (cmd->asset_nested_root()->asset());
    };
  command_validators[iroha::Command_AssetRemove] =
    [&](const void* command) -> Expected<int> {
        const iroha::AssetRemove* cmd =
                static_cast<const iroha::AssetRemove*>(command);
        if(cmd == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("AssetRemove must not nullptr", __FILE__)
          );
        }
        if(cmd->accPubKey() == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("AssetRemove accPubKey must not nullptr", __FILE__)
          );
        }
        return any_asset_validators[cmd->asset_nested_root()->asset_type()]
          (cmd->asset_nested_root()->asset());
    };
  command_validators[iroha::Command_AssetTransfer] =
    [&](const void* command) -> Expected<int> {
        const iroha::AssetTransfer* cmd =
                static_cast<const iroha::AssetTransfer*>(command);

        if(cmd == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("AssetTransfer must not nullptr", __FILE__)
          );
        }
        if(cmd->sender() == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("AssetTransfer sender must not nullptr", __FILE__)
          );
        }
        if(cmd->receiver() == nullptr){
          return makeUnexpected(
                  exception::RequirePropertyMissingException("AssetTransfer receiver must not nullptr", __FILE__)
          );
        }
        return any_asset_validators[cmd->asset_nested_root()->asset_type()]
           (cmd->asset_nested_root()->asset());
    };

    command_validators[iroha::Command_PeerAdd] =
      [&](const void* command) -> Expected<int> {
          const iroha::PeerAdd* cmd = static_cast<const iroha::PeerAdd*>(command);
          if(cmd == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("PeerAdd must not nullptr", __FILE__)
            );
          }
          return peer_validator(cmd->peer_nested_root());
      };

  command_validators[iroha::Command_PeerRemove] =
      [&](const void* command) -> Expected<int> {
          const iroha::PeerRemove* cmd =
                  static_cast<const iroha::PeerRemove*>(command);
          if(cmd == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("PeerRemove must not nullptr", __FILE__)
            );
          }
          return peer_validator(cmd->peer_nested_root());
      };
  command_validators[iroha::Command_PeerSetActive] =
      [&](const void* command) -> Expected<int> {
          const iroha::PeerSetActive* cmd =
                  static_cast<const iroha::PeerSetActive*>(command);
          if(cmd == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("PeerRemove must not nullptr", __FILE__)
            );
          }
          if(cmd->peerPubKey() == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("PeerRemove peerPubKey must not nullptr", __FILE__)
            );
          }
          return 1;
      };

  command_validators[iroha::Command_PeerSetTrust] =
      [&](const void* command) -> Expected<int> {
          const iroha::PeerSetTrust* cmd =
                  static_cast<const iroha::PeerSetTrust*>(command);
          if(cmd == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("PeerSetTrust must not nullptr", __FILE__)
            );
          }
          if(cmd->peerPubKey() == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("PeerRemove peerPubKey must not nullptr", __FILE__)
            );
          }
          return 1;
      };
  command_validators[iroha::Command_PeerChangeTrust] =
      [&](const void* command) -> Expected<int> {
          const iroha::PeerChangeTrust* cmd =
                  static_cast<const iroha::PeerChangeTrust*>(command);
          if(cmd == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("PeerChangeTrust must not nullptr", __FILE__)
            );
          }
          if(cmd->peerPubKey() == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("PeerRemove peerPubKey must not nullptr", __FILE__)
            );
          }
          return 1;
      };

  command_validators[iroha::Command_AccountAdd] =
      [&](const void* command) -> Expected<int> {
          const iroha::AccountAdd* cmd =
                  static_cast<const iroha::AccountAdd*>(command);
          if(cmd == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("AccountAdd must not nullptr", __FILE__)
            );
          }
          return account_validator(cmd->account_nested_root());
      };

  command_validators[iroha::Command_AccountRemove] =
      [&](const void* command) -> Expected<int> {
          const iroha::AccountRemove* cmd =
                  static_cast<const iroha::AccountRemove*>(command);
          if(cmd == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("AccountRemove must not nullptr", __FILE__)
            );
          }
          return account_validator(cmd->account_nested_root());
      };
  command_validators[iroha::Command_AccountAddSignatory] =
      [&](const void* command) -> Expected<int> {
          const iroha::AccountAddSignatory* cmd =
                  static_cast<const iroha::AccountAddSignatory*>(command);
          if(cmd == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("AccountAddSignatory must not nullptr", __FILE__)
            );
          }
          if(cmd->account() == nullptr){
            return makeUnexpected(
                    exception::RequirePropertyMissingException("AccountAddSignatory account peerPubKey must not nullptr", __FILE__)
            );
          }
          return 1;
      };

  command_validators[iroha::Command_AccountRemoveSignatory] =
          [&](const void* command) -> Expected<int> {
              const iroha::AccountRemoveSignatory* cmd =
                      static_cast<const iroha::AccountRemoveSignatory*>(command);
              if(cmd == nullptr){
                return makeUnexpected(
                        exception::RequirePropertyMissingException("AccountRemoveSignatory must not nullptr", __FILE__)
                );
              }
              if(cmd->account() == nullptr){
                return makeUnexpected(
                        exception::RequirePropertyMissingException("AccountAddSignatory account peerPubKey must not nullptr", __FILE__)
                );
              }
              return 1;
          };
  command_validators[iroha::Command_AccountSetUseKeys] =
          [&](const void* command) -> Expected<int> {
              const iroha::AccountSetUseKeys* cmd =
                      static_cast<const iroha::AccountSetUseKeys*>(command);
              if(cmd == nullptr){
                return makeUnexpected(
                        exception::RequirePropertyMissingException("AccountSetUseKeys must not nullptr", __FILE__)
                );
              }
              return 1;
          };

  command_validators[iroha::Command_ChaincodeAdd] =
          [&](const void* command) -> Expected<int> {
              const iroha::ChaincodeAdd* cmd =
                      static_cast<const iroha::ChaincodeAdd*>(command);
              return makeUnexpected(
                      exception::NotImplementedException("ChaincodeAdd", __FILE__)
              );
          };
  command_validators[iroha::Command_ChaincodeRemove] =
          [&](const void* command) -> Expected<int> {
              const iroha::ChaincodeRemove* cmd =
                      static_cast<const iroha::ChaincodeRemove*>(command);
              return makeUnexpected(
                      exception::NotImplementedException("ChaincodeAdd", __FILE__)
              );
          };
  command_validators[iroha::Command_ChaincodeExecute] =
          [&](const void* command) -> Expected<int> {
              const iroha::ChaincodeExecute* cmd =
                      static_cast<const iroha::ChaincodeExecute*>(command);
              return makeUnexpected(
                      exception::NotImplementedException("ChaincodeAdd", __FILE__)
              );
          };
  return command_validators[tx.command_type()](tx.command());
}

}  // namespace flatbuffer_service
