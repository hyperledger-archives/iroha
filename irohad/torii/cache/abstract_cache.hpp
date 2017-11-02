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

#ifndef IROHA_ABSTRACT_CACHE_HPP
#define IROHA_ABSTRACT_CACHE_HPP

#include <endpoint.pb.h>
#include <boost/optional.hpp>
#include <list>
#include <mutex>
#include <string>

namespace iroha {
  namespace cache {
    /**
     * Cache for any key-value types.
     * Internally it uses a map to cache ItemTypes and linked list
     * based index of KeyTypes to remove oldest items when getIndexSizeHigh() is
     * reached. Implemented as a CRTP pattern.
     * @tparam KeyType - type of cache keys
     * @tparam ValueType - type of cache values
     * @tparam T - need for CRTP
     */

    template <typename KeyType, typename ValueType, typename T>
    class AbstractCache {
     public:
      /**
       * @return high border of cache limit (@see AbstractCache#addItem)
       */
      uint32_t getIndexSizeHigh() const {
        return static_cast<const T &>(*this).getIndexSizeHighImpl();
      }

      /**
       * @return low border of cache limit (@see AbstractCache#addItem)
       */
      uint32_t getIndexSizeLow() const {
        return static_cast<const T &>(*this).getIndexSizeLowImpl();
      }

      /**
       * @return amount of items in cache
       */
      uint32_t getCacheItemCount() const {
        return static_cast<const T &>(*this).getCacheItemCountImpl();
      }

      /**
       * Adds new item to cache. When amount of cache records reaches
       * getIndexSizeHigh() a procedure of clean starts until getIndexSizeLow()
       * records left. Note: cache does not have a remove method, deletion
       * performs automatically.
       * Since every add operation can potentially lead to deletion,
       * it should be protected by mutex.
       * @param key - key to insert
       * @param value - value to insert
       */
      void addItem(const KeyType &key, const ValueType &value) {
        static_cast<T &>(*this).addItemImpl(key, value);
      }

      /**
       * Performs a search for an item with a specific key.
       * @param hash - key to find
       * @return Optional of ValueType
       */
      boost::optional<ValueType> findItem(const KeyType &key) const {
        return static_cast<const T &>(*this).findItemImpl(key);
      }
    };
  }
}

#endif  // IROHA_ABSTRACT_CACHE_HPP
