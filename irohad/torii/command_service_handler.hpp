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

#include <network/grpc_async_service.hpp>
#include <network/grpc_call.hpp>
#include <endpoint.grpc.pb.h>
#include <endpoint.pb.h>
#include <grpc++/alarm.h>

namespace torii {
  /**
   * to handle rpcs loop of CommandService.
   */
  class CommandServiceHandler : public network::GrpcAsyncService {
  public:

    /**
     * requires builder to use same server.
     * @param builder
     */
    CommandServiceHandler(::grpc::ServerBuilder &builder);

    virtual ~CommandServiceHandler() override;

    template <typename RequestType, typename ResponseType>
    using CommandServiceCall =
    network::Call<
      CommandServiceHandler,
      iroha::protocol::CommandService::AsyncService,
      RequestType,
      ResponseType
    >;

    /**
     * handles rpcs loop in CommandService.
     */
    virtual void handleRpcs() override;

    /**
     * releases the completion queue of CommandService.
     * @note Call this method after calling server->Shutdown() in ServerRunner
     */
    virtual void shutdown() override;

  private:

    /**
     * helper to call Call::enqueueRequest()
     * @param requester  - pointer to request method. e.g. &CommandService::AsyncService::RequestTorii
     * @param rpcHandler - handler of rpc in ServiceHandler.
     */
    template <typename RequestType, typename ResponseType>
    void enqueueRequest(
      network::RequestMethod<iroha::protocol::CommandService::AsyncService,
      RequestType, ResponseType> requester,
      network::RpcHandler<
      CommandServiceHandler, iroha::protocol::CommandService::AsyncService,
      RequestType, ResponseType> rpcHandler
    ) {
      std::unique_lock<std::mutex> lock(mtx_);
      if (!isShutdown_) {
        CommandServiceCall<iroha::protocol::Transaction, iroha::protocol::ToriiResponse>::enqueueRequest(
          &asyncService_, cq_.get(), requester, rpcHandler
        );
      }
    }

    /**
     * extracts request and response from Call instance
     * and calls an actual CommandService::AsyncTorii() implementation.
     * then, creates a new Call instance to serve an another client.
     */
    void ToriiHandler(CommandServiceCall<
      iroha::protocol::Transaction, iroha::protocol::ToriiResponse>*);

  private:
    iroha::protocol::CommandService::AsyncService asyncService_;
    std::unique_ptr<grpc::ServerCompletionQueue> cq_;
    std::mutex mtx_;
    bool isShutdown_ = false;
    ::grpc::Alarm* shutdownAlarm_ = nullptr;
  };
}  // namespace torii

#endif // TORII_COMMAND_SERVICE_HANDLER_HPP