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
#include <grpc/support/time.h>

namespace prot = iroha::protocol;

namespace torii {

  /**
   * registers async command service
   * @param builder
   */
  CommandServiceHandler::CommandServiceHandler(::grpc::ServerBuilder& builder) {
    builder.RegisterService(&asyncService_);
    completionQueue_ = builder.AddCompletionQueue();
  }

  CommandServiceHandler::~CommandServiceHandler() {}

  /**
   * shuts down service handler. (actually, shuts down completion queue only)
   */
  void CommandServiceHandler::shutdown() {
    completionQueue_->Shutdown();
  }

  /**
   * handles rpcs loop in CommandService.
   */
  void CommandServiceHandler::handleRpcs() {
    enqueueRequest<prot::Transaction, prot::ToriiResponse>(
        &prot::CommandService::AsyncService::RequestTorii,
        &CommandServiceHandler::ToriiHandler
    );

    /**
     * tag is a state corresponding to one rpc connection.
     * ok is true if read a regular event, false otherwise (e.g. grpc::Alarm is not a regular event).
     */
    void* tag;
    bool ok;

    /**
     * pulls a state of a new client's rpc request from completion queue.
     * If no request, CompletionQueue::Next() waits a new request (blocks this thread).
     * CompletionQueue::Next() returns false if completionQueue_->Shutdown() is executed.
     */
    while (completionQueue_->Next(&tag, &ok)) {
      auto callbackTag =
          static_cast<network::UntypedCall<CommandServiceHandler>::CallOwner*>(tag);
      if (ok && callbackTag) {
        /*assert(callbackTag);*/
        callbackTag->onCompleted(this);
      } else {
        isShutdownCompletionQueue_ = true;
        break;
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

    CommandService::ToriiAsync(call->request(), call->response());
    call->sendResponse(grpc::Status::OK); // TODO(motxx) currently, grpc::Status::CANCELLED is not supported.

    // Spawn a new Call instance to serve an another client.
    enqueueRequest<prot::Transaction, prot::ToriiResponse>(
      &prot::CommandService::AsyncService::RequestTorii,
      &CommandServiceHandler::ToriiHandler
    );
  }

}  // namespace torii
