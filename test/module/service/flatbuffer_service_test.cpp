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
#include <gtest/gtest.h>
#include <main_generated.h>
#include <service/flatbuffer_service.h>
#include <membership_service/peer_service.hpp>
#include <utils/datetime.hpp>

#include <iostream>
#include <memory>
#include <unordered_map>

using flatbuffers::Offset;
using flatbuffers::FlatBufferBuilder;

/***************************************************************************************
 * Generator
 ***************************************************************************************/
/*
std::vector<uint8_t> genAccount() {
  return flatbuffer_service::account::CreateAccount(
    "pubkey", "alias", "prevPubKey", {"pubkey1", "pubkey2", "pubkey3"}, 1);
}

std::vector<uint8_t>

std::vector<uint8_t> genAccountPermunationNull(int perm) {
  // ToDo: After safe exception handling
}

std::vector<uint8_t> genCurrency() {
  return flatbuffer_service::asset::CreateCurrency(
    "IROHA", "Domain", "Ledger", "Desc", "31415", 4);
}

std::vector<uint8_t> genCurrencyPermunationNull(int perm) {
  // ToDo: After safe exception handling
}

Offset<::iroha::Command::AccountAdd> genAccountAddCommand(
  FlatBufferBuilder& fbb) {
  auto account = genAccount();
  return ::iroha::CreateAccountAddDirect(fbb, &account);
}

Offset<::iroha::Signature> genSignature(FlatBufferBuilder& fbb,
                                        const std::vector<uint8_t>& blob) {
  return ::iroha::CreateSignatureDirect(fbb, "SIG PUBKEY", &sigblob);
}

Offset<Vector<Offset<::iroha::Signature>>> genSignatures(
  FlatBufferBuilder& fbb) {
return {genSignature({'a', 'b'}), genSignature('0', '1')};
}

Offset<::iroha::Attachment> genAttachment(FlatBufferBuilder& fbb) {
  std::vector<uint8_t> data{'d', 'a', 't', 'a'};
  return ::iroha::CreateAttachmentDirect(fbb, "none", &data);
}

Offset<::iroha::Transaction> genTx(FlatBufferBuilder& fbb) {
  std::vector<uint8_t> hash{'h', 's'};
  return ::iroha::CreateTransactionWrapperDirect(
    fbb, "CREATOR", ::iroha::Command::AccountAdd,
    genAccountAddCommand(fbb), genSignatures(fbb),
    &hash, 99999, genAttachment(fbb)
}

std::pair<bool, std::string> issueTx(const std::string& ip) {
  auto channel =
    grpc::CreateChannel(ip + ":50051", grpc::InsecureChannelCredentials());
  auto stub = iroha::Sumeragi::NewStub(channel);
  grpc::ClientContext context;

  flatbuffers::FlatBufferBuilder fbb;
  auto txoffset = genTx(fbb);
  fbb.Finish(txoffset);

  auto txref = flatbuffers::BufferRef<iroha::Transaction>(
    fbb.GetBufferPointer(), fbb.GetSize());

  flatbuffers::BufferRef<iroha::Response> response;

  auto status = stub->Torii(&context, txref, &response);
  return {status.ok(), response.GetRoot()->message()->str()};
}
*/
/***************************************************************************************
 * Test
 ***************************************************************************************/
TEST(FlatbufferServiceTest, toString) {
  auto publicKey = "SamplePublicKey";
  // Build a request with the name set.
  flatbuffers::FlatBufferBuilder fbb;

  std::unique_ptr<std::vector<flatbuffers::Offset<flatbuffers::String>>>
      signatories(new std::vector<flatbuffers::Offset<flatbuffers::String>>());
  signatories->emplace_back(fbb.CreateString(publicKey));

  auto account_vec = flatbuffer_service::account::CreateAccount(
      publicKey, "alias", "prevPubKey", {"sig1", "sig2"}, 1);

  auto command = iroha::CreateAccountAddDirect(fbb, &account_vec);

  std::unique_ptr<std::vector<flatbuffers::Offset<iroha::Signature>>>
      signature_vec(new std::vector<flatbuffers::Offset<iroha::Signature>>());
  std::unique_ptr<std::vector<uint8_t>> signed_message(
      new std::vector<uint8_t>());
  signed_message->emplace_back('a');
  signed_message->emplace_back('b');
  signed_message->emplace_back('c');
  signed_message->emplace_back('d');

  signature_vec->emplace_back(iroha::CreateSignatureDirect(
      fbb, publicKey, signed_message.get(), 1234567));

  auto tx_offset = iroha::CreateTransactionDirect(
      fbb, publicKey, iroha::Command::AccountAdd, command.Union(),
      signature_vec.get(), nullptr, 0);
  fbb.Finish(tx_offset);
  auto tx = flatbuffers::BufferRef<iroha::Transaction>(fbb.GetBufferPointer(),
                                                       fbb.GetSize());

  std::cout << flatbuffer_service::toString(*tx.GetRoot()) << std::endl;
}


/*********************************************************
 * Add
 *********************************************************/
TEST(FlatbufferServiceTest, toConsensusEvent_Add) {
  flatbuffers::FlatBufferBuilder fbb;

  const auto currencyBuf = flatbuffer_service::asset::CreateCurrency(
    "IROHA", "Domain", "Ledger", "Desc", "31415", 4);

  const auto signatureOffsets = [&] {
    std::vector<uint8_t> sigblob1 = {'a', 'b'};
    std::vector<uint8_t> sigblob2 = {'\0', 'a', '\0', 'b'};
    return std::vector<flatbuffers::Offset<::iroha::Signature>>{
      ::iroha::CreateSignatureDirect(fbb, "TxPubKey1", &sigblob1, 100000),
      ::iroha::CreateSignatureDirect(fbb, "TxPubKey2", &sigblob2, 100001)};
  }();

  const auto _hash = std::vector<uint8_t>{'h', '\0', '?', '\0'};

  const auto stamp = datetime::unixtime();

  const auto attachmentOffset = [&] {
    auto data = std::vector<uint8_t>{'d', '\0', '!'};
    return ::iroha::CreateAttachmentDirect(
      fbb, "=?ISO-2022-JP?B?VG95YW1hX05hbw==?=", &data);
  }();

  const auto txOffset = ::iroha::CreateTransactionDirect(
    fbb, "Creator PubKey", iroha::Command::Add,
    ::iroha::CreateAddDirect(fbb, "AccPubKey", &currencyBuf).Union(),
    &signatureOffsets, &_hash, stamp, attachmentOffset);

  fbb.Finish(txOffset);

  const auto ptr = fbb.ReleaseBufferPointer();
  const auto txptr = flatbuffers::GetRoot<::iroha::Transaction>(ptr.get());

  auto consensusEvent = flatbuffer_service::toConsensusEvent(*txptr);
  ASSERT_TRUE(consensusEvent);

  flatbuffers::unique_ptr_t uptr;
  consensusEvent.move_value(uptr);

  const auto root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(uptr.get());

  // validate peerSignatures()
  ASSERT_TRUE(root->peerSignatures()->size() == 0);
  ASSERT_EQ(root->code(), ::iroha::Code::UNDECIDED);

  // validate transactions()
  const auto txptrFromEvent =
    root->transactions()
      ->Get(0)
      ->tx_nested_root();  // ToDo: toConsensusEvent() receives 1 tx.

  ASSERT_STREQ(txptrFromEvent->creatorPubKey()->c_str(), "Creator PubKey");
  ASSERT_EQ(txptrFromEvent->command_type(), ::iroha::Command::Add);

  // validate nested account
  const auto croot =
    txptrFromEvent->command_as_Add()->asset_nested_root()->asset_as_Currency();

  ASSERT_STREQ(croot->currency_name()->c_str(), "IROHA");
  ASSERT_STREQ(croot->domain_name()->c_str(), "Domain");
  ASSERT_STREQ(croot->ledger_name()->c_str(), "Ledger");
  ASSERT_STREQ(croot->description()->c_str(), "Desc");
  ASSERT_STREQ(croot->amount()->c_str(), "31415");
  ASSERT_EQ(croot->precision(), 4);
}

/*********************************************************
 * Account Add
 *********************************************************/
TEST(FlatbufferServiceTest, toConsensusEvent_AccountAdd) {
  flatbuffers::FlatBufferBuilder fbb;

  const auto accountBuf = flatbuffer_service::account::CreateAccount(
      "PublicKey", "Alias\u30e6", "PrevPubKey", {"sig1", "sig2", "sig3"}, 1);

  const auto signatureOffsets = [&] {
    std::vector<uint8_t> sigblob1 = {'a', 'b'};
    std::vector<uint8_t> sigblob2 = {'\0', 'a', '\0', 'b'};
    return std::vector<flatbuffers::Offset<::iroha::Signature>>{
        ::iroha::CreateSignatureDirect(fbb, "TxPubKey1", &sigblob1, 100000),
        ::iroha::CreateSignatureDirect(fbb, "TxPubKey2", &sigblob2, 100001)};
  }();

  const auto _hash = std::vector<uint8_t>{'h', '\0', '?', '\0'};

  const auto stamp = datetime::unixtime();

  const auto attachmentOffset = [&] {
    auto data = std::vector<uint8_t>{'d', '\0', '!'};
    return ::iroha::CreateAttachmentDirect(
        fbb, "=?ISO-2022-JP?B?VG95YW1hX05hbw==?=", &data);
  }();

  const auto txOffset = ::iroha::CreateTransactionDirect(
      fbb, "Creator PubKey", iroha::Command::AccountAdd,
      ::iroha::CreateAccountAddDirect(fbb, &accountBuf).Union(),
      &signatureOffsets, &_hash, stamp, attachmentOffset);

  fbb.Finish(txOffset);

  const auto ptr = fbb.ReleaseBufferPointer();
  const auto txptr = flatbuffers::GetRoot<::iroha::Transaction>(ptr.get());

  auto consensusEvent = flatbuffer_service::toConsensusEvent(*txptr);
  ASSERT_TRUE(consensusEvent);

  flatbuffers::unique_ptr_t uptr;
  consensusEvent.move_value(uptr);

  const auto root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(uptr.get());

  // validate peerSignatures()
  ASSERT_TRUE(root->peerSignatures()->size() == 0);
  ASSERT_EQ(root->code(), ::iroha::Code::UNDECIDED);

  // validate transactions()
  const auto txptrFromEvent =
      root->transactions()
          ->Get(0)
          ->tx_nested_root();  // ToDo: toConsensusEvent() receives 1 tx.

  ASSERT_STREQ(txptrFromEvent->creatorPubKey()->c_str(), "Creator PubKey");
  ASSERT_EQ(txptrFromEvent->command_type(), ::iroha::Command::AccountAdd);

  // validate nested account
  const auto accroot =
      txptrFromEvent->command_as_AccountAdd()->account_nested_root();
  ASSERT_STREQ(accroot->pubKey()->c_str(), "PublicKey");
  ASSERT_STREQ(accroot->alias()->c_str(), "Alias\u30e6");
  ASSERT_STREQ(accroot->signatories()->Get(0)->c_str(), "sig1");
  ASSERT_STREQ(accroot->signatories()->Get(1)->c_str(), "sig2");
  ASSERT_STREQ(accroot->signatories()->Get(2)->c_str(), "sig3");
  ASSERT_EQ(accroot->useKeys(), 1);

  // validate signatures
  ASSERT_STREQ(txptrFromEvent->signatures()->Get(0)->publicKey()->c_str(),
               "TxPubKey1");
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->signature()->Get(0), 'a');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->signature()->Get(1), 'b');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->signature()->size(), 2);
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->timestamp(), 100000);

  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(0), '\0');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(1), 'a');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(2), '\0');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(3), 'b');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->size(), 4);
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->timestamp(), 100001);

  ASSERT_EQ(txptrFromEvent->hash()->Get(0), 'h');
  ASSERT_EQ(txptrFromEvent->hash()->Get(1), '\0');
  ASSERT_EQ(txptrFromEvent->hash()->Get(2), '?');
  ASSERT_EQ(txptrFromEvent->hash()->Get(3), '\0');
  ASSERT_EQ(txptrFromEvent->hash()->size(), 4);

  ASSERT_STREQ(txptrFromEvent->attachment()->mime()->c_str(),
               "=?ISO-2022-JP?B?VG95YW1hX05hbw==?=");

  // validate attachment
  ASSERT_EQ(txptrFromEvent->attachment()->data()->Get(0), 'd');
  ASSERT_EQ(txptrFromEvent->attachment()->data()->Get(1), '\0');
  ASSERT_EQ(txptrFromEvent->attachment()->data()->Get(2), '!');
  ASSERT_EQ(txptrFromEvent->attachment()->data()->size(), 3);
}

/*********************************************************
 * Asset Create
 *********************************************************/
TEST(FlatbufferServiceTest, toConsensusEvent_AssetCreate) {
  flatbuffers::FlatBufferBuilder fbb;
  const auto cmd = ::iroha::CreateAssetCreateDirect(fbb, "Asset", "Domain", "Ledger").Union();

  const auto signatureOffsets = [&] {
    std::vector<uint8_t> sigblob1 = {'a', 'b'};
    std::vector<uint8_t> sigblob2 = {'\0', 'a', '\0', 'b'};
    return std::vector<flatbuffers::Offset<::iroha::Signature>>{
      ::iroha::CreateSignatureDirect(fbb, "TxPubKey1", &sigblob1, 100000),
      ::iroha::CreateSignatureDirect(fbb, "TxPubKey2", &sigblob2, 100001)};
  }();

  const auto _hash = std::vector<uint8_t>{'h', '\0', '?', '\0'};

  const auto stamp = datetime::unixtime();

  const auto attachmentOffset = [&] {
    auto data = std::vector<uint8_t>{'d', '\0', '!'};
    return ::iroha::CreateAttachmentDirect(
      fbb, "=?ISO-2022-JP?B?VG95YW1hX05hbw==?=", &data);
  }();

  const auto txOffset = ::iroha::CreateTransactionDirect(
    fbb, "Creator PubKey", iroha::Command::AssetCreate, cmd,
    &signatureOffsets, &_hash, stamp, attachmentOffset);

  fbb.Finish(txOffset);

  const auto ptr = fbb.ReleaseBufferPointer();
  const auto txptr = flatbuffers::GetRoot<::iroha::Transaction>(ptr.get());

  auto consensusEvent = flatbuffer_service::toConsensusEvent(*txptr);
  ASSERT_TRUE(consensusEvent);

  flatbuffers::unique_ptr_t uptr;
  consensusEvent.move_value(uptr);

  const auto root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(uptr.get());

  // validate peerSignatures()
  ASSERT_TRUE(root->peerSignatures()->size() == 0);
  ASSERT_EQ(root->code(), ::iroha::Code::UNDECIDED);

  // validate transactions()
  const auto revTxPtr =
    root->transactions()
      ->Get(0)
      ->tx_nested_root();  // ToDo: toConsensusEvent() receives 1 tx.

  ASSERT_STREQ(revTxPtr->creatorPubKey()->c_str(), "Creator PubKey");
  ASSERT_EQ(revTxPtr->command_type(), iroha::Command::AssetCreate);

  auto revcmd = revTxPtr->command_as_AssetCreate();
  ASSERT_STREQ(revcmd->asset_name()->c_str(), "Asset");
  ASSERT_STREQ(revcmd->domain_name()->c_str(), "Domain");
  ASSERT_STREQ(revcmd->ledger_name()->c_str(), "Ledger");
}


/*********************************************************
 * Peer Add
 *********************************************************/
TEST(FlatbufferServiceTest, toConsensusEvent_PeerAdd) {
  flatbuffers::FlatBufferBuilder fbb;

  ::peer::Node np("IP", "PUBKEY", "LEDGER", 123.45, true, false);

  const auto peer = flatbuffer_service::primitives::CreatePeer(np);

  const auto signatureOffsets = [&] {
    std::vector<uint8_t> sigblob1 = {'a', 'b'};
    std::vector<uint8_t> sigblob2 = {'\0', 'a', '\0', 'b'};
    return std::vector<flatbuffers::Offset<::iroha::Signature>>{
      ::iroha::CreateSignatureDirect(fbb, "TxPubKey1", &sigblob1, 100000),
      ::iroha::CreateSignatureDirect(fbb, "TxPubKey2", &sigblob2, 100001)};
  }();

  const auto _hash = std::vector<uint8_t>{'h', '\0', '?', '\0'};

  const auto stamp = datetime::unixtime();

  const auto attachmentOffset = [&] {
    auto data = std::vector<uint8_t>{'d', '\0', '!'};
    return ::iroha::CreateAttachmentDirect(
      fbb, "=?ISO-2022-JP?B?VG95YW1hX05hbw==?=", &data);
  }();

  const auto txOffset = ::iroha::CreateTransactionDirect(
    fbb, "Creator PubKey", iroha::Command::PeerAdd,
    ::iroha::CreatePeerAddDirect(fbb, &peer).Union(),
    &signatureOffsets, &_hash, stamp, attachmentOffset);

  fbb.Finish(txOffset);

  const auto ptr = fbb.ReleaseBufferPointer();
  const auto txptr = flatbuffers::GetRoot<::iroha::Transaction>(ptr.get());

  auto consensusEvent = flatbuffer_service::toConsensusEvent(*txptr);
  ASSERT_TRUE(consensusEvent);

  flatbuffers::unique_ptr_t uptr;
  consensusEvent.move_value(uptr);

  const auto root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(uptr.get());

  // validate peerSignatures()
  ASSERT_TRUE(root->peerSignatures()->size() == 0);
  ASSERT_EQ(root->code(), ::iroha::Code::UNDECIDED);

  // validate transactions()
  const auto txptrFromEvent =
    root->transactions()
      ->Get(0)
      ->tx_nested_root();  // ToDo: toConsensusEvent() receives 1 tx.

  ASSERT_STREQ(txptrFromEvent->creatorPubKey()->c_str(), "Creator PubKey");
  ASSERT_EQ(txptrFromEvent->command_type(), ::iroha::Command::PeerAdd);

  // validate nested account
  const auto peerroot =
    txptrFromEvent->command_as_PeerAdd()->peer_nested_root();
  ASSERT_STREQ(peerroot->ip()->c_str(), "IP");
  ASSERT_STREQ(peerroot->publicKey()->c_str(), "PUBKEY");
  ASSERT_STREQ(peerroot->ledger_name()->c_str(), "LEDGER");
  ASSERT_EQ(peerroot->trust(), 123.45);
  ASSERT_EQ(peerroot->active(), true);
  ASSERT_EQ(peerroot->join_ledger(), false);
}


/*********************************************************
 * Peer Remove
 *********************************************************/
TEST(FlatbufferServiceTest, toConsensusEvent_PeerRemove) {
  flatbuffers::FlatBufferBuilder fbb;

  ::peer::Node np("IP", "PUBKEY", "LEDGER", 123.45, true, false);

  const auto peer = flatbuffer_service::primitives::CreatePeer(np);

  const auto signatureOffsets = [&] {
    std::vector<uint8_t> sigblob1 = {'a', 'b'};
    std::vector<uint8_t> sigblob2 = {'\0', 'a', '\0', 'b'};
    return std::vector<flatbuffers::Offset<::iroha::Signature>>{
      ::iroha::CreateSignatureDirect(fbb, "TxPubKey1", &sigblob1, 100000),
      ::iroha::CreateSignatureDirect(fbb, "TxPubKey2", &sigblob2, 100001)};
  }();

  const auto _hash = std::vector<uint8_t>{'h', '\0', '?', '\0'};

  const auto stamp = datetime::unixtime();

  const auto attachmentOffset = [&] {
    auto data = std::vector<uint8_t>{'d', '\0', '!'};
    return ::iroha::CreateAttachmentDirect(
      fbb, "=?ISO-2022-JP?B?VG95YW1hX05hbw==?=", &data);
  }();

  const auto txOffset = ::iroha::CreateTransactionDirect(
    fbb, "Creator PubKey", iroha::Command::PeerAdd,
    ::iroha::CreatePeerAddDirect(fbb, &peer).Union(),
    &signatureOffsets, &_hash, stamp, attachmentOffset);

  fbb.Finish(txOffset);

  const auto ptr = fbb.ReleaseBufferPointer();
  const auto txptr = flatbuffers::GetRoot<::iroha::Transaction>(ptr.get());

  auto consensusEvent = flatbuffer_service::toConsensusEvent(*txptr);
  ASSERT_TRUE(consensusEvent);

  flatbuffers::unique_ptr_t uptr;
  consensusEvent.move_value(uptr);

  const auto root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(uptr.get());

  // validate peerSignatures()
  ASSERT_TRUE(root->peerSignatures()->size() == 0);
  ASSERT_EQ(root->code(), ::iroha::Code::UNDECIDED);

  // validate transactions()
  const auto txptrFromEvent =
    root->transactions()
      ->Get(0)
      ->tx_nested_root();  // ToDo: toConsensusEvent() receives 1 tx.

  ASSERT_STREQ(txptrFromEvent->creatorPubKey()->c_str(), "Creator PubKey");
  ASSERT_EQ(txptrFromEvent->command_type(), ::iroha::Command::PeerAdd);

  // validate nested account
  const auto peerroot =
    txptrFromEvent->command_as_PeerAdd()->peer_nested_root();
  ASSERT_STREQ(peerroot->ip()->c_str(), "IP");
  ASSERT_STREQ(peerroot->publicKey()->c_str(), "PUBKEY");
  ASSERT_STREQ(peerroot->ledger_name()->c_str(), "LEDGER");
  ASSERT_EQ(peerroot->trust(), 123.45);
  ASSERT_EQ(peerroot->active(), true);
  ASSERT_EQ(peerroot->join_ledger(), false);
}




TEST(FlatbufferServiceTest, addSignature_AccountAdd) {
  flatbuffers::FlatBufferBuilder fbb;

  const auto accountBuf = flatbuffer_service::account::CreateAccount(
      "PublicKey", "Alias\u30e6", "PrevPubKey", {"sig1", "sig2", "sig3"}, 1);

  const auto signatureOffsets = [&] {
    std::vector<uint8_t> sigblob1 = {'a', 'b'};
    std::vector<uint8_t> sigblob2 = {'\0', 'a', '\0', 'b'};
    return std::vector<flatbuffers::Offset<::iroha::Signature>>{
        ::iroha::CreateSignatureDirect(fbb, "TxPubKey1", &sigblob1, 100000),
        ::iroha::CreateSignatureDirect(fbb, "TxPubKey2", &sigblob2, 100001)};
  }();

  const auto _hash = std::vector<uint8_t>{'h', '\0', '?', '\0'};

  const auto stamp = datetime::unixtime();

  const auto attachmentOffset = [&] {
    auto data = std::vector<uint8_t>{'d', '\0', '!'};
    return ::iroha::CreateAttachmentDirect(
        fbb, "=?ISO-2022-JP?B?VG95YW1hX05hbw==?=", &data);
  }();

  const auto txOffset = ::iroha::CreateTransactionDirect(
      fbb, "Creator PubKey", iroha::Command::AccountAdd,
      ::iroha::CreateAccountAddDirect(fbb, &accountBuf).Union(),
      &signatureOffsets, &_hash, stamp, attachmentOffset);

  fbb.Finish(txOffset);

  const auto ptr = fbb.ReleaseBufferPointer();
  const auto txptr = flatbuffers::GetRoot<::iroha::Transaction>(ptr.get());

  auto consensusEvent = flatbuffer_service::toConsensusEvent(*txptr);
  ASSERT_TRUE(consensusEvent);

  flatbuffers::unique_ptr_t uptr;
  consensusEvent.move_value(uptr);

  auto root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(uptr.get());

  const auto timeSig1Begin = datetime::unixtime();
  auto eventAddedSig1 = flatbuffer_service::addSignature(
      *root, "NEW PEER PUBLICKEY 1", "NEW PEER SIGNATURE 1");
  const auto timeSig1End = datetime::unixtime();
  root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(
      eventAddedSig1.value().get());
  ASSERT_LE(timeSig1Begin, root->peerSignatures()->Get(0)->timestamp());
  ASSERT_LE(root->peerSignatures()->Get(0)->timestamp(), timeSig1End);
  ASSERT_EQ(root->code(), ::iroha::Code::UNDECIDED);

  const auto timeSig2Begin = datetime::unixtime();
  auto eventAddedSig2 = flatbuffer_service::addSignature(
      *root, "`1234567890-=", "NEW PEER SIGNATURE\'\n\t2");
  const auto timeSig2End = datetime::unixtime();
  root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(
      eventAddedSig2.value().get());
  ASSERT_LE(timeSig2Begin, root->peerSignatures()->Get(1)->timestamp());
  ASSERT_LE(root->peerSignatures()->Get(1)->timestamp(), timeSig2End);
  ASSERT_EQ(root->code(), ::iroha::Code::UNDECIDED);

  const auto timeSig3Begin = datetime::unixtime();
  auto eventAddedSig3 = flatbuffer_service::addSignature(
      *root, "NEW\\ PEER PUBLICKEY\'\n\t3\u30e6\u30e6",
      "NEW PEER SIGNATURE 3\u30e6\u30e6");
  const auto timeSig3End = datetime::unixtime();
  root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(
      eventAddedSig3.value().get());
  ASSERT_LE(timeSig3Begin, root->peerSignatures()->Get(2)->timestamp());
  ASSERT_LE(root->peerSignatures()->Get(2)->timestamp(), timeSig3End);
  ASSERT_EQ(root->code(), ::iroha::Code::UNDECIDED);

  // validate peerSignatures()
  ASSERT_TRUE(root->peerSignatures()->size() == 3);

  ASSERT_STREQ(root->peerSignatures()->Get(0)->publicKey()->c_str(),
               "NEW PEER PUBLICKEY 1");
  ASSERT_STREQ(root->peerSignatures()->Get(1)->publicKey()->c_str(),
               "`1234567890-=");
  ASSERT_STREQ(root->peerSignatures()->Get(2)->publicKey()->c_str(),
               "NEW\\ PEER PUBLICKEY\'\n\t3\u30e6\u30e6");

  // ToDo: GetAsString(uoffset_t) is better, but I don't know how to do.
  auto sigs = root->peerSignatures();
  std::string sig1(sigs->Get(0)->signature()->begin(),
                   sigs->Get(0)->signature()->end());
  std::string sig2(sigs->Get(1)->signature()->begin(),
                   sigs->Get(1)->signature()->end());
  std::string sig3(sigs->Get(2)->signature()->begin(),
                   sigs->Get(2)->signature()->end());
  ASSERT_STREQ(sig1.c_str(), "NEW PEER SIGNATURE 1");
  ASSERT_STREQ(sig2.c_str(), "NEW PEER SIGNATURE\'\n\t2");
  ASSERT_STREQ(sig3.c_str(), "NEW PEER SIGNATURE 3\u30e6\u30e6");

  // validate transactions()
  const auto txptrFromEvent =
      root->transactions()
          ->Get(0)
          ->tx_nested_root();  // ToDo: toConsensusEvent() receives 1 tx.

  ASSERT_STREQ(txptrFromEvent->creatorPubKey()->c_str(), "Creator PubKey");
  ASSERT_EQ(txptrFromEvent->command_type(), ::iroha::Command::AccountAdd);

  // validate nested account
  const auto accroot =
      txptrFromEvent->command_as_AccountAdd()->account_nested_root();
  ASSERT_STREQ(accroot->pubKey()->c_str(), "PublicKey");
  ASSERT_STREQ(accroot->alias()->c_str(), "Alias\u30e6");
  ASSERT_STREQ(accroot->signatories()->Get(0)->c_str(), "sig1");
  ASSERT_STREQ(accroot->signatories()->Get(1)->c_str(), "sig2");
  ASSERT_STREQ(accroot->signatories()->Get(2)->c_str(), "sig3");
  ASSERT_EQ(accroot->useKeys(), 1);

  // validate signatures
  ASSERT_STREQ(txptrFromEvent->signatures()->Get(0)->publicKey()->c_str(),
               "TxPubKey1");
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->signature()->Get(0), 'a');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->signature()->Get(1), 'b');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->signature()->size(), 2);
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->timestamp(), 100000);

  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(0), '\0');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(1), 'a');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(2), '\0');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(3), 'b');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->size(), 4);
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->timestamp(), 100001);

  ASSERT_EQ(txptrFromEvent->hash()->Get(0), 'h');
  ASSERT_EQ(txptrFromEvent->hash()->Get(1), '\0');
  ASSERT_EQ(txptrFromEvent->hash()->Get(2), '?');
  ASSERT_EQ(txptrFromEvent->hash()->Get(3), '\0');
  ASSERT_EQ(txptrFromEvent->hash()->size(), 4);

  ASSERT_STREQ(txptrFromEvent->attachment()->mime()->c_str(),
               "=?ISO-2022-JP?B?VG95YW1hX05hbw==?=");

  // validate attachment
  ASSERT_EQ(txptrFromEvent->attachment()->data()->Get(0), 'd');
  ASSERT_EQ(txptrFromEvent->attachment()->data()->Get(1), '\0');
  ASSERT_EQ(txptrFromEvent->attachment()->data()->Get(2), '!');
  ASSERT_EQ(txptrFromEvent->attachment()->data()->size(), 3);
}

TEST(FlatbufferServiceTest, makeCommit_AccountAdd) {
  flatbuffers::FlatBufferBuilder fbb;

  const auto accountBuf = flatbuffer_service::account::CreateAccount(
      "PublicKey", "Alias\u30e6", "PrevPubKey", {"sig1", "sig2", "sig3"}, 1);

  const auto signatureOffsets = [&] {
    std::vector<uint8_t> sigblob1 = {'a', 'b'};
    std::vector<uint8_t> sigblob2 = {'\0', 'a', '\0', 'b'};
    return std::vector<flatbuffers::Offset<::iroha::Signature>>{
        ::iroha::CreateSignatureDirect(fbb, "TxPubKey1", &sigblob1, 100000),
        ::iroha::CreateSignatureDirect(fbb, "TxPubKey2", &sigblob2, 100001)};
  }();

  const auto _hash = std::vector<uint8_t>{'h', '\0', '?', '\0'};

  const auto stamp = datetime::unixtime();

  const auto attachmentOffset = [&] {
    auto data = std::vector<uint8_t>{'d', '\0', '!'};
    return ::iroha::CreateAttachmentDirect(
        fbb, "=?ISO-2022-JP?B?VG95YW1hX05hbw==?=", &data);
  }();

  const auto txOffset = ::iroha::CreateTransactionDirect(
      fbb, "Creator PubKey", iroha::Command::AccountAdd,
      ::iroha::CreateAccountAddDirect(fbb, &accountBuf).Union(),
      &signatureOffsets, &_hash, stamp, attachmentOffset);

  fbb.Finish(txOffset);

  const auto ptr = fbb.ReleaseBufferPointer();
  const auto txptr = flatbuffers::GetRoot<::iroha::Transaction>(ptr.get());

  auto consensusEvent = flatbuffer_service::toConsensusEvent(*txptr);
  ASSERT_TRUE(consensusEvent);

  flatbuffers::unique_ptr_t uptr;
  consensusEvent.move_value(uptr);

  auto root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(uptr.get());

  auto addedSigEvent = flatbuffer_service::addSignature(
      *root, "NEW PEER PUBLICKEY 1", "NEW PEER SIGNATURE 1");
  root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(
      addedSigEvent.value().get());

  auto committedEvent = flatbuffer_service::makeCommit(*root);
  ASSERT_TRUE(committedEvent);

  committedEvent.move_value(uptr);

  root = flatbuffers::GetRoot<::iroha::ConsensusEvent>(uptr.get());

  // validate peerSignatures()
  ASSERT_TRUE(root->peerSignatures()->size() == 1);
  ASSERT_EQ(root->code(), ::iroha::Code::COMMIT);

  // validate transactions()
  const auto txptrFromEvent =
      root->transactions()
          ->Get(0)
          ->tx_nested_root();  // ToDo: toConsensusEvent() receives 1 tx.

  ASSERT_STREQ(txptrFromEvent->creatorPubKey()->c_str(), "Creator PubKey");
  ASSERT_EQ(txptrFromEvent->command_type(), ::iroha::Command::AccountAdd);

  // validate nested account
  const auto accroot =
      txptrFromEvent->command_as_AccountAdd()->account_nested_root();
  ASSERT_STREQ(accroot->pubKey()->c_str(), "PublicKey");
  ASSERT_STREQ(accroot->alias()->c_str(), "Alias\u30e6");
  ASSERT_STREQ(accroot->signatories()->Get(0)->c_str(), "sig1");
  ASSERT_STREQ(accroot->signatories()->Get(1)->c_str(), "sig2");
  ASSERT_STREQ(accroot->signatories()->Get(2)->c_str(), "sig3");
  ASSERT_EQ(accroot->useKeys(), 1);

  // validate signatures
  ASSERT_STREQ(txptrFromEvent->signatures()->Get(0)->publicKey()->c_str(),
               "TxPubKey1");
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->signature()->Get(0), 'a');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->signature()->Get(1), 'b');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->signature()->size(), 2);
  ASSERT_EQ(txptrFromEvent->signatures()->Get(0)->timestamp(), 100000);

  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(0), '\0');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(1), 'a');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(2), '\0');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->Get(3), 'b');
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->signature()->size(), 4);
  ASSERT_EQ(txptrFromEvent->signatures()->Get(1)->timestamp(), 100001);

  ASSERT_EQ(txptrFromEvent->hash()->Get(0), 'h');
  ASSERT_EQ(txptrFromEvent->hash()->Get(1), '\0');
  ASSERT_EQ(txptrFromEvent->hash()->Get(2), '?');
  ASSERT_EQ(txptrFromEvent->hash()->Get(3), '\0');
  ASSERT_EQ(txptrFromEvent->hash()->size(), 4);

  ASSERT_STREQ(txptrFromEvent->attachment()->mime()->c_str(),
               "=?ISO-2022-JP?B?VG95YW1hX05hbw==?=");

  // validate attachment
  ASSERT_EQ(txptrFromEvent->attachment()->data()->Get(0), 'd');
  ASSERT_EQ(txptrFromEvent->attachment()->data()->Get(1), '\0');
  ASSERT_EQ(txptrFromEvent->attachment()->data()->Get(2), '!');
  ASSERT_EQ(txptrFromEvent->attachment()->data()->size(), 3);
}

TEST(FlatbufferServiceTest, copyConsensusEvent) {
  flatbuffers::FlatBufferBuilder xbb;
  std::vector<flatbuffers::Offset<::iroha::Signature>> peerSignatures;
  std::vector<uint8_t> signature = {'a', 'b', 'c'};
  peerSignatures.push_back(
      ::iroha::CreateSignatureDirect(xbb, "PUBKEY1", &signature, 100000));
  peerSignatures.push_back(
      ::iroha::CreateSignatureDirect(xbb, "PUBKEY2", &signature, 200000));

  std::vector<std::string> signatories{"123", "-=[p"};
  const auto account = flatbuffer_service::account::CreateAccount(
      "accpubkey", "alias", "PrevPubKey", signatories, 1);
  const auto command = ::iroha::CreateAccountAddDirect(xbb, &account);
  std::vector<flatbuffers::Offset<::iroha::Signature>> tx_signatures;
  std::vector<uint8_t> txsig = {'a', 'b', 'c'};
  tx_signatures.push_back(
      ::iroha::CreateSignatureDirect(xbb, "txsig pubkey 1", &txsig, 99999));
  std::vector<uint8_t> hash = {'h', 's'};
  std::vector<uint8_t> data = {'d', 't'};
  const auto stamp = datetime::unixtime();
  const auto attachment = ::iroha::CreateAttachmentDirect(xbb, "mime", &data);
  const auto txoffset = ::iroha::CreateTransactionDirect(
      xbb, "creator", ::iroha::Command::AccountAdd, command.Union(),
      &tx_signatures, &hash, stamp, attachment);
  xbb.Finish(txoffset);
  std::vector<uint8_t> txbuf(xbb.GetBufferPointer(),
                             xbb.GetBufferPointer() + xbb.GetSize());

  flatbuffers::FlatBufferBuilder ebb;
  const auto txw = ::iroha::CreateTransactionWrapperDirect(ebb, &txbuf);
  std::vector<flatbuffers::Offset<::iroha::TransactionWrapper>> txwrappers;
  txwrappers.push_back(txw);
  const auto eventofs = ::iroha::CreateConsensusEventDirect(
      ebb, &peerSignatures, &txwrappers, ::iroha::Code::UNDECIDED);
  ebb.Finish(eventofs);

  const auto eflatbuf = ebb.ReleaseBufferPointer();
  const auto eventptr = flatbuffers::GetRoot<::iroha::ConsensusEvent>(eflatbuf.get());

  flatbuffers::FlatBufferBuilder copyebb;
  const auto copyeventofs =
      flatbuffer_service::copyConsensusEvent(copyebb, *eventptr);
  ASSERT_TRUE(copyeventofs);
  copyebb.Finish(copyeventofs.value());

  const auto copyeventbuf = copyebb.ReleaseBufferPointer();
  const auto copyeventptr =
      flatbuffers::GetRoot<::iroha::ConsensusEvent>(copyeventbuf.get());

  ASSERT_EQ(copyeventptr->code(), ::iroha::Code::UNDECIDED);
  const auto revPeerSigs = copyeventptr->peerSignatures();
  ASSERT_STREQ(revPeerSigs->Get(0)->publicKey()->c_str(), "PUBKEY1");
  ASSERT_STREQ(revPeerSigs->Get(1)->publicKey()->c_str(), "PUBKEY2");
  std::vector<uint8_t> sig1(revPeerSigs->Get(0)->signature()->begin(),
                            revPeerSigs->Get(0)->signature()->end());
  std::vector<uint8_t> sig2(revPeerSigs->Get(1)->signature()->begin(),
                            revPeerSigs->Get(1)->signature()->end());
  std::vector<uint8_t> abc = {'a', 'b', 'c'};
  ASSERT_EQ(sig1, abc);
  ASSERT_EQ(sig2, abc);
  ASSERT_EQ(revPeerSigs->Get(0)->timestamp(), 100000);
  ASSERT_EQ(revPeerSigs->Get(1)->timestamp(), 200000);

  const auto txnested = copyeventptr->transactions()->Get(0)->tx_nested_root();
  ASSERT_STREQ(txnested->creatorPubKey()->c_str(), "creator");
  ASSERT_EQ(txnested->command_type(), ::iroha::Command::AccountAdd);

  const auto txnestedsig = txnested->signatures()->Get(0);
  ASSERT_STREQ(txnestedsig->publicKey()->c_str(), "txsig pubkey 1");
  ASSERT_EQ(txnestedsig->signature()->size(), 3);
  ASSERT_EQ(txnestedsig->signature()->Get(0), 'a');
  ASSERT_EQ(txnestedsig->signature()->Get(1), 'b');
  ASSERT_EQ(txnestedsig->signature()->Get(2), 'c');
  ASSERT_EQ(txnestedsig->timestamp(), 99999);
  ASSERT_EQ(txnested->hash()->Get(0), 'h');
  ASSERT_EQ(txnested->hash()->Get(1), 's');
  ASSERT_STREQ(txnested->attachment()->mime()->c_str(), "mime");
  ASSERT_EQ(txnested->attachment()->data()->Get(0), 'd');
  ASSERT_EQ(txnested->attachment()->data()->Get(1), 't');

  const auto accnested = txnested->command_as_AccountAdd()->account_nested_root();
  ASSERT_STREQ(accnested->pubKey()->c_str(), "accpubkey");
  ASSERT_STREQ(accnested->alias()->c_str(), "alias");
  ASSERT_STREQ(accnested->signatories()->Get(0)->c_str(), "123");
  ASSERT_STREQ(accnested->signatories()->Get(1)->c_str(), "-=[p");
  ASSERT_EQ(accnested->useKeys(), 1);
}
