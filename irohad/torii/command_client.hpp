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

#ifndef TORII_COMMAND_CLIENT_HPP
#define TORII_COMMAND_CLIENT_HPP

#include <endpoint.grpc.pb.h>
#include <endpoint.pb.h>
#include <grpc++/grpc++.h>
#include <grpc++/channel.h>
#include <memory>

namespace torii {

  /*
   * avoids from multiple-definition of ThreadPool
   * tp::ThradPool is type alias, not class. So we can't use struct ThreadPool;
   * We shouldn't know about ThreadPoolImpl, that is template class.
   */
  struct ThreadContainer;

  /**
   * CommandClient is used by peer service.
   */
  class CommandClient {
  public:
    CommandClient(const std::string& ip, int port);
    ~CommandClient();

    /**
     * requests tx to a torii server and returns response (blocking, sync)
     * @param tx
     * @return ToriiResponse
     */
    iroha::protocol::ToriiResponse ToriiBlocking(const iroha::protocol::Transaction& tx);

    /*
     * TODO(motxx): We can't use CommandClient::ToriiNonBlocking() for now. gRPC causes the error
     * E0714 04:24:40.045388600    4346 sync_posix.c:60]            assertion failed: pthread_mutex_lock(mu) == 0
     */
    /*
    void ToriiNonBlocking(const iroha::protocol::Transaction& tx,
                          const std::function<void(iroha::protocol::ToriiResponse& response)>& callback);
     */

  private:
    static void ToriiNonBlockingListener(
      grpc::CompletionQueue& cq,
      const std::function<void(iroha::protocol::ToriiResponse& response)>& callback);

  private:
    grpc::ClientContext context_;
    std::unique_ptr<iroha::protocol::CommandService::Stub> stub_;
    grpc::CompletionQueue cq_;
    grpc::Status status_;
    ThreadContainer* listenerPool_; // cannot use smart pointer because of avoiding redefinition.
  };

}  // namespace torii

#endif  // TORII_COMMAND_CLIENT_HPP
