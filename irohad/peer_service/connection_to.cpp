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

#ifndef SHORT_TIMER_LOW
// milliseconds
#define SHORT_TIMER_LOW 1000
#endif

#ifndef SHORT_TIMER_HIGH
// milliseconds
#define SHORT_TIMER_HIGH 2000
#endif

#ifndef LONG_TIMER_LOW
// milliseconds
#define LONG_TIMER_LOW (1000 * 60 * 1)
#endif

#ifndef LONG_TIMER_HIGH
// milliseconds
#define LONG_TIMER_HIGH (1000 * 60 * 60)
#endif

namespace peerservice {
  std::random_device ConnectionTo::random_device;
  // generator with random distribution
  std::default_random_engine ConnectionTo::generator(
      ConnectionTo::random_device());
  // uniform distribution for timer : from 1 sec to 2 secs
  std::uniform_int_distribution<uint32_t> ConnectionTo::next_short_timer(
      SHORT_TIMER_LOW, SHORT_TIMER_HIGH);

  // uniform distribution for timer: from 1 min to 1 hour
  std::uniform_int_distribution<uint32_t> ConnectionTo::next_long_timer(
      LONG_TIMER_LOW, LONG_TIMER_HIGH);  // ms

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

  void ConnectionTo::start_timer(
      std::uniform_int_distribution<uint32_t> &distr) {
    if (timer->active()) timer->stop();

    auto timeout_ms = std::chrono::milliseconds(distr(ConnectionTo::generator));
    timer->start(timeout_ms, uvw::TimerHandle::Time{0});
  }

  void ConnectionTo::ping(Heartbeat *request) {
    if (request == nullptr) throw std::invalid_argument("request is nullptr");

    grpc::ClientContext context;
    grpc::Status status;
    Heartbeat answer;

    status = stub_->RequestHeartbeat(&context, *request, &answer);
    printf("[my ledger is %lu] ping %s... ", request->height(),
           context.peer().c_str());

    // TODO: validate heartbeat messages
    if (status.ok()) {
      // peer is alive
      this->make_online();
      publish(answer);  // publish event to uvw

    } else {
      if (status.error_code() == grpc::StatusCode::CANCELLED) {
        // our heartbeat is invalid (this->myHeartbeat)
        // TODO handle this
        throw std::invalid_argument("our heartbeat is invalid");
      }

      // peer is dead
      this->make_offline();
    }
  }

  void ConnectionTo::make_online() noexcept {
    // TODO: change to logger
    printf("alive\n");
    this->online = true;
    this->start_timer(next_short_timer);
  }

  void ConnectionTo::make_offline() noexcept {
    // TODO: change to logger
    printf("dead\n");
    this->online = false;
    this->start_timer(next_long_timer);
  }
}
