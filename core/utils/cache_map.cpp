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

#include <utils/cache_map.hpp>

namespace structure {
// set max_chache_size
template <typename K, typename V>
void CacheMap<K, V>::set_cache_size(size_t max_cache_size) {
  if (max_cache_size_ < max_cache_size) {
    max_cache_size_ = max_cache_size;
  } else {
    while (max_cache_size_ > max_cache_size) {
      max_cache_size--;
      erase_one();
    }
  }
}

template <typename K, typename V>
size_t CacheMap<K, V>::erase_one() {
  if (data_.empty()) return;
  K& k = cache_.front();
  cache_.pop_front();
  if (max_cache_.front() == k) max_cache_.pop_front();
  data_.erase(k);
  return data_.size();
}

template <typename K, typename V>
size_t CacheMap<K, V>::set(const K& k, const V& v) {
  cache_.push_back(k);
  while (!max_cache_.empty() && max_cache_.back() < k) max_cache_.pop_back();
  max_cache_.push_back(k);
  data_[k] = v;
}

// default map alike function
template <typename K, typename V>
const V& CacheMap<K, V>::operator[](const K& k) {
  if (data_.count(k) == 0) return V();
  return data_[k];
}

template <typename K, typename V>
const V& CacheMap<K, V>::operator[](K&& k) {
  if (data_.count(k) == 0) return V();
  return data_[k];
}


/*
    size_t max_chache_size_;
    unordered_map<K,V> data_;
    dequeu<K> max_cache_;
    dequeu<K> cache_;
    */
}  // namespace structure
