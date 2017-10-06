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

#include "ametsuchi/impl/redis_flat_block_query.hpp"

namespace iroha {
  namespace ametsuchi {

    RedisFlatBlockQuery::RedisFlatBlockQuery(cpp_redis::redis_client &client,
                                             FlatFile &file_store)
        : FlatFileBlockQuery(file_store), client_(client) {}

    std::vector<uint64_t> RedisFlatBlockQuery::getBlockIds(
        const std::string &account_id) {
      std::vector<uint64_t> block_ids;
      client_.lrange(
          account_id, 0, -1, [this, &block_ids](cpp_redis::reply &reply) {
            for (const auto &block_reply : reply.as_array()) {
              const auto &string_reply = block_reply.as_string();

              // check if reply is an integer
              if (isdigit(string_reply.c_str()[0])) {
                block_ids.push_back(std::stoul(string_reply));
              }
            }
          });
      client_.sync_commit();
      return block_ids;
    }

    std::function<void(cpp_redis::reply &)>
    RedisFlatBlockQuery::callbackToLrange(
        const rxcpp::subscriber<model::Transaction> &s, uint64_t block_id) {
      return [this, &s, block_id](cpp_redis::reply &reply) {
        auto tx_ids_reply = reply.as_array();

        auto bytes = block_store_.get(block_id);
        auto document =
            model::converters::stringToJson(bytesToString(bytes.value()));
        if (not document.has_value()) {
          s.on_completed();
          return;
        }
        auto block = serializer_.deserialize(document.value());
        if (not block.has_value()) {
          s.on_completed();
          return;
        }

        for (const auto &tx_reply : tx_ids_reply) {
          auto tx_id = std::stoul(tx_reply.as_string());
          auto &&tx = block->transactions.at(tx_id);
          s.on_next(tx);
        }
        s.on_completed();
      };
    }

    rxcpp::observable<model::Transaction>
    RedisFlatBlockQuery::getAccountTransactions(std::string account_id) {
      return rxcpp::observable<>::create<model::Transaction>(
          [this, account_id](auto subscriber) {
            auto block_ids = this->getBlockIds(account_id);
            for (auto block_id : block_ids) {
              this->client_.lrange(
                  account_id + ":" + std::to_string(block_id),
                  0,
                  -1,
                  this->callbackToLrange(subscriber, block_id));
            }
            this->client_.sync_commit();
          });
    }

    rxcpp::observable<model::Transaction>
    RedisFlatBlockQuery::getAccountAssetTransactions(std::string account_id,
                                                     std::string asset_id) {
      return rxcpp::observable<>::create<model::Transaction>(
          [this, account_id, asset_id](auto subscriber) {
            auto block_ids = this->getBlockIds(account_id);
            for (auto block_id : block_ids) {
              // create key for querying redis
              std::string account_assets_key;
              account_assets_key.append(account_id);
              account_assets_key.append(":");
              account_assets_key.append(std::to_string(block_id));
              account_assets_key.append(":");
              account_assets_key.append(asset_id);
              client_.lrange(account_assets_key,
                             0,
                             -1,
                             this->callbackToLrange(subscriber, block_id));
            }
            client_.sync_commit();
          });
    }

  }  // namespace ametsuchi
}  // namespace iroha
