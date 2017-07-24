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

#ifndef NETWORK_GRPC_ASYNC_SERVICE_HPP
#define NETWORK_GRPC_ASYNC_SERVICE_HPP

namespace network {

  template <typename ServiceHandler, typename AsyncService, typename RequestType, typename ResponseType>
  class Call;

  /**
   * interface of handling rpcs in a service.
   */
  class GrpcAsyncService {
  public:
    virtual ~GrpcAsyncService() {}

    /**
     * handles incoming RPCs.
     * 1. Create a Call instance that handles a rpc.
     * 2. Execute CompletionQueue::Next() and gets current status with tag.
     * 3. Handle a rpc associated with the tag. For polymorphism, cast the tag with UntypedCall.
     * 4. Back to 2
     */
    virtual void handleRpcs() = 0;

    /**
     * stops spawning new Call instances and enqueues a special event
     * that causes the completion queue to be shut down.
     */
    virtual void shutdown() = 0;
  };

  /**
   * to refer a method that requests one rpc.
   * e.g. iroha::protocol::AsyncService::RequestTorii
   *
   * AsyncService - e.g. iroha::protocol::CommandService::AsyncService
   * RequestType  - e.g. Transaction   in rpc Torii (Transaction) returns (ToriiResponse)
   * ResponseType - e.g. ToriiResponse in rpc Torii (Transaction) returns (ToriiResponse)
   */
  template <typename AsyncService, typename RequestType, typename ResponseType>
  using RequestMethod = void (AsyncService::*)(
    ::grpc::ServerContext*, RequestType*,
    ::grpc::ServerAsyncResponseWriter<ResponseType>*,
    ::grpc::CompletionQueue*, ::grpc::ServerCompletionQueue*, void*);

  /**
   * to refer a method that extracts request and response from Call instance
   * and creates a new Call instance to serve new clients.
   */
  template <typename ServiceHandler, typename AsyncService, typename RequestType, typename ResponseType>
  using RpcHandler = void (ServiceHandler::*)(
    Call<ServiceHandler, AsyncService, RequestType, ResponseType>*);

}  // namespace network

#endif  // NETWORK_GRPC_ASYNC_SERVICE_HPP
