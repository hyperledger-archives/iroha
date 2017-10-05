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

#ifndef IROHA_REDIS_FLAT_BLOCK_QUERY_HPP
#define IROHA_REDIS_FLAT_BLOCK_QUERY_HPP

#include <ametsuchi/impl/flat_file/flat_file.hpp>
#include <cpp_redis/redis_client.hpp>
//#include "ametsuchi/block_query.hpp"
#include "ametsuchi/impl/flat_file_block_query.hpp"

#include "model/converters/json_block_factory.hpp"

namespace iroha {
  namespace ametsuchi {
    class RedisFlatBlockFile : public FlatFileBlockQuery {
     public:
      RedisFlatBlockFile(std::string redis_host,
                         size_t redis_port,
                         FlatFile& file_store);

      RedisFlatBlockFile(FlatFile& block_store) = delete;

      ~RedisFlatBlockFile() override;

      rxcpp::observable<model::Transaction> getAccountTransactions(
          std::string account_id) override;

      rxcpp::observable<model::Transaction> getAccountAssetTransactions(
          std::string account_id, std::string asset_id) override;

     private:
      /**
       * Returns all blocks' ids containing given account id
       * @param account_id
       * @return vector of block ids
       */
      std::vector<uint64_t> getBlockIdsByAccountId(std::string& account_id);

      /**
       * creates callback to lrange query to redis to supply result to
       * subscriber s
       * @param s
       * @param block_id
       * @return
       */
      std::function<void(cpp_redis::reply&)> callbackToLrange(
          const rxcpp::subscriber<model::Transaction>& s, uint64_t block_id);

      cpp_redis::redis_client client_;
    };
  }  // namespace ametsuchi
}  // namespace iroha

#endif  // IROHA_REDIS_FLAT_BLOCK_QUERY_HPP
