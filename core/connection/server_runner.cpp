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

#ifndef CONNECTION_SERVER_RUNNER_HPP
#define CONNECTION_SERVER_RUNNER_HPP


#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <memory>

#include "server_runner.hpp"

namespace connection {

    ServerRunner::ServerRunner(const std::string& ip,
                               const std::vector<grpc::Service*>& services) {
        grpc::ServerBuilder builder;

        builder.AddListeningPort(ip, grpc::InsecureServerCredentials());
        for (auto s: services) {
            builder.RegisterService(s);
        }
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
        server->Wait();
    }

}  // namespace connection

#endif