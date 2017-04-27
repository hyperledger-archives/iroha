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
template <typename K, typename V>
class CacheMap {
private:
  size_t max_chache_size_;
  std::unordered_map<K,V> data_;
  std::deque<K> max_cache_;
  std::deque<K> cache_;

 public:
  CacheMap(size_t max_chache_size = 1) : max_chache_size_(max_chache_size) {}
  ~CacheMap() { clear(); }

  // set max_chache_size
  void set_cache_size(size_t);

  // [] oprator
  V& operator[](const K& k);
  V& operator[](K&& k);

  // erase base-key
  size_t erase(const K& k);

  // get maximum key
  const K& getMaxKey() const { return max_cache_.front(); }

  size_t max_size() const noexcept { return max_chache_size_; }
  size_t size() const noexcept { return data_.size(); }
  bool empty() const noexcept { return data_.empty(); }
  size_t count(const K& k) const { return data_.count(k); }
  void clear() noexcept {
    data_.clear();
    max_cache_.clear();
    cache_.clear();
  }

};
}

#endif  // IROHA_CACHEMAP_HPP
