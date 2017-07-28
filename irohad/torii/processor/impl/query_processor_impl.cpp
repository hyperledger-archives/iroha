/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "torii/processor/query_processor_impl.hpp"
#include "model/queries/responses/error_response.hpp"

namespace iroha {
  namespace torii {

    QueryProcessorImpl::QueryProcessorImpl(
        model::QueryProcessingFactory &qpf,
        validation::StatelessValidator &stateless_validator)
        : qpf_(qpf), validator_(stateless_validator) {}

    void QueryProcessorImpl::queryHandle(std::shared_ptr<model::Query> query) {
      model::ErrorResponse response;
      response.query = *query;
      response.reason = "Not valid";

      // if not valid send wrong response
      if (!validator_.validate(*query)) {
        subject_.get_subscriber().on_next(
            std::make_shared<model::ErrorResponse>(response));
      } else {  // else execute query
        auto qpf_response = qpf_.execute(*query);
        subject_.get_subscriber().on_next(qpf_response);
      }
    }

    rxcpp::observable<std::shared_ptr<model::QueryResponse>>
    QueryProcessorImpl::queryNotifier() {
      return subject_.get_observable();
    }
  }
}