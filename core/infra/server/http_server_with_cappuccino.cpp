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

#include <server/http_server.hpp>
#include <vendor/Cappuccino/cappuccino.hpp>
#include <util/logger.hpp>
#include <service/peer_service.hpp>
#include <infra/config/peer_service_with_json.hpp>

#include <consensus/connection/connection.hpp>

#include <repository/consensus/transaction_repository.hpp>


namespace http {

    const auto assetName = "PointDemo";

    using nlohmann::json;
    using Request = Cappuccino::Request;
    using Response = Cappuccino::Response;


    json responseError(std::string message){
        return json({
                            {"message", std::move(message)},
                            {"status", 400}
                    });
    }

    enum class RequestType{
        Int,
        Str,
        Bool,
        Float
    };

    std::vector<std::string> split(const std::string& str, const std::string& delim) noexcept{
        std::vector<std::string> result;
        std::string::size_type pos = 0;
        while(pos != std::string::npos) {
            auto p = str.find(delim, pos);
            if(p == std::string::npos){
                result.push_back(str.substr(pos));
                break;
            }else{
                result.push_back(str.substr(pos, p - pos));
            }
            pos = p + delim.size();
        }
        return result;
    }

    void server() {
        logger::info("server") << "initialize server!";
        Cappuccino::Cappuccino( 0, nullptr);

        Cappuccino::route<Cappuccino::Method::POST>("/account/register", [](std::shared_ptr<Request> request) -> Response {
            auto res = Response(request);
            auto data = request->json();
            std::string uuid;

            res.json(json({
              {"status",  200},
              {"message", "successful"},
              {"uuid",   uuid}
            }));

            return res;
        });

        Cappuccino::route<Cappuccino::Method::GET>( "/account",[](std::shared_ptr<Request> request) -> Response{
            std::string uuid = request->params("uuid");
            auto res = Response(request);

            res.json(json({
                  {"status",  200}
            }));

            return res;
        });

        Cappuccino::route<Cappuccino::Method::POST>( "/asset/operation",[](std::shared_ptr<Request> request) -> Response{
            auto res = Response(request);
            auto data = request->json();
            if(!data.empty()) {

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
            auto tx_json = json::array();

            res.json(json({
              {"status",  200},
              {"history", tx_json}
            }));
            return res;
        });

        logger::info("server") << "start server!";
        // runnning
        Cappuccino::run();

    }
};  // namespace http
