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
    cq_ = builder.AddCompletionQueue();
  }

  CommandServiceHandler::~CommandServiceHandler() {
    //while (cq_->AsyncNext(&tag, &ok, gpr_time_from_seconds(1, GPR_CLOCK_MONOTONIC)) == 1) ;
    //delete shutdownAlarm_;
  }

  /**
   * shuts down service handler.
   * specifically, enqueues a special event that causes the completion queue to be shut down.
   */
  void CommandServiceHandler::shutdown() {
    //void* tag;
    //bool ok;
    cq_->Shutdown();
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
    auto stat = CommandService::ToriiAsync(call->request(), call->response());
    call->sendResponse(stat);

    // Spawn a new Call instance to serve an another client.
    enqueueRequest<prot::Transaction, prot::ToriiResponse>(
      &prot::CommandService::AsyncService::RequestTorii,
      &CommandServiceHandler::ToriiHandler
    );
  }

}  // namespace torii
