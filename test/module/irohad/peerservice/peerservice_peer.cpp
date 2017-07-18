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

#include <common/byteutils.hpp>
#include <peer_service/service.hpp>
#include <uvw.hpp>

#define NPEERS 4

using namespace peerservice;

int main(int argc, char** argv) {
  if (argc != 3) {
    printf(
        "Provide 1 argument: total number of peers\n"
        "Provide 2 argument: number 0..total_peers-1\n");
    exit(1);
  }

  size_t TOTAL = (size_t)std::stoi(argv[1]);
  size_t ME = (size_t)std::stoi(argv[2]);

  constexpr auto pubksize = iroha::ed25519::pubkey_t::size();

  std::vector<Node> cluster;
  for (auto i = 0u; i < TOTAL; i++) {
    Node node;
    // pubkeys will be aa..a, bb..b, cc..c, dd..d and so on
    auto pubstr = std::string(pubksize, (char)('a' + i));
    node.pubkey = iroha::to_blob<pubksize>(pubstr);
    node.ip = std::string("127.0.0.1");
    node.port = (uint16_t)(10000 + i);
    cluster.push_back(std::move(node));
  }

  std::string listen_on =
      cluster[ME].ip + ":" + std::to_string(cluster[ME].port);
  printf("LISTEN ON: %s, pubkey: %s\n", listen_on.c_str(),
         cluster[ME].pubkey.to_string().c_str());

  auto loop = uvw::Loop::create();
  PeerServiceImpl ps(cluster, cluster[ME].pubkey, loop);

  /// register peer service. this step is mandatory
  grpc::ServerBuilder builder;
  builder.AddListeningPort(listen_on, grpc::InsecureServerCredentials());
  builder.RegisterService(&ps);
  auto server = builder.BuildAndStart();

  ps.ping();

  loop->run();

  assert(loop->alive());
  return 0;
}