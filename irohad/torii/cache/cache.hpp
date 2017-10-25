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

#ifndef IROHA_CACHE_HPP
#define IROHA_CACHE_HPP

#include <endpoint.pb.h>
#include <boost/optional.hpp>
#include <list>
#include <mutex>
#include <string>

namespace torii {
  namespace cache {
    /**
     * Cache for torii responses.
     * Internally it uses uses a map to cache tx statuses and linked list
     * based index to remove oldest items when getIndexSizeHigh() is reached.
     */
    class ToriiResponseCache {
     public:
      /**
       * When amount of cache records reaches getIndexSizeHigh() a clean
       * procedure is started until getIndexSizeLow() records left.
       * Mostly done for tests but feel free to use anywhere if you need.
       */
      uint32_t getIndexSizeHigh() const;
      uint32_t getIndexSizeLow() const;

      uint64_t getCacheItemCount() const;

      /**
       * Adds new item to cache. Note: cache doe not have a remove method,
       * deletion performs automatically.
       * Since every add operation can potentially lead to deletion,
       * it should be protected by mutex.
       * @param response - response status of transaction
       * @param hash - hash of transaction
       */
      void addItem(const iroha::protocol::ToriiResponse &response,
                   const std::string &hash);

      /**
       * Performs a search for an item with a specific hash.
       * @param hash - hash to find
       * @return Optional of ToriiResponse
       */
      boost::optional<iroha::protocol::ToriiResponse> findItem(
          const std::string &hash);

     private:
      std::unordered_map<std::string, iroha::protocol::ToriiResponse>
          handler_map_;
      std::list<std::string> handler_map_index_;
      std::mutex handler_map_mutex_;

      /**
     * Protection from handler map overflow.
     * Values are quite random and should be tuned for better performance
     * and may be even move to config.
     */
      const uint32_t MAX_HANDLER_MAP_SIZE_HIGH = 20000;
      const uint32_t MAX_HANDLER_MAP_SIZE_LOW = 10000;
    };
  }
}

#endif  // IROHA_CACHE_HPP
