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
#include <crypto/hash.hpp>

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

    boost::optional<uint64_t> RedisFlatBlockQuery::getBlockId(
        const std::string &hash) {
      boost::optional<uint64_t> blockId;
      client_.get(hash, [this, &blockId](cpp_redis::reply &reply) {
        if (reply.is_null()) {
          blockId = boost::none;
        } else {
          blockId = std::stoul(reply.as_string());
        }
      });
      client_.sync_commit();

      return blockId;
    }

    std::function<void(cpp_redis::reply &)>
    RedisFlatBlockQuery::callbackToLrange(
        const rxcpp::subscriber<model::Transaction> &s, uint64_t block_id) {
      return [this, &s, block_id](cpp_redis::reply &reply) {
        auto tx_ids_reply = reply.as_array();

        block_store_.get(block_id) | [](auto bytes) {
          return model::converters::stringToJson(bytesToString(bytes));
        } | [this](const auto &json) {
          return serializer_.deserialize(json);
        } | [&](const auto &block) {
          for (const auto &tx_reply : tx_ids_reply) {
            auto tx_id = std::stoul(tx_reply.as_string());
            auto &&tx = block.transactions.at(tx_id);
            s.on_next(tx);
          }
        };
      };
    }

    rxcpp::observable<model::Transaction>
    RedisFlatBlockQuery::getAccountTransactions(std::string account_id) {
      return rxcpp::observable<>::create<model::Transaction>(
          [this, account_id](auto subscriber) {
            auto block_ids = this->getBlockIds(account_id);
            if (block_ids.empty()) {
              subscriber.on_completed();
              return;
            }

            for (auto block_id : block_ids) {
              client_.lrange(account_id + ":" + std::to_string(block_id),
                             0,
                             -1,
                             this->callbackToLrange(subscriber, block_id));
            }
            client_.sync_commit();
            subscriber.on_completed();
          });
    }

    rxcpp::observable<model::Transaction>
    RedisFlatBlockQuery::getAccountAssetTransactions(std::string account_id,
                                                     std::string asset_id) {
      return rxcpp::observable<>::create<model::Transaction>(
          [this, account_id, asset_id](auto subscriber) {
            auto block_ids = this->getBlockIds(account_id);
            if (block_ids.empty()) {
              subscriber.on_completed();
              return;
            }

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
            subscriber.on_completed();
          });
    }

    rxcpp::observable<model::Transaction> RedisFlatBlockQuery::getTxByHash(
        std::string hash) {
      return rxcpp::observable<>::create<model::Transaction>(
          [this, hash](auto subscriber) {
            auto blockId = this->getBlockId(hash);
            if (not blockId) {
              subscriber.on_completed();
              return;
            }

            block_store_.get(blockId.value()) | [](auto bytes) {
              return model::converters::stringToJson(bytesToString(bytes));
            } | [this](const auto &json) {
              return serializer_.deserialize(json);
            } | [&](const auto &block) {
              for (auto &tx : block.transactions) {
                if (iroha::hash(tx).to_hexstring() == hash) {
                  subscriber.on_next(tx);
                }
              }
            };
            subscriber.on_completed();
          });
    }

    boost::optional<model::Transaction> RedisFlatBlockQuery::getTxByHashSync(
        std::string hash) {
      auto blockId = this->getBlockId(hash);
      if (not blockId) {
        return boost::none;
      }
      return block_store_.get(blockId.value()) |
          [](auto bytes) {
            return model::converters::stringToJson(bytesToString(bytes));
          }
      | [this](const auto &json) { return serializer_.deserialize(json); } |
          [&](const auto &block) {
            auto it =
                std::find_if(block.transactions.begin(),
                             block.transactions.end(),
                             [&hash](auto tx) {
                               return iroha::hash(tx).to_hexstring() == hash;
                             });
            return (it == block.transactions.end())
                ? boost::none
                : boost::make_optional<model::Transaction>(*it);
          };
    }

  }  // namespace ametsuchi
}  // namespace iroha
