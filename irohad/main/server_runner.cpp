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

#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <logger/logger.hpp>

#include "server_runner.hpp"

namespace server_runner {

  logger::Logger console("ServerRunner");

  // server instances
  std::unique_ptr<grpc::Server> serverInstance;
  std::mutex waitForServer;
  std::condition_variable serverInstanceCV;

  // initial settings
  std::string serverAddress;
  std::vector<grpc::Service *> services;

  void initialize(const std::string &ip, int port,
                  const std::vector<grpc::Service *> &srvs) {
    serverAddress = ip + ":" + std::to_string(port);
    services = srvs;
  }

  void run() {
    grpc::ServerBuilder builder;

    // TODO(motxx): Is it ok to open same port for all services?
    builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
    for (auto srv : services) {
      builder.RegisterService(srv);
    }

    waitForServer.lock();
    serverInstance = builder.BuildAndStart();
    waitForServer.unlock();
    serverInstanceCV.notify_one();

    console.info("Server listening on {}", serverAddress);

    serverInstance->Wait();
  }

  void shutDown() { serverInstance->Shutdown(); }

  bool waitForServersReady() {
    std::unique_lock<std::mutex> lock(waitForServer);
    while (!serverInstance) serverInstanceCV.wait(lock);
  }
}  // namespace server_runner
