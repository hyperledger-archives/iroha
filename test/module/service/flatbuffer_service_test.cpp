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
#include <memory>
#include <unordered_map>
#include <iostream>
#include <generated/main_generated.h>
#include <service/flatbuffer_service.h>


TEST(FlatbufferServiceTest, toString) {

    auto publicKey = "SamplePublicKey";
    // Build a request with the name set.
    flatbuffers::FlatBufferBuilder fbb;

    std::unique_ptr<std::vector<flatbuffers::Offset<flatbuffers::String>>> signatories(
            new std::vector<flatbuffers::Offset<flatbuffers::String>>()
    );
    signatories->emplace_back(fbb.CreateString(publicKey));

    auto account_vec = [&] {
        flatbuffers::FlatBufferBuilder fbbAccount;

        std::unique_ptr<std::vector<flatbuffers::Offset<flatbuffers::String>>> signatories(
                new std::vector<flatbuffers::Offset<flatbuffers::String>>( {fbbAccount.CreateString("publicKey1")})
        );

        auto account = iroha::CreateAccountDirect(fbbAccount, publicKey, "alias", signatories.get(), 1);
        fbbAccount.Finish(account);

        std::unique_ptr<std::vector<uint8_t>> account_vec(
                new std::vector<uint8_t>()
        );

        auto buf = fbbAccount.GetBufferPointer();

        account_vec->assign(
                buf, buf + fbbAccount.GetSize()
        );

        return account_vec;
    }();

    auto command = iroha::CreateAccountAddDirect(fbb, account_vec.get());

    std::unique_ptr<std::vector<flatbuffers::Offset<iroha::Signature>>> signature_vec(
            new std::vector<flatbuffers::Offset<iroha::Signature>>()
    );
    std::unique_ptr<std::vector<uint8_t>> signed_message(new std::vector<uint8_t>());
    signed_message->emplace_back('a');
    signed_message->emplace_back('b');
    signed_message->emplace_back('c');
    signed_message->emplace_back('d');

    signature_vec->emplace_back(
        iroha::CreateSignatureDirect(fbb,publicKey, signed_message.get(),1234567)
    );

    auto tx_offset = iroha::CreateTransactionDirect(
        fbb,
        publicKey,
        iroha::Command::Command_AccountAdd,
        command.Union(),
        signature_vec.get(),
        nullptr,
        0
    );
    fbb.Finish(tx_offset);
    auto tx = flatbuffers::BufferRef<iroha::Transaction>(
            fbb.GetBufferPointer(),
            fbb.GetSize()
    );

    std::cout << flatbuffer_service::toString(*tx.GetRoot()) << std::endl;
}

TEST(FlatbufferServicePeerTest, PeerService) {

}

/*
WIP
TEST(FlatbufferServiceTest, toConsensusEvent) {
  flatbuffers::FlatBufferBuilder fbbTransaction;

  std::vector<flatbuffers::Offset<flatbuffers::String>> signatories;
  signatories.push_back(fbbTransaction.CreateString("publicKey1"));

  auto accountBuf = flatbuffer_service::CreateAccountBuffer(
      "PublicKey", "Alias", signatories, 1);

  auto txOffset = ::iroha::CreateTransactionDirect(
      fbbTransaction, "Creator PubKey", iroha::Command_AssetAdd,
      ::iroha::CreateAccountAddDirect(fbbTransaction, &accountBuf), &signatures,
      &hashes, attachmentOffset);
  auto consensusEvent = flatbuffer_service::toConsensusEvent(txOffset);

  
}
*/

