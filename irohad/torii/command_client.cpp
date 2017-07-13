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

/*
 * This client is used by peer service sending tx to change state peers.
 */
namespace torii {

  using iroha::protocol::Transaction;
  using iroha::protocol::ToriiResponse;

  class CommandClient {
  public:
    CommandClient(const std::string& ip, int port)
      : stub_(iroha::protocol::CommandService::NewStub(
      grpc::CreateChannel(ip + ":" + std::to_string(port), grpc::InsecureChannelCredentials())))
    {}

    ToriiResponse Torii(const Transaction& tx) {
      ToriiResponse response;

      std::unique_ptr<grpc::ClientAsyncResponseReader<iroha::protocol::ToriiResponse>> rpc(
        stub_->AsyncTorii(&context_, tx, &cq_)
      );

      using State = network::UntypedCall<torii::CommandServiceHandler>::State;

      rpc->Finish(&response, &status_, (void *)static_cast<int>(State::ResponseSent));

      void* got_tag;
      bool ok = false;
      assert(cq_.Next(&got_tag, &ok));
      assert(got_tag == (void *)static_cast<int>(State::ResponseSent));
      assert(ok);

      if (status_.ok()) {
        return response;
      }
      response.set_code(iroha::protocol::ResponseCode::FAIL);
      response.set_message("RPC failed");
      return response;
    }

  private:
    grpc::ClientContext context_;
    std::unique_ptr<iroha::protocol::CommandService::Stub> stub_;
    grpc::CompletionQueue cq_;
    grpc::Status status_;
  };

  ToriiResponse sendTransaction(const Transaction& tx,
                                const std::string& targetPeerIp,
                                int targetPeerPort) {
    CommandClient client(targetPeerIp, targetPeerPort);
    return client.Torii(tx);
  }

}  // namespace torii
