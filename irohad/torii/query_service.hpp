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

#ifndef TORII_QUERY_SERVICE_HPP
#define TORII_QUERY_SERVICE_HPP

#include <endpoint.grpc.pb.h>
#include <endpoint.pb.h>
#include "model/converters/pb_query_factory.hpp"
#include "model/queries/responses/stateless_response.hpp"
#include "torii/processor/query_processor.hpp"

namespace iroha {
  namespace model {
    // class QueryStatelessResponse;
    namespace converters {
      class PbQueryFactory;
    }
  }
}

namespace torii {
  /**
   * Actual implementation of async QueryService.
   * ToriiServiceHandler::(SomeMethod)Handler calls a corresponding method in
   * this class.
   */
  class QueryService {
   public:
    QueryService(iroha::model::converters::PbQueryFactory& pb_factory,
                 iroha::torii::QueryProcessor& query_processor)
        : pb_factory_(pb_factory), query_processor_(query_processor){};
    /**
     * actual implementation of async Find in QueryService
     * @param request - Query
     * @param response - QueryResponse
     */
    void FindAsync(iroha::protocol::Query const& request,
                   iroha::protocol::QueryResponse& response) {
      auto query = pb_factory_.deserialize(request);
      query_processor_.query_handle(*query);
      query_processor_.query_notifier()
          .filter([](auto iroha_response) {
            return iroha:: instanceof
                <iroha::model::QueryStatelessResponse>(iroha_response);
          })
          .subscribe([&response](auto iroha_response) {
            auto ep = response.mutable_error_response();
            ep->set_reason("stateless validation failed");
          });
      //      response.set_code(iroha::protocol::ResponseCode::OK);
      //      response.set_message("Find async response");
    }

   private:
    iroha::model::converters::PbQueryFactory& pb_factory_;
    iroha::torii::QueryProcessor& query_processor_;
  };

}  // namespace torii

#endif  // TORII_QUERY_SERVICE_HPP
