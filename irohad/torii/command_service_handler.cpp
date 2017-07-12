/*
Copyright 2016 Soramitsu Co., Ltd.

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

#include <endpoint.grpc.pb.h>
#include <network/grpc_async_service.hpp>
#include <network/grpc_call.hpp>
#include <torii/command_service_handler.hpp>

namespace prot = iroha::protocol;

namespace torii {

  /**
   * to handle async rpcs of CommandService.
   */
  /**
   * requires builder to use same server.
   * @param builder
   */
  CommandServiceHandler::CommandServiceHandler(::grpc::ServerBuilder& builder) {
    builder.RegisterService(&asyncService_);
    cq_ = builder.AddCompletionQueue();
  }

  ~CommandServiceHandler::CommandServiceRpcsHandler() override {
    bool didShutdown = false;
    { std::unique_lock }

    if (didShutdown) {
      // Alarm
    }
  }

  /**
   * handles all rpc in CommandService.
   * We use
   */
  void CommandServiceHandler::handleRpcs() override {
    enqueueRequest<prot::Transaction, prot::ToriiResponse>(
        &prot::CommandService::AsyncService::RequestTorii,
        &CommandServiceHandler::ToriiHandler);

    void* tag;
    bool ok;
    while (cq_->Next(&tag, &ok)) {
      auto callbackTag =
          static_cast<network::UntypedCall<CommandServiceHandler>*>(tag);
      if (callbackTag) {
        callbackTag->onCompleted(*this);
      } else {
        cq_->Shutdown();
      }
    }
  }

  /**
   * releases the completion queue of CommandService.
   * @note Call this method after calling server->Shutdown() in ServerRunner
   */
  void CommandServiceHandler::shutdown() override { cq_->Shutdown(); }

  /**
   * extracts request and response from Call instance
   * and calls an actual CommandService::AsyncTorii() implementation.
   * then, creates a new Call instance to serve an another client.
   */
  void CommandServiceHandler::ToriiHandler() {}

}  // namespace torii
