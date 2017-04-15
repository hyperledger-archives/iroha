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

#include <flatbuffers/flatbuffers.h>
#include <grpc++/grpc++.h>
#include <service/flatbuffer_service.h>
#include <consensus/connection/connection.hpp>
#include <crypto/signature.hpp>
#include <infra/config/iroha_config_with_json.hpp>
#include <infra/config/peer_service_with_json.hpp>
#include <service/peer_service.hpp>
#include <util/exception.hpp>
#include <util/logger.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace connection {
/**
 * Using
 */
using Sumeragi = ::iroha::Sumeragi;
using ConsensusEvent = ::iroha::ConsensusEvent;
using Response = ::iroha::Response;
using Transaction = ::iroha::Transaction;
using Signature = ::iroha::Signature;

using grpc::Channel;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ClientContext;
using grpc::Status;

/**
 * Enum
 */
enum ResponseType {
  RESPONSE_OK,
  RESPONSE_INVALID_SIG,  // wrong signature
  RESPONSE_ERRCONN,      // connection error
};

/**
 * Store callback function
 */
template <class CallBackFunc>
class Receiver {
 public:
  void set(CallBackFunc&& rhs) {
    if (receiver_) {
      throw exception::DuplicateSetException(
          "Receiver<" + std::string(typeid(CallBackFunc).name()) + ">",
          __FILE__);
    }
    receiver_ = std::make_shared<CallBackFunc>(rhs);
  }

  // ToDo rewrite operator() overload.
  void invoke(const std::string& from,
              std::unique_ptr<::iroha::Transaction> argv) {
    (*receiver_)(from, std::move(argv));
  }

  // ToDo rewrite operator() overload.
  void invoke(const std::string& from,
              std::unique_ptr<::iroha::ConsensusEvent> argv) {
    (*receiver_)(from, std::move(argv));
  }

 private:
  std::shared_ptr<CallBackFunc> receiver_;
};

/************************************************************************************
 * Verify
 ************************************************************************************/
namespace iroha {
namespace SumeragiImpl {
namespace Verify {

Receiver<Verify::CallBackFunc> receiver;

/**
 * Receive callback
 */
void receive(Verify::CallBackFunc&& callback) {
  receiver.set(std::move(callback));
}

bool send(const std::string& ip, const std::unique_ptr<ConsensusEvent>& event) {
  // ToDo
  if (config::PeerServiceConfig::getInstance().isExistIP(ip)) {
    auto channel =
        grpc::CreateChannel(ip + ":50051", grpc::InsecureChannelCredentials());
    auto stub = ::iroha::Sumeragi::NewStub(channel);

    grpc::ClientContext context;

    auto publicKey = "SamplePublicKey";
    // Build a request with the name set.
    flatbuffers::FlatBufferBuilder fbb;
    std::unique_ptr<std::vector<flatbuffers::Offset<flatbuffers::String>>>
        signatories(new std::vector<flatbuffers::Offset<flatbuffers::String>>(
            {fbb.CreateString("publicKey1")}));
    auto account = ::iroha::CreateAccountDirect(fbb, publicKey, "alias",
                                                signatories.get(), 1);
    std::unique_ptr<std::vector<uint8_t>> account_vec(
        new std::vector<uint8_t>());
    auto command = ::iroha::CreateAccountAddDirect(fbb, account_vec.get());

    std::vector<uint8_t> signaturesBlob;

    {
      flatbuffers::FlatBufferBuilder fbbSignatures;

      // TODO: Tempolary implementation. Use 'sign' function
      std::vector<uint8_t> signature;
      for (auto e : std::string("hash + timestamp + pubkey ?")) {
        signature.push_back(e);
      }

      std::vector<flatbuffers::Offset<::iroha::Signature>> signatures;

      signatures.push_back(::iroha::CreateSignatureDirect(
          fbbSignatures, publicKey, &signature,
          1234567  // TODO: timestamp
          ));

      auto signaturesOffset =
          ::iroha::CreateSignaturesDirect(fbbSignatures, &signatures);

      fbbSignatures.Finish(signaturesOffset);

      auto buf =
          reinterpret_cast<const char*>(fbbSignatures.GetBufferPointer());
      signaturesBlob.assign(buf, buf + fbbSignatures.GetSize());
    }

    auto tx_offset = ::iroha::CreateTransactionDirect(
        fbb, publicKey, ::iroha::Command::Command_AccountAdd, command.Union(),
        &signaturesBlob, nullptr,
        ::iroha::CreateAttachmentDirect(fbb, nullptr, nullptr));

    fbb.Finish(tx_offset);

    auto tx = flatbuffers::BufferRef<::iroha::Transaction>(
        fbb.GetBufferPointer(), fbb.GetSize());

    flatbuffers::BufferRef<::iroha::Response> response;

    // The actual RPC.
    auto status = stub->Torii(&context, tx, &response);

    if (status.ok()) {
      auto msg = response.GetRoot()->message();
      std::cout << "RPC response: " << msg->str() << std::endl;
    } else {
      std::cout << "RPC failed" << std::endl;
    }
    return true;
  }
  return false;
}

bool sendAll(const std::unique_ptr<ConsensusEvent>& event) {
  auto receiver_ips = config::PeerServiceConfig::getInstance().getGroup();
  for (const auto& p : receiver_ips) {
    if (p["ip"].get<std::string>() !=
        config::PeerServiceConfig::getInstance().getMyIpWithDefault("AA")) {
      logger::info("connection") << "Send to " << p["ip"].get<std::string>();
      send(p["ip"].get<std::string>(), event);
    }
  }
  return true;
}
}
}
}  // namespace iroha::SumeragiImpl::Verify


/************************************************************************************
 * Torii
 ************************************************************************************/
namespace iroha {
namespace SumeragiImpl {
namespace Torii {

Receiver<Torii::CallBackFunc> receiver;

void receive(Torii::CallBackFunc&& callback) {
  receiver.set(std::move(callback));
}
}
}
}  // namespace iroha::SumeragiImpl::Torii

/************************************************************************************
 * Connection Client
 ************************************************************************************/
class SumeragiConnectionClient {
 public:
  explicit SumeragiConnectionClient(std::shared_ptr<Channel> channel)
      : stub_(Sumeragi::NewStub(channel)) {}

  ::iroha::Response* Verify(
      const std::unique_ptr<ConsensusEvent>& consensusEvent) const {
    grpc::ClientContext context;
    flatbuffers::BufferRef<Response> responseRef;
    logger::info("connection") << "Operation";
    logger::info("connection")
        << "size: " << consensusEvent->peerSignatures()->size();
    logger::info("connection")
        << "CommandType: "
        << consensusEvent->transactions()->Get(0)->command_type();

    // For now, ConsensusEvent has one transaction.
    auto transaction = consensusEvent->transactions()->Get(0);

    auto newConsensusEvent = flatbuffer_service::toConsensusEvent(*transaction);

    flatbuffers::FlatBufferBuilder fbb;
    // TODO

    auto requestConsensusEventRef =
        flatbuffers::BufferRef<::iroha::ConsensusEvent>(fbb.GetBufferPointer(),
                                                        fbb.GetSize());

    Status status =
        stub_->Verify(&context, requestConsensusEventRef, &responseRef);

    if (status.ok()) {
      logger::info("connection")
          << "response: " << responseRef.GetRoot()->message();
      return responseRef.GetRoot();
    } else {
      logger::error("connection")
          << status.error_code() << ": " << status.error_message();
      return responseRef.GetRoot();
      // std::cout << status.error_code() << ": " << status.error_message();
      // return {"RPC failed", RESPONSE_ERRCONN};
    }
  }

  ::iroha::Response* Torii(
      const std::unique_ptr<Transaction>& transaction) const {
    // Copy transaction to FlatBufferBuilder memory, then create
    // BufferRef<Transaction>
    // and share it to another sumeragi by using stub interface Torii.

    grpc::ClientContext context;
    flatbuffers::FlatBufferBuilder fbbTransaction;

    std::vector<uint8_t> hashes(*transaction->hash()->begin(),
                                *transaction->hash()->end());

    std::vector<uint8_t> signatures(transaction->signatures()->begin(),
                                    transaction->signatures()->end());

    // CreateSomething(), then .Union() -> Offset<void>

    {
      int length = 0;
      auto commandBuf = extractCommandBuffer(transaction, length);
      flatbuffers::Verifier verifier(commandBuf, length);
      ::iroha::VerifyCommand(verifier, (const void*)commandBuf.data(), transaction.command_type());

//      flatbuffers::GetRoot<>
    }

    std::vector<uint8_t> attachmentData(
        *transaction->attachment()->data()->begin(),
        *transaction->attachment()->data()->end());

    auto tx = ::iroha::CreateTransactionDirect(
        fbb, transaction->creatorPubKey()->c_str(), transaction->command_type(),
        //      reinterpret_cast<flatbuffers::Offset<void>*>(
        //          const_cast<void*>(transaction->command())),
        &tx_signatures, &_hash,
        ::iroha::CreateAttachmentDirect(
            fbb, transaction->attachment()->mime()->c_str(), &attachmentData));

    fbb.Finish(tx);
    auto req_transaction = flatbuffers::BufferRef<::iroha::Transaction>(
        fbb.GetBufferPointer(), fbb.GetSize());

    Status status = stub_->Torii(&context, req_transaction, &response);


    flatbuffers::BufferRef<Response> response;

    if (status.ok()) {
      logger::info("connection")
          << "response: " << response.GetRoot()->message();
      return response.GetRoot();
    } else {
      logger::error("connection")
          << status.error_code() << ": " << status.error_message();
      // std::cout << status.error_code() << ": " << status.error_message();
      return response.GetRoot();
    }
  }

 private:

  std::vector<uint8_t> extractCommandBuffer(const Transaction& transaction, std::size_t& length) {
    flatbuffers::FlatBufferBuilder fbbCommand;
    auto type = transaction.command_type();
    auto obj = transaction.command();
    auto commandOffset =
        flatbuffer_service::CreateCommandDirect(fbbCommand, obj, type);
    fbbCommand.Finish(commandOffset);

    auto buf = fbbCommand.GetBufferPointer();
    length = fbbCommand.GetSize();
    std::vector<uint8_t> ret;
    ret.assign(buf, buf + length);
    return ret;
  }


 private:
  std::unique_ptr<Sumeragi::Stub> stub_;
};

/************************************************************************************
 * Connection Service
 ************************************************************************************/
class SumeragiConnectionServiceImpl final : public ::iroha::Sumeragi::Service {
 public:
  Status Verify(ServerContext* context,
                const flatbuffers::BufferRef<ConsensusEvent>* request,
                flatbuffers::BufferRef<Response>* response) override {
    const ::iroha::ConsensusEvent* event = request->GetRoot();
    const std::string from = "from";
    iroha::SumeragiImpl::Verify::receiver.invoke(
        from, std::unique_ptr<::iroha::ConsensusEvent>(
                  const_cast<::iroha::ConsensusEvent*>(event)));
    return Status::OK;
  }

  Status Torii(ServerContext* context,
               const flatbuffers::BufferRef<Transaction>* transaction,
               flatbuffers::BufferRef<Response>* response) override {
    flatbuffers::FlatBufferBuilder fbb;
    auto res_offset =
        ::iroha::CreateResponseDirect(fbb, "OK!!", ::iroha::Code_COMMIT, 0);
    fbb.Finish(res_offset);
    // Since we keep reusing the same FlatBufferBuilder, the memory it owns
    // remains valid until the next call (this BufferRef doesn't own the
    // memory it points to).
    iroha::SumeragiImpl::Torii::receiver.invoke(
        "from", std::unique_ptr<::iroha::Transaction>(
                    const_cast<::iroha::Transaction*>(transaction->GetRoot())));

    *response = flatbuffers::BufferRef<::iroha::Response>(
        fbb.GetBufferPointer(), fbb.GetSize());
    logger::info("AA") << "Yurushite";
    return grpc::Status::OK;
  }
};

/************************************************************************************
 * Main connection
 ************************************************************************************/
std::mutex wait_for_server;
ServerBuilder builder;
grpc::Server* server = nullptr;
std::condition_variable server_cv;

void initialize_peer() {
  // ToDo catch exception of to_string


  logger::info("Connection GRPC") << " initialize_peer ";
}

int run() {
  logger::info("Connection GRPC") << " RUN ";
  auto address =
      "0.0.0.0:" +
      std::to_string(
          config::IrohaConfigManager::getInstance().getGrpcPortNumber(50051));
  SumeragiConnectionServiceImpl service;
  grpc::ServerBuilder builder;
  builder.AddListeningPort(address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  wait_for_server.lock();
  server = builder.BuildAndStart().release();
  wait_for_server.unlock();
  server_cv.notify_one();

  server->Wait();
  return 0;
}

void finish() {
  server->Shutdown();
  delete server;
}


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

}  // namespace connection
