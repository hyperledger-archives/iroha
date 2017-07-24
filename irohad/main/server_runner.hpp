/*
Copyright 2017 Soramitsu Co., Ltd.

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

#include <grpc++/grpc++.h>
#include <grpc++/server_builder.h>

#ifndef MAIN_SERVER_RUNNER_HPP
#define MAIN_SERVER_RUNNER_HPP

namespace torii { class ToriiServiceHandler; }

class ServerRunner {
public:
  ServerRunner(const std::string &ip, int port);
  ~ServerRunner();
  void run();
  void shutdown();
  void waitForServersReady();

private:
  std::unique_ptr<grpc::Server> serverInstance_;
  std::mutex waitForServer_;
  std::condition_variable serverInstanceCV_;

  std::string serverAddress_;
  std::unique_ptr<torii::ToriiServiceHandler> toriiServiceHandler_;
};

#endif  // MAIN_SERVER_RUNNER_HPP
