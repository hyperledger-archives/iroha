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
#ifndef IROHA_HTTP_CLIENT_HPP_HPP
#define IROHA_HTTP_CLIENT_HPP_HPP

#include <json.hpp>

namespace http_client {
    using nlohmann::json;

    // WIP
    int GET(std::string dest, int port, std::string path);

    // WIP
    int POST(std::string dest, int port, std::string path);
    int POST(std::string dest, int port, std::string path, json data);

}

#endif //IROHA_HTTP_CLIENT_HPP_HPP
