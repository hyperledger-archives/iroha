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
#include <gmock/gmock.h>
#include <vector>
#include "yac_mocks.hpp"
#include "module/irohad/torii/mock_classes.hpp"
#include "consensus/yac/impl/yac_peer_orderer_impl.hpp"

#include <iostream>
using namespace std;
using ::testing::Return;

TEST(YacPeerOrdererTest, PeerOrdererInitialOrderWhenInvokeNormalCase) {
  cout << "----------| InitialOrder() => valid object |----------" << endl;

  std::vector<iroha::model::Peer> peers = {mk_peer("1"),
                                           mk_peer("2"),
                                           mk_peer("3"),
                                           mk_peer("4")};

  shared_ptr<WsvQueryMock> wsv = make_shared<WsvQueryMock>();
  EXPECT_CALL(*wsv, getPeers()).WillOnce(Return(peers));
  YacPeerOrdererImpl orderer(wsv);
  auto order = orderer.getInitialOrdering();
  ASSERT_EQ(order.value().getPeers(), peers);
}

TEST(YacPeerOrdererTest, PeerOrdererInitialOrderWhenInvokeFailCase) {
  cout << "----------| InitialOrder() => nullopt case |----------" << endl;

  shared_ptr<WsvQueryMock> wsv = make_shared<WsvQueryMock>();
  EXPECT_CALL(*wsv, getPeers()).WillOnce(Return(nonstd::nullopt));
  YacPeerOrdererImpl orderer(wsv);
  auto order = orderer.getInitialOrdering();
  ASSERT_EQ(order, nonstd::nullopt);
}

TEST(YacPeerOrdererTest, PeerOrdererOrderingWhenInvokeNormalCase) {
  cout << "----------| Order() => valid object |----------" << endl;

  std::vector<iroha::model::Peer> peers = {mk_peer("1"),
                                           mk_peer("2"),
                                           mk_peer("3"),
                                           mk_peer("4")};

  shared_ptr<WsvQueryMock> wsv = make_shared<WsvQueryMock>();
  EXPECT_CALL(*wsv, getPeers()).WillOnce(Return(peers));
  YacPeerOrdererImpl orderer(wsv);
  auto order = orderer.getOrdering(YacHash());
  ASSERT_EQ(order.value().getPeers().size(), peers.size());
}

TEST(YacPeerOrdererTest, PeerOrdererOrderingWhenInvokeFaillCase) {
  cout << "----------| Order() => nullopt case |----------" << endl;

  shared_ptr<WsvQueryMock> wsv = make_shared<WsvQueryMock>();
  EXPECT_CALL(*wsv, getPeers()).WillOnce(Return(nonstd::nullopt));
  YacPeerOrdererImpl orderer(wsv);
  auto order = orderer.getOrdering(YacHash());
  ASSERT_EQ(order, nonstd::nullopt);
}