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

#include <iostream>
#include <crypto/hash.hpp>
#include <flatbuffers/flatbuffers.h>
#include <transaction_generated.h>
#include <service/flatbuffer_service.h>
#include <infra/config/peer_service_with_json.hpp>
#include <utils/datetime.hpp>

/**
 * Create tx from client via REST API and send iroha node.
 */
int main() {

  const auto alias = std::string("sate");
  const auto commandType = "PUT/accounts";
  const auto creatorPubKey = "rI9Bks2reclulb+3/RENiouWSNaBHbRH6wo7BUoQ1Tc=";//config::PeerServiceConfig::getInstance().getMyPublicKey();
  const auto creatorPrivateKey = "+KkxVvvBHkiJIe7sxX0D7ELya4AWc7hKuwJnjYcqUk+02dll48oyw028K1Hjr+v9osVdwb1KdG6FNQAha9qgSQ==";
  const auto timestamp = datetime::unixtime();

  // sha3_256(alias + “PUT/accounts” + creator + timestamp)
  const auto hashable = alias + commandType + creatorPubKey + std::to_string(timestamp);
  const auto hash = hash::sha3_256_hex(hashable);

  flatbuffers::FlatBufferBuilder fbb;
  auto sigOffset = flatbuffer_service::primitives::CreateSignature(
    fbb, hash, timestamp);
  fbb.Finish(sigOffset);
  auto buf = fbb.GetBufferPointer();
  auto flatbuf = std::vector<uint8_t>{buf, buf + fbb.GetSize()};
  auto sigroot = flatbuffers::GetRoot<::iroha::Signature>(flatbuf.data());
  std::string signature_str{sigroot->signature()->begin(), sigroot->signature()->end()};

  std::cout << "client public key: \"" << config::PeerServiceConfig::getInstance().getMyPublicKey() << "\"" << std::endl;
  std::cout << "client private key: \"" << config::PeerServiceConfig::getInstance().getMyPrivateKey() << "\"" << std::endl;

  std::cout << std::endl;

  std::cout << "alias: \"" << alias << "\"" << std::endl;
  std::cout << "\"" << commandType << "\"" << std::endl;
  std::cout << "creator_pubkey: \"" << creatorPubKey << "\"" << std::endl;
  std::cout << "(creator private key: \"" << creatorPrivateKey << "\")" << std::endl;
  std::cout << "(timestamp: " << timestamp << ")" << std::endl;
  std::cout << "(before hashed string: \"" << hashable <<  "\")" << std::endl;
  std::cout << "hash: \"" << hash << "\"" << std::endl;
  std::cout << "signature: \"" << signature_str << "\"" << std::endl;
}