/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef INTEGRATION_FRAMEWORK_FAKE_PEER_LOADER_GRPC_HPP_
#define INTEGRATION_FRAMEWORK_FAKE_PEER_LOADER_GRPC_HPP_

#include "framework/integration_framework/fake_peer/types.hpp"
#include "loader.grpc.pb.h"
#include "logger/logger.hpp"

namespace integration_framework {
  namespace fake_peer {

    class LoaderGrpc : public iroha::network::proto::Loader::Service {
     public:
      explicit LoaderGrpc(const std::shared_ptr<FakePeer> &fake_peer);

      grpc::Status retrieveBlocks(
          ::grpc::ServerContext *context,
          const iroha::network::proto::BlocksRequest *request,
          ::grpc::ServerWriter<iroha::protocol::Block> *writer) override;

      grpc::Status retrieveBlock(
          ::grpc::ServerContext *context,
          const iroha::network::proto::BlockRequest *request,
          iroha::protocol::Block *response) override;

      bool sendBlockRequest(const std::string &dest_address,
                            const LoaderBlockRequest &request);

      size_t sendBlocksRequest(const std::string &dest_address,
                               const LoaderBlocksRequest &request);

     private:
      std::weak_ptr<FakePeer> fake_peer_wptr_;
      logger::Logger log_;
    };
  }
}

#endif /* INTEGRATION_FRAMEWORK_FAKE_PEER_LOADER_GRPC_HPP_ */
