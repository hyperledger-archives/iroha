/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

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
//
// Created by Takumi Yamashita on 2017/04/28.
//

#ifndef IROHA_CACHEMAP_HPP
#define IROHA_CACHEMAP_HPP

#include <deque>
#include <unordered_map>

namespace structure {

// CacheMap
/*
 * CacheMap is unordered_map that to identify max size alike.
 * CacheMap does not call erase method from external.
 * CacheMap does not overwrite.
 * CacheMap autoerases element that was last push, when over size.
 * CacheMap operate below all function, amortized complexity O(1) (clear is
 * not).
 */
template <typename Key, typename Value>
class CacheMap {
 private:
  size_t max_cache_size_;
  std::unordered_map<Key, Value> data_;
  std::deque<Key> max_cache_;
  std::deque<Key> cache_;

  // erase last push node
  size_t erase_one();

 public:
  CacheMap(size_t max_cache_size = 1) : max_cache_size_(max_cache_size) {}
  ~CacheMap() { clear(); }

  // set max_chache_size
  void set_cache_size(size_t);

  // set key and value
  size_t set(const Key&, const Value&);

  // [] oprator
  const Value& operator[](const Key& k);
  const Value& operator[](Key&& k);

  // get maximum key
  const Key& getMaxKey() const { return max_cache_.front(); }

  size_t max_size() const noexcept { return max_cache_size_; }
  size_t size() const noexcept { return data_.size(); }
  bool empty() const noexcept { return data_.empty(); }
  size_t count(const Key& k) const { return data_.count(k); }
  void clear() noexcept {
    data_.clear();
    max_cache_.clear();
    cache_.clear();
  }
};
} // namespace structure

#endif  // IROHA_CACHEMAP_HPP
