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

#include <torii/command_client.hpp>
#include <torii/command_service_handler.hpp>
#include <network/grpc_call.hpp>
#include <block.pb.h>
#include <grpc++/grpc++.h>
#include <thread>
#include <thread_pool.hpp>

namespace torii {

  using iroha::protocol::Transaction;
  using iroha::protocol::ToriiResponse;

  /*
   * avoids from multiple-definition of ThreadPool
   * tp::ThradPool is type alias, not class. So we can't use struct ThreadPool;
   * We shouldn't know about ThreadPoolImpl, that is template class.
   */
  struct ThreadContainer {
    tp::ThreadPool pool;
  };

  struct ToriiAsyncClientCall {
    iroha::protocol::ToriiResponse response;
    grpc::ClientContext context;
    grpc::Status status;
    std::unique_ptr<grpc::ClientAsyncResponseReader<iroha::protocol::ToriiResponse>> response_reader;
  };

  CommandClient::CommandClient(const std::string& ip, int port)
    : stub_(iroha::protocol::CommandService::NewStub(
    grpc::CreateChannel(ip + ":" + std::to_string(port), grpc::InsecureChannelCredentials()))),
      listenerPool_(new ThreadContainer)
  {}

  CommandClient::~CommandClient() {
    delete listenerPool_;
  }

  /**
   * requests tx to a torii server and returns response (blocking, sync)
   * @param tx
   * @return ToriiResponse
   */
  ToriiResponse CommandClient::ToriiBlocking(const Transaction& tx) {
    ToriiResponse response;

    std::unique_ptr<grpc::ClientAsyncResponseReader<iroha::protocol::ToriiResponse>> rpc(
      stub_->AsyncTorii(&context_, tx, &cq_)
    );

    using State = network::UntypedCall<torii::CommandServiceHandler>::State;

    rpc->Finish(&response, &status_, (void *)static_cast<int>(State::ResponseSent));

    void* got_tag;
    bool ok = false;

    if (!cq_.Next(&got_tag, &ok)) {  // CompletionQueue::Next() is blocking.
      throw std::runtime_error("CompletionQueue::Next() returns error");
    }

    assert(got_tag == (void *)static_cast<int>(State::ResponseSent));
    assert(ok);

    if (status_.ok()) {
      return response;
    }

    response.set_code(iroha::protocol::ResponseCode::FAIL);
    response.set_message("RPC failed");
    return response;
  }


  /*
   * TODO(motxx): We can't use CommandClient::ToriiNonBlocking() for now. gRPC causes the error
   * E0714 04:24:40.045388600    4346 sync_posix.c:60]            assertion failed: pthread_mutex_lock(mu) == 0
   */
  /*
  void CommandClient::ToriiNonBlocking(
    const Transaction& tx,
    const std::function<void(ToriiResponse& response)>& callback)
  {
    ToriiAsyncClientCall* call = new ToriiAsyncClientCall;
    call->response_reader = stub_->AsyncTorii(&call->context, tx, &cq_);
    call->response_reader->Finish(&call->response, &call->status, (void*)call);

    listenerPool_->pool.post(std::bind(ToriiNonBlockingListener, cq_, callback));
  }
  */

  void CommandClient::ToriiNonBlockingListener(
    grpc::CompletionQueue& cq,
    const std::function<void(ToriiResponse& response)>& callback)
  {
    void* got_tag;
    bool ok = false;

    while (cq.Next(&got_tag, &ok)) {
      ToriiAsyncClientCall* call = static_cast<ToriiAsyncClientCall*>(got_tag);
      assert(ok); // guarantees the request for updates by Finish()

      if (call->status.ok()) {
        callback(call->response);
      } else {
        ToriiResponse responseFailure;
        responseFailure.set_code(iroha::protocol::ResponseCode::FAIL);
        responseFailure.set_message("RPC failed");
        callback(responseFailure);
      }

      delete call;
    }
  }


}  // namespace torii
