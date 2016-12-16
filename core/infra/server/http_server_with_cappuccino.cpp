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

#include <json.hpp>

#include "../../server/http_server.hpp"
#include "../../vendor/Cappuccino/cappuccino.hpp"
#include "../../util/logger.hpp"
#include "../../service/peer_service.hpp"
#include "../../infra/protobuf/convertor.hpp"

#include "../../consensus/connection/connection.hpp"


namespace http {

    const auto assetName = "PointDemo";

    using nlohmann::json;
    using Request = Cappuccino::Request;
    using Response = Cappuccino::Response;


    using namespace transaction;
    using namespace command;
    using namespace event;
    using namespace object;

    json responseError(std::string message){
        return json({
                            {"message", message},
                            {"status", 400}
                    });
    }

    enum class RequestType{
        Int,
        Str,
        Bool,
        Float
    };

    void server() {
        logger::info("server", "initialize server!");
        Cappuccino::Cappuccino( 0, nullptr);

        Cappuccino::route<Cappuccino::Method::POST>( "/account/register",[](std::shared_ptr<Request> request) -> Response{
            auto res = Response(request);
            auto data = request->json();
            if(!data.empty()){
                try{
                    auto publicKey = data["publicKey"].get<std::string>();
                    auto alias     = data["alias"].get<std::string>();
                    auto timestamp = data["timestamp"].get<int>();

                    auto event = ConsensusEvent<Transaction<Add<object::Account>>>(
                        publicKey.c_str(),
                        publicKey.c_str(),
                        alias.c_str()
                    );
                    event.addTxSignature(
                        peer::getMyPublicKey(),
                        signature::sign(event.getHash(), peer::getMyPublicKey(), peer::getPrivateKey()).c_str()
                    );
                    connection::send(peer::getMyIp(), convertor::encode(event));

                }catch(...) {
                    res.json(json({
                      {"status",  400},
                      {"message", "Invalied json type or value"}
                    }));
                    return res;
                }
            }else{
                res.json(json({
                  {"status",  400},
                  {"message", "Invalied json"}
                }));
                return res;
            }
            res.json(json({
              {"status",  200},
              {"message", "successful"},
              {"uuid",    ""}
            }));

            return res;
        });

        Cappuccino::route<Cappuccino::Method::GET>( "/account",[](std::shared_ptr<Request> request) -> Response{
            std::string uuid = request->params("uuid");
            auto res = Response(request);

            auto rdata = "";//repository::account::find("uuid");
            auto data = json::parse(rdata);

            res.json(json({
                  {"status",  200},
                  {"alias", data["alias"]},
                  {"assets", data["assets"]}
            }));

            return res;
        });


        Cappuccino::route<Cappuccino::Method::POST>( "/asset/operation",[](std::shared_ptr<Request> request) -> Response{
            auto res = Response(request);
            auto data = request->json();
            if(!data.empty()){
                try{
                    auto assetUuid = data["asset-uuid"].get<std::string>();
                    auto timestamp = data["timestamp"].get<int>();
                    auto signature = data["signature"].get<std::string>();
                    auto command   = data["params"]["command"].get<std::string>();
                    auto value     = data["params"]["value"].get<std::string>();
                    auto sender    = data["params"]["sender"].get<std::string>();
                    auto receiver  = data["params"]["receiver"].get<std::string>();

                    auto event = ConsensusEvent<Transaction<Transfer<Asset>>>(
                            sender.c_str(),
                            sender.c_str(),
                            receiver.c_str(),
                            assetName,
                            std::atoi(value.c_str())
                    );
                    event.addTxSignature(
                            peer::getMyPublicKey(),
                            signature::sign(event.getHash(), peer::getMyPublicKey(), peer::getPrivateKey()).c_str()
                    );
                    connection::send(peer::getMyIp(), convertor::encode(event));


                }catch(...) {
                    res.json(json({
                      {"status",  400},
                      {"message", "Invalied json type or value"}
                    }));
                    return res;
                }
            }else{
                res.json(json({
                  {"status",  400},
                  {"message", "Invalied json"}
                }));
                return res;
            }

            res.json(json({
              {"status",  200},
              {"message", "Ok"}
            }));
            return res;
        });

        Cappuccino::route<Cappuccino::Method::GET>( "/history/transaction",[](std::shared_ptr<Request> request) -> Response{
            std::string uuid = request->params("uuid");
            auto res = Response(request);

            auto transaction_data = "";//repository::transaction::findAll();

            auto tx_json = json::array();
            for(auto tx: json::parse(transaction_data)){
                tx_json.push_back(tx);
            }

            res.json(json({
              {"status",  200},
              {"history", tx_json}
            }));
            return res;
        });

        logger::info("server", "start server!");
        // runnning
        Cappuccino::run();

    }
};  // namespace http
