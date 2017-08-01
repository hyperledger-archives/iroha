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

#include <vector>
#include <string>
#include "common/blob_converter.hpp"

namespace iroha {
  namespace common {

    std::vector<uint8_t> convert(std::string &source) {
      std::vector<uint8_t> result;
      for (auto &&chr: source) {
        result.push_back(chr);
      }
      return result;
    }

    std::string convert(std::vector<uint8_t> &source) {
      std::string result;
      for (auto &&elem: source) {
        result += elem;
      }
      return result;
    }

  } // namespace common
} // namespace iroha