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
#include <torii/command_service.hpp>
#include <unistd.h>

namespace prot = iroha::protocol;

namespace torii {

  /**
   * registers async command service
   * @param builder
   */
  CommandServiceHandler::CommandServiceHandler(::grpc::ServerBuilder& builder) {
    builder.RegisterService(&asyncService_);
    cq_ = builder.AddCompletionQueue();
  }

  CommandServiceHandler::~CommandServiceHandler() {
    delete shutdownAlarm_;
  }

  /**
   * shuts down service handler.
   * specifically, enqueues a special event that causes the completion queue to be shut down.
   */
  void CommandServiceHandler::shutdown() {
    bool didShutdown = false;
    {
      std::unique_lock<std::mutex> lock(mtx_);
      if (!isShutdown_) {
        isShutdown_ = true;
        didShutdown = true;
      }
    }

    if (didShutdown) {
      // enqueue a special event that causes the completion queue to be shut down.
      // tag is nullptr in order to determine no Call instance allocated when static_cast.
      shutdownAlarm_ = new ::grpc::Alarm(cq_.get(), gpr_now(GPR_CLOCK_MONOTONIC), nullptr);
    }
  }

  /**
   * handles rpcs loop in CommandService.
   */
  void CommandServiceHandler::handleRpcs() {
    enqueueRequest<prot::Transaction, prot::ToriiResponse>(
        &prot::CommandService::AsyncService::RequestTorii,
        &CommandServiceHandler::ToriiHandler
    );

    void* tag;
    bool ok;
    while (cq_->Next(&tag, &ok)) {
      auto callbackTag =
          static_cast<network::UntypedCall<CommandServiceHandler>::CallOwner*>(tag);
      if (ok && callbackTag) {
        callbackTag->onCompleted(this);
      } else {
        // callbackTag is nullptr (and) ok is false
        // if the queue is shutting down.
        cq_->Shutdown();
        isShutdownCompletionQueue_ = true;
      }
    }
  }

  /**
   * extracts request and response from Call instance
   * and calls an actual CommandService::AsyncTorii() implementation.
   * then, spawns a new Call instance to serve an another client.
   */
  void CommandServiceHandler::ToriiHandler(
    CommandServiceCall<prot::Transaction, prot::ToriiResponse>* call) {
    auto stat = CommandService::ToriiAsync(call->request(), call->response());
    call->sendResponse(stat);

    // Spawn a new Call instance to serve an another client.
    enqueueRequest<prot::Transaction, prot::ToriiResponse>(
      &prot::CommandService::AsyncService::RequestTorii,
      &CommandServiceHandler::ToriiHandler
    );
  }

}  // namespace torii
