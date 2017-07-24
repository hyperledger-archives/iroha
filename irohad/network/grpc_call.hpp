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

#ifndef NETWORK_GRPC_CALL_HPP
#define NETWORK_GRPC_CALL_HPP

#include <grpc++/grpc++.h>
#include <assert.h>
#include <network/grpc_async_service.hpp>

namespace network {

  /**
   * to use polymorphism in ServiceHandler::handleRpcs()
   * @tparam ServiceHandler
   */
  template <typename ServiceHandler>
  class UntypedCall {
  public:
    virtual ~UntypedCall() {}

    enum class State { RequestCreated, ResponseSent };

    /**
     * invokes when state is RequestReceivedTag.
     * @param serviceHandler - an instance that has all rpc handlers. e.g. CommandService
     */
    virtual void requestReceived(ServiceHandler* serviceHandler) = 0;

    /**
     * invokes when state is ResponseSentTag.
     */
    virtual void responseSent() = 0;

    /**
     * owns concrete Call type and executes derived functions.
     * container for vtable to work if casts UntypedCall<> from void*
     */
    class CallOwner {
    public:
      CallOwner(UntypedCall* call, UntypedCall::State state)
        : call_(call), state_(state) {}

      /**
       * selects a procedure by state and invokes it by using polymorphism.
       * this is called from ServiceHandler::handleRpcs()
       * @param serviceHandler - an instance that has all rpc handlers. e.g. CommandService
       */
      void onCompleted(ServiceHandler *serviceHandler) {
        switch (state_) {
          case UntypedCall::State::RequestCreated: {
            call_->requestReceived(serviceHandler);
            break;
          }
          case UntypedCall::State::ResponseSent: {
            call_->responseSent();
            break;
          }
        }
      }

    private:
      UntypedCall* call_; // owns concrete Call type, works vtable.
      const UntypedCall::State state_;
    };
  };

  /**
   * to manage the state of one rpc.
   * @tparam ServiceHandler - class that has interface GrpcAsyncService.
   * @tparam AsyncService - [SomeService]::AsyncService in *.grpc.pb.h
   * @tparam RequestType - type of a request from client
   * @tparam ResponseType - type of a response to client
   */
  template <typename ServiceHandler, typename AsyncService, typename RequestType, typename ResponseType>
  class Call : public UntypedCall<ServiceHandler> {
  public:

    using RpcHandlerType    = network::RpcHandler<ServiceHandler, AsyncService, RequestType, ResponseType>;
    using RequestMethodType = network::RequestMethod<AsyncService, RequestType, ResponseType>;
    using CallType          = Call<ServiceHandler, AsyncService, RequestType, ResponseType>;
    using UntypedCallType   = UntypedCall<ServiceHandler>;
    using CallOwnerType     = typename UntypedCallType::CallOwner;

    Call(RpcHandlerType rpcHandler)
      : rpcHandler_(rpcHandler), responder_(&ctx_) {}

    virtual ~Call() {}

    /**
     * invokes when state is RequestReceivedTag.
     * this method is called by onCompleted() in super class (UntypedCall).
     * @param serviceHandler - an instance that has all rpc handlers. e.g. CommandService
     */
    void requestReceived(ServiceHandler* serviceHandler) override {
      (serviceHandler->*rpcHandler_)(this);
    }

    /**
     * invokes when state is ResponseSentTag.
     * this method is called by onCompleted() in super class (UntypedCall).
     * @param serviceHandler - an instance that has all rpc handlers. e.g. CommandService
     */
    void responseSent() override {
      // response has been sent and delete the Call instance.
      delete this;
    }

    /**
     * notifies response and grpc::Status when finishing handling rpc.
     * @param status
     */
    void sendResponse(::grpc::Status status) {
      responder_.Finish(response_, status, &ResponseSentTag);
    }

    /**
     * creates a Call instance for one rpc and enqueues it
     * to the completion queue.
     * @param serviceHandler
     * @param cq
     * @param requestMethod
     * @param rpcHandler
     */
    static void enqueueRequest(AsyncService* asyncService,
                               ::grpc::ServerCompletionQueue* cq,
                               RequestMethodType requestMethod,
                               RpcHandlerType rpcHandler) {
      auto call = new CallType(rpcHandler);

      (asyncService->*requestMethod)(&call->ctx_, &call->request(),
                                     &call->responder_, cq, cq,
                                     &call->RequestReceivedTag);
    }

  public:
    auto& request()  { return request_; }
    auto& response() { return response_; }

  private:
    CallOwnerType RequestReceivedTag { this, UntypedCallType::State::RequestCreated };
    CallOwnerType ResponseSentTag { this, UntypedCallType::State::ResponseSent };

  private:
    RpcHandlerType rpcHandler_;
    RequestType request_;
    ResponseType response_;
    ::grpc::ServerContext ctx_;
    ::grpc::ServerAsyncResponseWriter<ResponseType> responder_;
  };

}  // namespace network

#endif  // NETWORK_GRPC_CALL_HPP
