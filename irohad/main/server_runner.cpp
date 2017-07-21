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
#include <main/server_runner.hpp>
#include <torii/torii_service_handler.hpp>

logger::Logger Log("ServerRunner");

ServerRunner::ServerRunner(const std::string &ip, int port)
    : serverAddress_(ip + ":" + std::to_string(port)) {}

ServerRunner::~ServerRunner() {
  toriiServiceHandler_->shutdown();
}

void ServerRunner::run() {
  grpc::ServerBuilder builder;

  builder.AddListeningPort(serverAddress_, grpc::InsecureServerCredentials());

  // Register services.
  toriiServiceHandler_ = std::make_unique<torii::ToriiServiceHandler>(builder);

  serverInstance_ = builder.BuildAndStart();
  serverInstanceCV_.notify_one();

  Log.info("Server listening on {}", serverAddress_);

  // proceed to server's main loop
  toriiServiceHandler_->handleRpcs();
}

void ServerRunner::shutdown() {
  serverInstance_->Shutdown();

  while (!toriiServiceHandler_->isShutdownCompletionQueue()) {
    usleep(1); // wait for shutting down completion queue
  }
  toriiServiceHandler_->shutdown();
}

void ServerRunner::waitForServersReady() {
  std::unique_lock<std::mutex> lock(waitForServer_);
  while (!serverInstance_) serverInstanceCV_.wait(lock);
}
