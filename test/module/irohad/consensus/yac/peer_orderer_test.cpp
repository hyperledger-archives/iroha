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
#include <cmath>
#include <unordered_map>
#include <vector>
#include "consensus/yac/impl/peer_orderer_impl.hpp"
#include "module/irohad/ametsuchi/ametsuchi_mocks.hpp"
#include "module/irohad/consensus/yac/yac_mocks.hpp"

using namespace iroha::ametsuchi;
using namespace iroha::consensus::yac;

using namespace std;
using ::testing::Return;

size_t N_PEERS = 4;

class YacPeerOrdererTest : public ::testing::Test {
 public:
  YacPeerOrdererTest() : orderer(make_shared<MockPeerQuery>()) {}

  void SetUp() override {
    wsv = make_shared<MockPeerQuery>();
    orderer = PeerOrdererImpl(wsv);
  }

  std::vector<iroha::model::Peer> peers = [] {
    std::vector<iroha::model::Peer> result;
    for (size_t i = 1; i <= N_PEERS; ++i) {
      result.push_back(iroha::consensus::yac::mk_peer(std::to_string(i)));
    }
    return result;
  }();

  shared_ptr<MockPeerQuery> wsv;
  PeerOrdererImpl orderer;
};

TEST_F(YacPeerOrdererTest, PeerOrdererInitialOrderWhenInvokeNormalCase) {
  cout << "----------| InitialOrder() => valid object |----------" << endl;

  EXPECT_CALL(*wsv, getLedgerPeers()).WillOnce(Return(peers));
  auto order = orderer.getInitialOrdering();
  ASSERT_EQ(order.value().getPeers(), peers);
}

TEST_F(YacPeerOrdererTest, PeerOrdererInitialOrderWhenInvokeFailCase) {
  cout << "----------| InitialOrder() => nullopt case |----------" << endl;

  EXPECT_CALL(*wsv, getLedgerPeers()).WillOnce(Return(nonstd::nullopt));
  auto order = orderer.getInitialOrdering();
  ASSERT_EQ(order, nonstd::nullopt);
}

TEST_F(YacPeerOrdererTest, PeerOrdererOrderingWhenInvokeNormalCase) {
  cout << "----------| Order() => valid object |----------" << endl;

  EXPECT_CALL(*wsv, getLedgerPeers()).WillOnce(Return(peers));
  auto order = orderer.getOrdering(YacHash());
  ASSERT_EQ(order.value().getPeers().size(), peers.size());
}

TEST_F(YacPeerOrdererTest, PeerOrdererOrderingWhenInvokeFaillCase) {
  cout << "----------| Order() => nullopt case |----------" << endl;

  EXPECT_CALL(*wsv, getLedgerPeers()).WillOnce(Return(nonstd::nullopt));
  auto order = orderer.getOrdering(YacHash());
  ASSERT_EQ(order, nonstd::nullopt);
}

TEST_F(YacPeerOrdererTest, FairnessTest) {
  // Calculate number of permutations of peers
  size_t comb = std::tgamma(N_PEERS+1);
  // Run experiments 30 times for each combination
  size_t times = comb * 30;
  std::unordered_map<std::string, int> histogram;
  EXPECT_CALL(*wsv, getLedgerPeers())
      .Times(times)
      .WillRepeatedly(Return(peers));
  for (size_t t = 1; t <= times; ++t) {
    std::string hash_string =   std::to_string(t);
    auto order = orderer.getOrdering(YacHash(hash_string, hash_string));
    auto current_peers = order.value().getPeers();
    std::string res = std::accumulate(current_peers.begin(),
                                      current_peers.end(),
                                      std::string(),
                                      [](std::string res, const auto &peer) {
                                        return res + " " + peer.address;
                                      });
    histogram[res]++;
  }
  ASSERT_EQ(comb, histogram.size());

  // Output historgram, should output uniform distribution
  std::cout << "The histogram output: " << std::endl;
  std::for_each(histogram.begin(),
                histogram.end(),
                [](const std::pair<std::string, int> &element) {
                  std::cout << element.first << " :: " << element.second
                            << std::endl;
                });
}