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
  void invoke(const std::string& from, flatbuffers::unique_ptr_t&& argv) {
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

bool send(const std::string& ip, const ::iroha::ConsensusEvent& event) {
  // ToDo: Extract transaction from consensus event.
  logger::info("Connection with grpc") << "Send!";
  if (config::PeerServiceConfig::getInstance().isExistIP(ip)) {
    logger::info("Connection with grpc") << "isExistIP " << ip;
    auto channel =
        grpc::CreateChannel(ip + ":50051", grpc::InsecureChannelCredentials());
    auto stub = ::iroha::Sumeragi::NewStub(channel);

    grpc::ClientContext context;

    auto publicKey = "SamplePublicKey";

    // Build a request with the name set.
    flatbuffers::FlatBufferBuilder fbbTransaction;

    // TODO: valid command
    // Tempolary implementation. Currently, use CreaeteAccount command.
    auto command = [&] {
      std::vector<std::string> signatories = {
        "publicKey1"
      };

      auto accountBuf = flatbuffer_service::CreateAccountBuffer(
          publicKey, "alias", signatories, 1);

      return ::iroha::CreateAccountAddDirect(fbbTransaction, &accountBuf);
    }();

    // TODO: Tempolary implementation. Use 'sign' function
    std::vector<uint8_t> signature;
    for (auto e : std::string("hash + timestamp + pubkey ?")) {
      signature.push_back(e);
    }

    std::vector<flatbuffers::Offset<::iroha::Signature>> signatures;

    signatures.push_back(::iroha::CreateSignatureDirect(
        fbbTransaction, publicKey, &signature,
        1234567  // TODO: timestamp
        ));

      std::vector<flatbuffers::Offset<::iroha::Transaction>> transactionOffsetVec;
      std::vector<flatbuffers::Offset<::iroha::Signature>>   peerSignatureOffsetVec;
      transactionOffsetVec.emplace_back(::iroha::CreateTransactionDirect(
        fbbTransaction, publicKey, ::iroha::Command::Command_AccountAdd,
        command.Union(), &signatures, nullptr,
        ::iroha::CreateAttachmentDirect(fbbTransaction, nullptr, nullptr))
      );
      for(const auto& aPeerSig: *event.peerSignatures()) {
          std::vector<uint8_t> aPeerSigBlob(
                  aPeerSig->signature()->begin(),
                  aPeerSig->signature()->end()
          );
          peerSignatureOffsetVec.push_back(
              ::iroha::CreateSignatureDirect(
                  fbbTransaction,
                  aPeerSig->publicKey()->c_str(),
                  &aPeerSigBlob,
                  1234567
              )
          );
      }


    auto consensusEventOffset = ::iroha::CreateConsensusEvent(
        fbbTransaction, fbbTransaction.CreateVector(peerSignatureOffsetVec), fbbTransaction.CreateVector(transactionOffsetVec)
    );
    fbbTransaction.Finish(consensusEventOffset);

    auto eventRef = flatbuffers::BufferRef<::iroha::ConsensusEvent>(
        fbbTransaction.GetBufferPointer(), fbbTransaction.GetSize());

    flatbuffers::BufferRef<::iroha::Response> responseRef;

    logger::info("Connection with grpc") << "isExistIP " << ip;
    // The actual RPC.
    auto status = stub->Verify(&context, eventRef, &responseRef);

    if (status.ok()) {
      auto msg = responseRef.GetRoot()->message();
      std::cout << "RPC response: " << msg->str() << std::endl;
    } else {
      std::cout << "RPC failed" << std::endl;
    }
    return true;
  }
  logger::info("Connection with grpc") << "is not ExistIP__" << ip;
  return false;
}

bool sendAll(const ::iroha::ConsensusEvent& event) {
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
      const ::iroha::ConsensusEvent& consensusEvent) const {
    grpc::ClientContext context;
    flatbuffers::BufferRef<Response> responseRef;
    logger::info("connection") << "Operation";
    logger::info("connection")
        << "size: " << consensusEvent.peerSignatures()->size();
    logger::info("connection")
        << "CommandType: "
        << consensusEvent.transactions()->Get(0)->command_type();

    // For now, ConsensusEvent has one transaction.
    const auto& transaction = *consensusEvent.transactions()->Get(0);

    auto newConsensusEvent = flatbuffer_service::toConsensusEvent(transaction);

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

  ::iroha::Response* Torii(const ::iroha::Transaction& transaction) const {
    // Copy transaction to FlatBufferBuilder memory, then create
    // BufferRef<Transaction>
    // and share it to another sumeragi by using stub interface Torii.

    ::grpc::ClientContext clientContext;
    flatbuffers::FlatBufferBuilder fbbTransaction;

    std::vector<uint8_t> hashBlob(transaction.hash()->begin(),
                                  transaction.hash()->end());

    std::vector<flatbuffers::Offset<::iroha::Signature>> signatures;
    for (auto&& txSig : *transaction.signatures()) {
      std::vector<uint8_t> data(*txSig->signature()->begin(),
                                *txSig->signature()->end());
      signatures.emplace_back(::iroha::CreateSignatureDirect(
          fbbTransaction, txSig->publicKey()->c_str(), &data));
    }

    // FIXED: Creation of command offset. reinterpret_cast<> ->
    // flatbuffer_service::Create...()
    auto commandOffset = [&] {
      std::size_t length = 0;
      auto commandBuf = extractCommandBuffer(transaction, length);
      flatbuffers::Verifier verifier(commandBuf.get(), length);
      ::iroha::VerifyCommand(verifier, commandBuf.get(),
                             transaction.command_type());
      return flatbuffer_service::CreateCommandDirect(
          fbbTransaction, commandBuf.get(), transaction.command_type());
    }();

    std::vector<uint8_t> attachmentData(
        *transaction.attachment()->data()->begin(),
        *transaction.attachment()->data()->end());

    auto transactionOffset = ::iroha::CreateTransactionDirect(
        fbbTransaction, transaction.creatorPubKey()->c_str(),
        transaction.command_type(), commandOffset, &signatures, &hashBlob,
        ::iroha::CreateAttachmentDirect(
            fbbTransaction, transaction.attachment()->mime()->c_str(),
            &attachmentData));

    fbbTransaction.Finish(transactionOffset);

    flatbuffers::BufferRef<::iroha::Transaction> reqTransactionRef(
        fbbTransaction.GetBufferPointer(), fbbTransaction.GetSize());

    flatbuffers::BufferRef<Response> responseRef;

    ::grpc::Status status =
        stub_->Torii(&clientContext, reqTransactionRef, &responseRef);

    if (status.ok()) {
      logger::info("connection")
          << "response: " << responseRef.GetRoot()->message();
      return responseRef.GetRoot();
    } else {
      logger::error("connection")
          << status.error_code() << ": " << status.error_message();
      // std::cout << status.error_code() << ": " << status.error_message();
      return responseRef.GetRoot();
    }
  }

 private:
  flatbuffers::unique_ptr_t extractCommandBuffer(
      const ::iroha::Transaction& transaction, std::size_t& length) {
    flatbuffers::FlatBufferBuilder fbbCommand;
    auto type = transaction.command_type();
    auto obj = transaction.command();
    auto commandOffset =
        flatbuffer_service::CreateCommandDirect(fbbCommand, obj, type);
    fbbCommand.Finish(commandOffset);
    length = fbbCommand.GetSize();
    return fbbCommand.ReleaseBufferPointer();
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
    // バッファの一部分を参照している。これをreceiverにmoveした場合、バッファの実態の解放時にLeakする
    //    const ::iroha::ConsensusEvent* event = request->GetRoot();

    assert(request->GetRoot()->peerSignatures() != nullptr);

    flatbuffers::FlatBufferBuilder fbbConsensusEvent;

    std::vector<flatbuffers::Offset<::iroha::Signature>> peerSignatures;
    for(const auto& aPeerSig: *request->GetRoot()->peerSignatures()) {
      std::vector<uint8_t> aPeerSigBlob(
          aPeerSig->signature()->begin(),
          aPeerSig->signature()->end()
      );
      peerSignatures.push_back(
          ::iroha::CreateSignatureDirect(
              fbbConsensusEvent,
              aPeerSig->publicKey()->c_str(),
              &aPeerSigBlob,
              1234567
          )
      );
    }


    auto tx = request->GetRoot()->transactions()->Get(0);
      std::vector<flatbuffers::Offset<::iroha::Signature>> signatures;

    if(tx->signatures() != nullptr){
      std::vector<uint8_t> signatureBlob(
          tx->signatures()->Get(0)->signature()->begin(),
          tx->signatures()->Get(0)->signature()->end()
      );
      signatures.push_back(
          ::iroha::CreateSignatureDirect(
              fbbConsensusEvent,
              tx->signatures()->Get(0)->publicKey()->c_str(),
              &signatureBlob,
              1234567
          )
      );
    }
    std::vector<uint8_t> hashes;
    if(tx->hash() != nullptr){
      hashes.assign(
         tx->hash()->begin(),
         tx->hash()->end()
      );
    }

    flatbuffers::Offset<::iroha::Attachment> attachmentOffset;
    std::vector<uint8_t> data;
    if(tx->attachment() != nullptr &&
       tx->attachment()->data() != nullptr &&
       tx->attachment()->mime() != nullptr
    ){
      data.assign(
              tx->attachment()->data()->begin(),
              tx->attachment()->data()->end()
      );

      attachmentOffset = ::iroha::CreateAttachmentDirect(
              fbbConsensusEvent,
              tx->attachment()->mime()->c_str(),
              &data
      );
    }

    std::vector<flatbuffers::Offset<::iroha::Transaction>> transactions;

    // TODO: Currently, #(transaction) is one.
    transactions.push_back(
        ::iroha::CreateTransactionDirect(
            fbbConsensusEvent,
            tx->creatorPubKey()->c_str(),
            tx->command_type(),
            flatbuffer_service::CreateCommandDirect(
                    fbbConsensusEvent,
                    tx->command(),
                    tx->command_type()
            ),
            &signatures,
            &hashes,
            attachmentOffset
        )
    );

    fbbConsensusEvent.Finish(::iroha::CreateConsensusEventDirect(
        fbbConsensusEvent, &peerSignatures, &transactions
    ));

    const std::string from = "from";
    iroha::SumeragiImpl::Verify::receiver.invoke(
        from, fbbConsensusEvent.ReleaseBufferPointer()
    );

      fbbResponse.Clear();

      auto responseOffset = ::iroha::CreateResponseDirect(
              fbbResponse, "OK!!", ::iroha::Code_COMMIT, 0);
      fbbResponse.Finish(responseOffset);

      *response = flatbuffers::BufferRef<::iroha::Response>(
          fbbResponse.GetBufferPointer(), fbbResponse.GetSize());

      return Status::OK;
  }

  Status Torii(ServerContext* context,
               const flatbuffers::BufferRef<Transaction>* transactionRef,
               flatbuffers::BufferRef<Response>* responseRef) override {
    fbbResponse.Clear();

    auto responseOffset = ::iroha::CreateResponseDirect(
        fbbResponse, "OK!!", ::iroha::Code_COMMIT, 0);
    fbbResponse.Finish(responseOffset);

    logger::debug("SumeragiConnectionServiceImpl::Torii") << "RPC works";

    /*
      // This code leaks.
      // Maybe, buffer of caller deallocate Transaction and std::unique_ptr
      movement occurs.
      // So, double free happens.
        iroha::SumeragiImpl::Torii::receiver.invoke(
            "from",
            std::unique_ptr<::iroha::Transaction>(
                // FIX: 呼び出し元のバッファを即座に破棄するのなら安全なはず
                const_cast<::iroha::Transaction*>(transactionRef->GetRoot())));
        *responseRef = flatbuffers::BufferRef<::iroha::Response>(
            fbbResponse.GetBufferPointer(), fbbResponse.GetSize());
    */

    {
      const auto transaction = transactionRef->GetRoot();

      flatbuffers::FlatBufferBuilder fbbTransaction;

      std::vector<flatbuffers::Offset<::iroha::Signature>> tx_signatures;

      if (transaction->signatures() != nullptr) {
        for (auto&& txSig : *transaction->signatures()) {
          std::vector<uint8_t> _data;
          if (txSig->signature() != nullptr) {
            for (auto d : *txSig->signature()) {
              _data.emplace_back(d);
            }
            tx_signatures.emplace_back(::iroha::CreateSignatureDirect(
                fbbTransaction, txSig->publicKey()->c_str(), &_data));
          }
        }
      }

      std::vector<uint8_t> _hash;
      if(transaction->hash() != nullptr) {
          _hash.assign(
              transaction->hash()->begin(),
              transaction->hash()->end()
          );
      }


      std::vector<uint8_t> attachmentData;

      if(transaction->attachment() != nullptr && transaction->attachment()->data() != nullptr){
        attachmentData.assign(
          transaction->attachment()->data()->begin(),
          transaction->attachment()->data()->end()
        );
      }

      fbbTransaction.Finish(::iroha::CreateTransactionDirect(
          fbbTransaction, transaction->creatorPubKey()->c_str(),
          transaction->command_type(),
          flatbuffer_service::CreateCommandDirect(fbbTransaction,
                                                  transaction->command(),
                                                  transaction->command_type()),
          &tx_signatures, &_hash,
          ::iroha::CreateAttachmentDirect(
              fbbTransaction, transaction->attachment()->mime()->c_str(),
              &attachmentData)));

      // Leak doesn't occur till here.

      /*
          // Leak occurs.
            auto flatbuf = fbbTransaction.ReleaseBufferPointer();

            iroha::SumeragiImpl::Torii::receiver.invoke(
                "from",  // TODO: Specify 'from'
                std::unique_ptr<::iroha::Transaction>(
                    flatbuffers::GetMutableRoot<::iroha::Transaction>(
                        flatbuf.get())));
                        */
      //      auto flatbuf = fbbTransaction.ReleaseBufferPointer(); // Leak
      //      doesn't occur.
      /*
            std::unique_ptr<::iroha::Transaction> uptr(
                flatbuffers::GetMutableRoot<::iroha::Transaction>(bufptr.get()));
      */
      iroha::SumeragiImpl::Torii::receiver.invoke(
          "from",  // TODO: Specify 'from'
          fbbTransaction.ReleaseBufferPointer());
    }

    // This reference remains until next calling
    // SumeragiConnectionServiceImpl::Torii() method.
    *responseRef = flatbuffers::BufferRef<::iroha::Response>(
        fbbResponse.GetBufferPointer(), fbbResponse.GetSize());

    return grpc::Status::OK;
  }

 private:
  flatbuffers::FlatBufferBuilder fbbResponse;
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
