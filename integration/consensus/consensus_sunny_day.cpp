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

#include <grpc++/grpc++.h>
#include <gtest/gtest.h>
#include "consensus/yac/impl/network_impl.hpp"
#include "consensus/yac/impl/timer_impl.hpp"
#include "module/irohad/consensus/yac/yac_mocks.hpp"

using ::testing::Return;
using ::testing::An;

using namespace iroha::consensus::yac;

Peer mk_local_peer(uint64_t num) {
  Peer peer;
  peer.address = "0.0.0.0:" + std::to_string(num);
  return peer;
}

class FixedCryptoProvider : public CryptoProviderMock {
 public:
  explicit FixedCryptoProvider(const std::string &public_key) {
    pubkey.fill(0);
    std::copy(public_key.begin(), public_key.end(), pubkey.begin());
  }

  VoteMessage getVote(YacHash hash) override {
    auto vote = CryptoProviderMock::getVote(hash);
    vote.signature.pubkey = pubkey;
    return vote;
  }

  decltype(VoteMessage().signature.pubkey) pubkey;
};

class ConsensusSunnyDayTest : public ::testing::Test {
 public:
  std::shared_ptr<NetworkImpl> network;
  std::shared_ptr<CryptoProviderMock> crypto;
  std::shared_ptr<TimerImpl> timer;
  uint64_t delay = 3 * 1000;
  std::shared_ptr<Yac> yac;

  static uint64_t my_num;
  static Peer my_peer;
  static std::vector<Peer> default_peers;

  static void init(uint64_t num_peers, uint64_t num) {
    my_num = num;
    my_peer = mk_local_peer(10000 + my_num);
    for (decltype(num_peers) i = 0; i < num_peers; ++i) {
      default_peers.push_back(mk_local_peer(10000 + i));
    }
  }
};

uint64_t ConsensusSunnyDayTest::my_num;
Peer ConsensusSunnyDayTest::my_peer;
std::vector<Peer> ConsensusSunnyDayTest::default_peers;

TEST_F(ConsensusSunnyDayTest, SunnyDayTest) {
  network = std::make_shared<NetworkImpl>(my_peer.address, default_peers);
  crypto = std::make_shared<FixedCryptoProvider>(std::to_string(my_num));
  timer = std::make_shared<TimerImpl>();
  yac = Yac::create(std::move(YacVoteStorage()), network, crypto, timer,
                    ClusterOrdering(default_peers), delay);
  network->subscribe(yac);

  auto committed = false;

  yac->on_commit().subscribe([&committed](auto hash) {
    std::cout << "^_^ COMMITTED!!!" << std::endl;
    committed = true;
  });

  EXPECT_CALL(*crypto, verify(An<CommitMessage>()))
      .Times(1)
      .WillRepeatedly(Return(true));
  EXPECT_CALL(*crypto, verify(An<VoteMessage>())).WillRepeatedly(Return(true));

  std::unique_ptr<grpc::Server> server;

  auto thread = std::thread([&server, this] {
    grpc::ServerBuilder builder;
    int port = 0;
    builder.AddListeningPort(my_peer.address, grpc::InsecureServerCredentials(),
                             &port);
    builder.RegisterService(network.get());
    server = builder.BuildAndStart();
    ASSERT_TRUE(server);
    ASSERT_NE(port, 0);
    server->Wait();
  });

  // Wait for other peers to start
  std::this_thread::sleep_for(std::chrono::seconds(10));

  YacHash my_hash("proposal_hash", "block_hash");
  yac->vote(my_hash, ClusterOrdering(default_peers));
  std::this_thread::sleep_for(
      std::chrono::milliseconds(delay * default_peers.size() + 10 * 1000));

  ASSERT_TRUE(committed);

  server->Shutdown();
  if (thread.joinable()) {
    thread.join();
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  uint64_t num_peers = 1, my_num = 0;
  if (argc == 3) {
    num_peers = std::stoul(argv[1]);
    my_num = std::stoul(argv[2]);
  }
  ConsensusSunnyDayTest::init(num_peers, my_num);
  return RUN_ALL_TESTS();
}