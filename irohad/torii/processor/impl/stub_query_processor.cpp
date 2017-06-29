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


#include <torii/processor/stub_query_processor.hpp>

namespace iroha {
  namespace torii {
    using rxcpp::observable;
    using rxcpp::subscriber;
    using std::shared_ptr;
    using dao::Query;
    using dao::QueryResponse;
    using dao::Client;

    void QueryProcessorStub::handle(Client client, Query query) {
      return;
    }

    observable<shared_ptr<QueryResponse>> QueryProcessorStub::notifier() {
      return observable<>::create<shared_ptr<QueryResponse>> (
          [](rxcpp::subscriber<shared_ptr<QueryResponse>> s) {
            s.on_completed();
          });
    }
  } //namespace torii
} //namespace iroha
