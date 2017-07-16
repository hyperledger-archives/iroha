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

#include "connection_to.hpp"
#include "service.hpp"

namespace peerservice {
  std::default_random_engine ConnectionTo::generator;
  // uniform distribution for timer : from 1 sec to 2 secs
  std::uniform_int_distribution<uint16_t> ConnectionTo::next_timer(1000, 2000);

  ConnectionTo::ConnectionTo(const Node &n, std::shared_ptr<uvw::Loop> loop)
      : node(n) {
    if (loop == nullptr) throw std::invalid_argument("loop is null");

    this->online = false;
    this->timer = loop->resource<uvw::TimerHandle>();

    auto to = n.ip + ":" + std::to_string(n.port);
    auto channel = grpc::CreateChannel(to, grpc::InsecureChannelCredentials());
    stub_ = PeerService::NewStub(channel);
  }

  ConnectionTo::~ConnectionTo() { this->timer->close(); }

  void ConnectionTo::start_timer() {
    auto timeout_ms = std::chrono::milliseconds(
        ConnectionTo::next_timer(ConnectionTo::generator));
    timer->start(timeout_ms, uvw::TimerHandle::Time{0});
  }

  Heartbeat ConnectionTo::ping(Heartbeat *request) {
    grpc::ClientContext context;
    grpc::Status status;
    Heartbeat answer;

    status = stub_->RequestHeartbeat(&context, *request, &answer);

    // TODO: validate heartbeat messages

    if (status.ok()) {
      // peer is alive
      this->start_timer();
      this->online = true;
      return answer;
    } else {
      if (status.error_code() == grpc::StatusCode::CANCELLED) {
        // our heartbeat is invalid
        // TODO handle this
        throw "something";
      }

      // peer is dead
      this->online = false;
      this->timer->stop();  // stop timer and wait for heartbeat from him
    }
  }

  void ConnectionTo::reset_timer() { timer->again(); }
}