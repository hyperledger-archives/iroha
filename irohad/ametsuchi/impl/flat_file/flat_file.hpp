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

#ifndef IROHA_FLAT_FILE_HPP
#define IROHA_FLAT_FILE_HPP

#include <memory>
#include <nonstd/optional.hpp>
#include <string>
#include <vector>

namespace iroha {
  namespace ametsuchi {
    class FlatFile {
     public:
      static std::unique_ptr<FlatFile> create(const std::string &path);
      ~FlatFile();
      void add(uint32_t id, const std::vector<uint8_t> &block);
      nonstd::optional<std::vector<uint8_t>> get(uint32_t id) const;
      uint32_t last_id() const;
      std::string directory() const;

     private:
      uint32_t current_id;
      const std::string dump_dir;

      FlatFile(uint32_t current_id, const std::string &path);
      bool file_exist(const std::string &name) const;
      long file_size(const std::string &filename) const;
    };
  }  // namespace ametsuchi
}  // namespace iroha
#endif  // IROHA_FLAT_FILE_HPP
