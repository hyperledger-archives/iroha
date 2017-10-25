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

#include "torii/cache/cache.hpp"

namespace torii {
  namespace cache {
    void ToriiResponseCache::addItem(
        const iroha::protocol::ToriiResponse &response,
        const std::string &hash) {
      std::lock_guard<std::mutex> lock(handler_map_mutex_);
      // elements with the same hash should be replaced
      handler_map_[hash] = response;
      handler_map_index_.push_back(hash);
      if (handler_map_.size() > getIndexSizeHigh()) {
        while (handler_map_.size() > getIndexSizeLow()) {
          handler_map_.erase(handler_map_index_.front());
          handler_map_index_.pop_front();
        }
      }
    }

    boost::optional<iroha::protocol::ToriiResponse>
    ToriiResponseCache::findItem(const std::string &hash) {
      auto found = handler_map_.find(hash);
      if (found == handler_map_.end()) {
        return boost::none;
      } else {
        iroha::protocol::ToriiResponse response;
        response.set_tx_status(handler_map_.at(hash).tx_status());
        return boost::make_optional<iroha::protocol::ToriiResponse>(response);
      }
    }

    uint32_t ToriiResponseCache::getIndexSizeHigh() const {
      return MAX_HANDLER_MAP_SIZE_HIGH;
    }

    uint32_t ToriiResponseCache::getIndexSizeLow() const {
      return MAX_HANDLER_MAP_SIZE_LOW;
    }

    uint64_t ToriiResponseCache::getCacheItemCount() const {
      return handler_map_.size();
    }
  }
}
