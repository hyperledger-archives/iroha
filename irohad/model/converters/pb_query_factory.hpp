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
#ifndef IROHA_PB_QUERY_FACTORY_HPP
#define IROHA_PB_QUERY_FACTORY_HPP

#include "model/query.hpp"
#include "queries.pb.h"

namespace iroha {
  namespace model {
    namespace converters {

      /**
       * Converting business objects to protobuf and vice versa
       */
      class PbQueryFactory {
       public:

        /**
         * Convert proto query to model query
         * @param pb_block - reference to proto query
         * @return model Query
         */
        std::shared_ptr<model::Query> deserialize(protocol::Query &pb_query);


      };

    }  // namespace converters
  }    // namespace model
}  // namespace iroha

#endif  // IROHA_PB_QUERY_FACTORY_HPP
