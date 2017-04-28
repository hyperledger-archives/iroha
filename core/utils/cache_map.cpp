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

// set max_chache_size
/*
template <typename Key, typename Value>
void structure::CacheMap<Key, Value>::set_cache_size(size_t max_cache_size)
template <typename Key, typename Value>
size_t structure::CacheMap<Key, Value>::erase_one()

template <typename Key, typename Value>
size_t structure::CacheMap<Key, Value>::set(const Key& k, const Value& v)

// default map alike function
template <typename Key, typename Value>
const Value& structure::CacheMap<Key, Value>::operator[](const Key& k)
template <typename Key, typename Value>
const Value& structure::CacheMap<Key, Value>::operator[](Key&& k) {
  if (data_.count(k) == 0) return Value();
  return data_[k];
}
*/