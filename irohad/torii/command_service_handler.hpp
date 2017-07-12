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

#ifndef TORII_COMMAND_SERVICE_HANDLER_HPP
#define TORII_COMMAND_SERVICE_HANDLER_HPP

#include <endpoint.grpc.pb.h>
#include <endpoint.pb.h>

namespace torii {
  class CommandServiceHandler : public network::GrpcAsyncService {
  public:

    /**
     * requires builder to use same server.
     * @param builder
     */
    CommandServiceHandler(::grpc::ServerBuilder &builder);

    ~CommandServiceRpcsHandler() override;

    template <typename RequestType, typename ResponseType>
    using CommandServiceCall =
    network::Call<
      CommandServiceHandler,
      prot::CommandService::AsyncService,
      RequestType,
      ResponseType
    >;

    /**
     * handles all rpc in CommandService.
     * We use
     */
    void handleRpcs() override;

    /**
     * releases the completion queue of CommandService.
     * @note Call this method after calling server->Shutdown() in ServerRunner
     */
    void shutdown() override;

  private:

    /**
     *
     * @param requester
     */
    template <typename RequestType, typename ResponseType>
    void enqueueRequest(
      network::RequestMethod<prot::CommandService::AsyncService,
      RequestType, ResponseType> requester,
      network::RpcHandler<
      CommandServiceHandler, prot::CommandService::AsyncService,
      RequestType, ResponseType> rpcHandler
    ) {
      std::unique_lock<std::mutex> lock(mtx_);
      if (!isShutdown_) {
        CommandServiceCall<prot::Transaction, prot::ToriiResponse>::enqueueRequest(
          &asyncService_, cq_.get(), requester, rpcHandler
        );
      }
    }

    /**
     * extracts request and response from Call instance
     * and calls an actual CommandService::AsyncTorii() implementation.
     * then, creates a new Call instance to serve an another client.
     */
    void ToriiHandler();

  private:
    iroha::protocol::CommandService::AsyncService asyncService_;
    // TODO(motxx): Investigate a required number of completion queues if we use multiple services.
    std::unique_ptr<grpc::ServerCompletionQueue> cq_;
    std::mutex mtx_;  // TODO(motxx): Write the reason of using mutex for ENQUEUE_REQUEST.
    bool isShutdown_ = false;
  };
}  // namespace torii

#endif // TORII_COMMAND_SERVICE_HANDLER_HPP