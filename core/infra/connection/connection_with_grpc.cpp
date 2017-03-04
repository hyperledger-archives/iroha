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

#include <consensus/connection/connection.hpp>
#include <util/logger.hpp>
#include <service/peer_service.hpp>

#include <infra/config/peer_service_with_json.hpp>
#include <infra/config/iroha_config_with_json.hpp>

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

using grpc::Channel;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ClientContext;
using grpc::Status;

namespace connection {

    using Api::Sumeragi;
    using Api::TransactionRepository;
    using Api::AssetRepository;


    using Api::Query;
    using Api::ConsensusEvent;
    using Api::StatusResponse;
    using Api::Transaction;
    using Api::TransactionResponse;
    using Api::AssetResponse;

    namespace iroha {
        namespace Sumeragi {
            namespace Verify {
                std::vector<std::string> receiver_ips;
                std::vector<
                        std::function<void(
                                const std::string& from,
                                ConsensusEvent& message)
                        >
                > receivers;
            };
            namespace Torii {
                std::vector<
                        std::function<void(
                                const std::string& from,
                                Transaction& message
                        )>
                > receivers;
            }
        };
        namespace TransactionRepository {
            namespace find {
                std::vector<
                        std::function<void(
                                const std::string& from,
                                Query& message
                        )>
                > receivers;
            };
        }
        namespace AssetRepository {
            namespace find {
                std::vector<
                        std::function<void(
                                const std::string &from,
                                Query &message
                        )>
                > receivers;
            }
        }
    };

    class SumeragiConnectionClient {
    public:
        explicit SumeragiConnectionClient(std::shared_ptr<Channel> channel)
                : stub_(Sumeragi::NewStub(channel)) {}

        std::string Verify(const ConsensusEvent& consensusEvent) {
            StatusResponse response;
            logger::info("connection")  <<  "Operation";
            logger::info("connection")  <<  "size: "    <<  consensusEvent.eventsignatures_size();
            logger::info("connection")  <<  "name: "    <<  consensusEvent.transaction().asset().name();

            ClientContext context;

            Status status = stub_->Verify(&context, consensusEvent, &response);

            if (status.ok()) {
                logger::info("connection")  << "response: " << response.value();
                return response.value();
            } else {
                logger::error("connection") << status.error_code() << ": " << status.error_message();
                //std::cout << status.error_code() << ": " << status.error_message();
                return "RPC failed";
            }
        }

        std::string Torii(const Transaction& transaction) {
            StatusResponse response;

            ClientContext context;

            Status status = stub_->Torii(&context, transaction, &response);

            if (status.ok()) {
                logger::info("connection")  << "response: " << response.value();
                return response.value();
            } else {
                logger::error("connection") << status.error_code() << ": " << status.error_message();
                //std::cout << status.error_code() << ": " << status.error_message();
                return "RPC failed";
            }
        }

    private:
        std::unique_ptr<Sumeragi::Stub> stub_;
    };


    class SumeragiConnectionServiceImpl final : public Sumeragi::Service {
    public:

        Status Verify(
                ServerContext*          context,
                const ConsensusEvent*   pevent,
                StatusResponse*         response
        ) override {
            ConsensusEvent event;
            event.CopyFrom(pevent->default_instance());
            event.mutable_eventsignatures()->CopyFrom(pevent->eventsignatures());
            event.mutable_transaction()->CopyFrom(pevent->transaction());
            event.set_status(pevent->status());
            logger::info("connection") << "size: " << event.eventsignatures_size();
            auto dummy = "";
            for (auto& f: iroha::Sumeragi::Verify::receivers){
                f(dummy, event);
            }
            response->set_value("OK");
            return Status::OK;
        }

        Status Torii(
                ServerContext*      context,
                const Transaction*  transacion,
                StatusResponse*     response
        ) override {
            auto dummy = "";
            Transaction tx;
            tx.CopyFrom(*transacion);
            for (auto& f: iroha::Sumeragi::Torii::receivers){
                f(dummy, tx);
            }
            response->set_value("OK");
            return Status::OK;
        }

    };

    class TransactionRepositoryServiceImpl final : public TransactionRepository::Service {
      public:

        Status find(
            ServerContext*          context,
            const Query*              query,
            TransactionResponse*   response
        ) override {
            Query q;
            q.CopyFrom(*query);
            auto dummy = "";
            for (auto& f: iroha::TransactionRepository::find::receivers){
                f(dummy, q);
            }
            response->set_message("OK");
            return Status::OK;
        }
    };

    class AssetRepositoryServiceImpl final : public AssetRepository::Service {
    public:

        Status find(
            ServerContext*          context,
            const Query*              query,
            AssetResponse*         response
        ) override {
            auto dummy = "";
            Query q;
            q.CopyFrom(*query);
            for (auto& f: iroha::AssetRepository::find::receivers){
                f(dummy, q);
            }
            response->set_message("OK");
            return Status::OK;
        }
    };

    namespace iroha {

        namespace Sumeragi {

            SumeragiConnectionServiceImpl service;

            namespace Verify {

                void addSubscriber(std::string ip) {
                    receiver_ips.push_back(ip);
                }

                bool receive(
                    const std::function<void(
                        const std::string&,
                        ConsensusEvent&)>& callback
                ) {
                    receivers.push_back(callback);
                    return true;
                }

                bool send(
                    const std::string &ip,
                    const ConsensusEvent &event
                ) {
                    if (find(receiver_ips.begin(), receiver_ips.end(), ip) != receiver_ips.end()) {
                        SumeragiConnectionClient client(
                            grpc::CreateChannel(
                                ip + ":" + std::to_string(config::IrohaConfigManager::getInstance().getGrpcPortNumber(50051)),
                                grpc::InsecureChannelCredentials()
                            )
                        );
                        std::string reply = client.Verify(event);
                        return true;
                    } else {
                        return false;
                    }
                }

                bool sendAll(
                    const ConsensusEvent &event
                ) {
                    for (auto &ip : receiver_ips) {
                        if (ip != config::PeerServiceConfig::getInstance().getMyIp()) {
                            send(ip, event);
                        }
                    }
                    return true;
                }

            }

            namespace Torii{

                bool receive(
                    const std::function<void(
                    const std::string &,
                    Transaction&)
                > &callback){
                    receivers.push_back(callback);
                    return true;
                }

            }

        }

        namespace TransactionRepository {
            namespace find {
                TransactionRepositoryServiceImpl service;

                bool receive(
                        const std::function<void(
                                const std::string &,
                                Query &)> &callback
                ) {
                    receivers.push_back(callback);
                    return true;
                }
            };
        };

        namespace AssetRepository {
            namespace find {
                AssetRepositoryServiceImpl service;

                bool receive(
                        const std::function<void(
                                const std::string &,
                                Query &)> &callback
                ) {
                    receivers.push_back(callback);
                    return true;
                }
            };
        }

    };

    ServerBuilder builder;


    void initialize_peer() {
        std::string server_address("0.0.0.0:" + std::to_string(config::IrohaConfigManager::getInstance().getGrpcPortNumber(50051)));
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&iroha::Sumeragi::service);
        builder.RegisterService(&iroha::TransactionRepository::find::service);
        builder.RegisterService(&iroha::AssetRepository::find::service);
    }

    int run() {
        std::unique_ptr<Server> server(builder.BuildAndStart());
        server->Wait();
        return 0;
    }
    void finish(){
        builder = ServerBuilder();
    }

};
