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

#include <infra/flatbuf/main_generated.h>
#include "autogen_extend.h"

#include <memory>
#include <string>

namespace flatbuffer_service {

std::string toString(const iroha::Transaction& tx){
    assert(tx.creatorPubKey() != nullptr);
    std::string res = "";
    res += "creatorPubKey:" + tx.creatorPubKey()->str() + ",\n";
    if(tx.signatures() != nullptr){
        res += "signatures:[\n";
        for(const auto& s: *tx.signatures()){
            assert(s->publicKey() != nullptr);
            assert(s->signature() != nullptr);

            res += "  [\n    publicKey:" + s->publicKey()->str() + ",\n";
            res += "    signature:" + std::string(reinterpret_cast<const char*>(s->signature()->Data())) + ",\n";
            res += "    timestamp:" + std::to_string(s->timestamp()) +  "\n  ]\n";
        }
        res += "]\n";
    }
    if(tx.attachment() != nullptr){
        assert(tx.attachment()->mime() != nullptr);
        assert(tx.attachment()->data() != nullptr);

        res += "attachment:[\n";
        res += " mime:" + std::string(reinterpret_cast<const char*>(tx.attachment()->mime()->Data())) + ",\n";
        res += " data:" + std::string(reinterpret_cast<const char*>(tx.attachment()->data()->Data())) + ",\n";
        res += "]\n";
    }

    std::map<iroha::Command, std::function<std::string(const void *)>> command_to_strings;
    std::map<iroha::AnyAsset, std::function<std::string(const void *)>> any_asset_to_strings;

    any_asset_to_strings[iroha::AnyAsset_ComplexAsset] = [&](const void* asset) -> std::string{
        const iroha::ComplexAsset* ast = static_cast<const iroha::ComplexAsset *>(asset);

        std::string res = " ComplexAsset[\n";
        res += "        asset_name:" + ast->asset_name()->str() + ",\n";
        res += "        domain_name:" + ast->domain_name()->str() + ",\n";
        res += "        ledger_name:" + ast->ledger_name()->str() + ",\n";
        res += "        description:" + ast->description()->str()  + "\n";
        res += "        asset:logic:WIP\n";
        res += "    ]\n";
        return res;
    };
    any_asset_to_strings[iroha::AnyAsset_Currency] = [&](const void* asset) -> std::string{
        const iroha::Currency* ast = static_cast<const iroha::Currency *>(asset);

        std::string res = " Currency[\n";
        res += "        currency_name:" + ast->currency_name()->str() + ",\n";
        res += "        domain_name:" + ast->domain_name()->str() + ",\n";
        res += "        ledger_name:" + ast->ledger_name()->str() + ",\n";
        res += "        description:" + ast->description()->str()  + "\n";
        res += "        amount:" + std::to_string(ast->amount())  + "\n";
        res += "        precision:" + std::to_string(ast->precision())  + "\n";
        res += "    ]\n";
        return res;
    };

    command_to_strings[iroha::Command_AssetCreate] = [&](const void* command) -> std::string{
        const iroha::AssetCreate* cmd = static_cast<const iroha::AssetCreate *>(command);

        std::string res = "AssetCreate[\n";
        res += "    creatorPubKey:" + cmd->creatorPubKey()->str() + ",\n";
        res += "    ledger_name:" + cmd->ledger_name()->str() + ",\n";
        res += "    domain_name:" + cmd->domain_name()->str() + ",\n";
        res += "    asset_name:" + cmd->asset_name()->str() + "\n";
        res += "]\n";
        return res;
    };

    command_to_strings[iroha::Command_AssetCreate] = [&](const void* command) -> std::string{
        const iroha::AssetCreate* cmd = static_cast<const iroha::AssetCreate *>(command);

        std::string res = "AssetCreate[\n";
        res += "    creatorPubKey:" + cmd->creatorPubKey()->str() + ",\n";
        res += "    ledger_name:" + cmd->ledger_name()->str() + ",\n";
        res += "    domain_name:" + cmd->domain_name()->str() + ",\n";
        res += "    asset_name:" + cmd->asset_name()->str() + "\n";
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_AssetAdd] = [&](const void* command) -> std::string{
        const iroha::AssetAdd*    cmd = static_cast<const iroha::AssetAdd *>(command);

        std::string res = "AssetAdd[\n";
        res += "    accPubKey:" + cmd->accPubKey()->str() + ",\n";
        res += "    asset:" + any_asset_to_strings[cmd->asset_nested_root()->asset_type()](cmd->asset_nested_root()->asset());
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_AssetRemove] = [&](const void* command) -> std::string{
        const iroha::AssetRemove* cmd = static_cast<const iroha::AssetRemove *>(command);

        std::string res = "AssetRemove[\n";
        res += "    accPubKey:" + cmd->accPubKey()->str() + ",\n";
        res += "    asset:" + any_asset_to_strings[cmd->asset_nested_root()->asset_type()](cmd->asset_nested_root()->asset());
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_AssetTransfer] = [&](const void* command) -> std::string{
        const iroha::AssetTransfer* cmd = static_cast<const iroha::AssetTransfer *>(command);

        std::string res = "AssetTransfer[\n";
        res += "    sender:" + cmd->sender()->str() + ",\n";
        res += "    receiver:" + cmd->receiver()->str() + ",\n";
        res += "    asset:" + any_asset_to_strings[cmd->asset_nested_root()->asset_type()](cmd->asset_nested_root()->asset());
        res += "]\n";
        return res;
    };

    command_to_strings[iroha::Command_PeerAdd] = [&](const void* command) -> std::string{
        const iroha::PeerAdd* cmd = static_cast<const iroha::PeerAdd *>(command);

        std::string res = "PeerAdd[\n";
        res += "    peer:publicKey:" + cmd->peer_nested_root()->publicKey()->str() + ",\n";
        res += "    peer:ip:" + cmd->peer_nested_root()->ip()->str() + ",\n";
        res += "    peer:active:" + std::to_string(cmd->peer_nested_root()->active()) + ",\n";
        res += "    peer:join_network:" + std::to_string(cmd->peer_nested_root()->join_network()) + ",\n";
        res += "    peer:join_validation:" + std::to_string(cmd->peer_nested_root()->join_validation()) + "\n";
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_PeerRemove] = [&](const void* command) -> std::string{
        const iroha::PeerRemove* cmd = static_cast<const iroha::PeerRemove *>(command);

        std::string res = "PeerRemove[\n";
        res += "    peer:publicKey:" + cmd->peer_nested_root()->publicKey()->str() + ",\n";
        res += "    peer:ip:" + cmd->peer_nested_root()->ip()->str() + ",\n";
        res += "    peer:active:" + std::to_string(cmd->peer_nested_root()->active()) + ",\n";
        res += "    peer:join_network:" + std::to_string(cmd->peer_nested_root()->join_network()) + ",\n";
        res += "    peer:join_validation:" + std::to_string(cmd->peer_nested_root()->join_validation()) + "\n";
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_PeerSetActive] = [&](const void* command) -> std::string{
        const iroha::PeerSetActive* cmd = static_cast<const iroha::PeerSetActive *>(command);

        std::string res = "PeerSetActive[\n";
        res += "    peer:peerPubKey:" +  cmd->peerPubKey()->str() + ",\n";
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_PeerSetTrust] = [&](const void* command) -> std::string{
        const iroha::PeerSetTrust* cmd = static_cast<const iroha::PeerSetTrust *>(command);

        std::string res = "PeerSetTrust[\n";
        res += "    peerPubKey:" +  cmd->peerPubKey()->str() + ",\n";
        res += "    trust:" + std::to_string(cmd->trust()) + ",\n";
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_PeerChangeTrust] = [&](const void* command) -> std::string{
        const iroha::PeerChangeTrust* cmd = static_cast<const iroha::PeerChangeTrust *>(command);

        std::string res = "PeerSetTrust[\n";
        res += "    peerPubKey:" +  cmd->peerPubKey()->str() + ",\n";
        res += "    delta:" + std::to_string(cmd->delta()) + "\n";
        res += "]\n";
        return res;
    };

    command_to_strings[iroha::Command_AccountAdd] = [&](const void* command) -> std::string{
        const iroha::AccountAdd* cmd = static_cast<const iroha::AccountAdd *>(command);

        std::string res = "AccountAdd[\n";
        res += "    account:WIP\n"; //+ cmd->account_nested_root(). + ",\n";
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_AccountRemove] = [&](const void* command) -> std::string{
        const iroha::AccountRemove* cmd = static_cast<const iroha::AccountRemove *>(command);

        std::string res = "AccountRemove[\n";
        res += "    account:WIP\n"; //+ cmd->account_nested_root(). + ",\n";
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_AccountAddSignatory] = [&](const void* command) -> std::string{
        const iroha::AccountAddSignatory* cmd = static_cast<const iroha::AccountAddSignatory *>(command);

        std::string res = "AccountAddSignatory[\n";
        res += "    account:WIP\n"; //+ cmd->account_nested_root(). + ",\n";
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_AccountRemoveSignatory] = [&](const void* command) -> std::string{
        const iroha::AccountRemoveSignatory* cmd = static_cast<const iroha::AccountRemoveSignatory *>(command);

        std::string res = "AccountRemoveSignatory[\n";
        res += "    account:WIP\n"; //+ cmd->account_nested_root(). + ",\n";
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_AccountSetUseKeys] = [&](const void* command) -> std::string{
        const iroha::AccountSetUseKeys* cmd = static_cast<const iroha::AccountSetUseKeys *>(command);

        std::string res = "AccountSetUseKeys[\n";
        res += "    account:WIP\n"; //+ cmd->account_nested_root(). + ",\n";
        res += "]\n";
        return res;
    };

    command_to_strings[iroha::Command_ChaincodeAdd] = [&](const void* command) -> std::string{
        const iroha::ChaincodeAdd* cmd = static_cast<const iroha::ChaincodeAdd *>(command);

        std::string res = "ChaincodeAdd[\n";
        res += "]\n";
        return res;
    };
    command_to_strings[iroha::Command_ChaincodeRemove] = [&](const void* command) -> std::string{
        const iroha::ChaincodeRemove* cmd = static_cast<const iroha::ChaincodeRemove *>(command);

        std::string res = "ChaincodeRemove[\n";
        res += "]\n";
    };
    command_to_strings[iroha::Command_ChaincodeExecute] = [&](const void* command) -> std::string{
        const iroha::ChaincodeExecute* cmd = static_cast<const iroha::ChaincodeExecute *>(command);

        std::string res = "ChaincodeExecute[\n";
        res += "]\n";
    };

    res += command_to_strings[tx.command_type()](tx.command());
    return res;
}


/**
 * toConsensusEvent
 * - Encapsulate a transaction in a consensus event. Argument fromTx will be
 *   deeply copied and create new consensus event that has the copied transaction.
 */
std::unique_ptr<::iroha::ConsensusEvent> toConsensusEvent(
    const iroha::Transaction& fromTx) {

  flatbuffers::FlatBufferBuilder fbbConsensusEvent;

  // At first, peerSignatures is empty. (Is this right?)
  std::vector<flatbuffers::Offset<Signature>> peerSignatures;

  std::vector<flatbuffers::Offset<iroha::Signature>> signatures;

  // Tempolary implementation: Currently, #(tx) is one.
  const auto& aSignature = fromTx.signatures()->Get(0);

  std::vector<uint8_t> signatureBlob(
    aSignature->signature()->begin(),
    aSignature->signature()->end()
  );

  signatures.push_back(
    ::iroha::CreateSignatureDirect(
      fbbConsensusEvent,
      aSignature->publicKey()->c_str(),
      &signatureBlob,
      1234567
    )
  );

  std::vector<uint8_t> hashes(
    fromTx.hash()->begin(),
    fromTx.hash()->end()
  );

  std::vector<uint8_t> data(
    fromTx.attachment()->data()->begin(),
    fromTx.attachment()->data()->end()
  );

  auto attachmentOffset = ::iroha::CreateAttachmentDirect(
    fbbConsensusEvent,
    fromTx.attachment()->mime()->c_str(),
    &data
  );

  std::vector<flatbuffers::Offset<Transaction>> transactions;

  // TODO: Currently, #(transaction) is one.
  transactions.push_back(
    ::iroha::CreateTransactionDirect(
      fbbConsensusEvent,
      fromTx.creatorPubKey()->c_str(),
      fromTx.command_type(),
      flatbuffer_service::CreateCommandDirect(
        fbbConsensusEvent,
        fromTx.command(),
        fromTx.command_type()
      ),
      &signatures,
      &hashes,
      attachmentOffset
    )
  );

  auto consensusEventOffset = ::iroha::CreateConsensusEventDirect(
    fbbConsensusEvent,
    &peerSignatures,
    &transactions
  );

  fbbConsensusEvent.Finish(consensusEventOffset);

  auto flatbuf = fbbConsensusEvent.ReleaseBufferPointer();

  return std::unique_ptr<::iroha::ConsensusEvent>(
    flatbuffers::GetMutableRoot<::iroha::ConsensusEvent>(flatbuf.get())
  );
}

std::unique_ptr<::iroha::ConsensusEvent> addSignature(
    const std::unique_ptr<iroha::ConsensusEvent>& event) {}

} // namespace flatbuffer_service
