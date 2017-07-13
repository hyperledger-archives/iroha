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

namespace torii {

  iroha::protocol::ToriiResponse sendTransaction(
      const iroha::protocol::Transaction& tx,
      const std::string& targetPeerIp,
      int targetPeerPort);

}  // namespace torii

#endif  // TORII_COMMAND_CLIENT_HPP
