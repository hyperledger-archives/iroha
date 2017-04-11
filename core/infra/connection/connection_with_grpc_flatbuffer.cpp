/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

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

#include <grpc++/grpc++.h>

#include <flatbuffers/flatbuffers.h>
#include <consensus/connection/connection.hpp>
#include <service/peer_service.hpp>
#include <util/logger.hpp>

#include <infra/config/iroha_config_with_json.hpp>
#include <infra/config/peer_service_with_json.hpp>

#include <algorithm>
#include <crypto/signature.hpp>
#include <memory>
#include <string>
#include <vector>


namespace connection {

using Sumeragi = ::iroha::Sumeragi;
using ConsensusEvent = ::iroha::ConsensusEvent;
using Response = ::iroha::Response;
using Transaction = ::iroha::Transaction;
using Signature = ::iroha::Signature;

enum ResponseType {
  RESPONSE_OK,
  // wrong signature
  RESPONSE_INVALID_SIG,
  // connection error
  RESPONSE_ERRCONN,
};

// using Response = std::pair<std::string, ResponseType>;
/*
// TODO: very dirty solution, need to be out of here
std::function<RecieverConfirmation(const std::string&)> sign = [](const
std::string &hash) { RecieverConfirmation confirm; Signature signature;
    signature.set_publickey(config::PeerServiceConfig::getInstance().getMyPublicKey());
    signature.set_signature(signature::sign(
            config::PeerServiceConfig::getInstance().getMyPublicKey(),
            hash,
            config::PeerServiceConfig::getInstance().getMyPrivateKey())
    );
    confirm.set_hash(hash);
    confirm.mutable_signature()->Swap(&signature);
    return confirm;
};

std::function<bool(const RecieverConfirmation&)> valid = [](const
RecieverConfirmation &c) { return signature::verify(c.signature().signature(),
c.hash(), c.signature().publickey());
};
*/

namespace iroha {
namespace SumeragiImpl {
namespace Verify {
std::vector<std::function<void(const std::string& from,
                               std::unique_ptr<ConsensusEvent> message)> >
    receivers;
};
namespace Torii {
std::vector<
    std::function<void(const std::string& from, std::unique_ptr<Transaction> message)> >
    receivers;
}
};  // namespace SumeragiImpl
};  // namespace iroha

using grpc::Channel;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ClientContext;
using grpc::Status;

class SumeragiConnectionClient {
 public:
  explicit SumeragiConnectionClient(std::shared_ptr<Channel> channel)
      : stub_(Sumeragi::NewStub(channel)) {}

  Response Verify(const ConsensusEvent& consensusEvent) {
    /*
    Response response;
    logger::info("connection")  <<  "Operation";
    logger::info("connection")  <<  "size: "    <<
    consensusEvent.eventsignatures_size(); logger::info("connection")  <<
    "name: "    <<  consensusEvent.transaction().asset().name();

    ClientContext context;

    Status status = stub_->Verify(&context, consensusEvent, &response);

    if (status.ok()) {
        logger::info("connection")  << "response: " << response.value();
        //return {response.value(), valid(response.confirm()) ? RESPONSE_OK :
    RESPONSE_INVALID_SIG}; } else { logger::error("connection") <<
    status.error_code() << ": " << status.error_message();
        //std::cout << status.error_code() << ": " << status.error_message();
        //return {"RPC failed", RESPONSE_ERRCONN};
    }
     */
  }

  Response Torii(const Transaction& transaction) {
    /*
    Response response;

    ClientContext context;

    Status status = stub_->Torii(&context, transaction, &response);

    if (status.ok()) {
        logger::info("connection")  << "response: " << response.value();
        return {response.value(), RESPONSE_OK};
    } else {
        logger::error("connection") << status.error_code() << ": " <<
    status.error_message();
        //std::cout << status.error_code() << ": " << status.error_message();
        return {"RPC failed", RESPONSE_ERRCONN};
    }
     */
  }

 private:
  std::unique_ptr<Sumeragi::Stub> stub_;
};


class SumeragiConnectionServiceImpl final : public ::iroha::Sumeragi::Service {
 public:
  Status Verify(ServerContext* context,
                const flatbuffers::BufferRef<ConsensusEvent>* request,
                flatbuffers::BufferRef<Response>* response) override {
    const ::iroha::ConsensusEvent* event = request->GetRoot();
    return Status::OK;
  }

  Status Torii(ServerContext* context,
               const flatbuffers::BufferRef<Transaction>* transaction,
               flatbuffers::BufferRef<Response>* response) override {
    return Status::OK;
  }
};


namespace iroha {

namespace SumeragiImpl {

SumeragiConnectionServiceImpl service;

namespace Verify {

bool receive(
    const std::function<void(const std::string&,
                             std::unique_ptr<ConsensusEvent>)>& callback) {
  receivers.push_back(callback);
  return true;
}

bool send(const std::string& ip, const ConsensusEvent&& event) {
  // ToDo
  /*
  auto receiver_ips = config::PeerServiceConfig::getInstance().getIpList();
  if (find(receiver_ips.begin(), receiver_ips.end(), ip) != receiver_ips.end())
  { return true; } else { return false;
  }
  */
  return true;
}

bool sendAll(const ConsensusEvent&& event) {
  // ToDo
  /*
  auto receiver_ips = config::PeerServiceConfig::getInstance().getIpList();
  for (auto &ip : receiver_ips) {
      // ToDo
      if (ip != config::PeerServiceConfig::getInstance().getMyIp()) {
          send(ip, std::move(event));
      }
  }
  */
  return true;
}
}  // namespace Verify

namespace Torii {
bool receive(
    const std::function<void(const std::string&, std::unique_ptr<Transaction>)>& callback) {
  receivers.push_back(callback);
  return true;
}
}  // namespace Torii
}  // namespace SumeragiImpl
};  // namespace iroha

ServerBuilder builder;
grpc::Server* server = nullptr;
std::condition_variable server_cv;

void initialize_peer() {
  // ToDo catch exception of to_string
  auto address =
      "0.0.0.0:" +
      std::to_string(
          config::IrohaConfigManager::getInstance().getGrpcPortNumber(50051));
  SumeragiConnectionServiceImpl service;
  grpc::ServerBuilder builder;
  builder.AddListeningPort(address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
}

int run() {
  server = builder.BuildAndStart().release();
  server_cv.notify_one();
  server->Wait();
  return 0;
}
void finish() {
  server->Shutdown();
  delete server;
}

};  // namespace connection
