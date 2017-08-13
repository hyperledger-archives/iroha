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

#include <dirent.h>
#include "logger/logger.hpp"

void remove_all(const std::string &dump_dir) {
  auto log = logger::log("common::remove_all");
  if (!dump_dir.empty()) {
    // Directory iterator:
    struct dirent **namelist;
    auto status = scandir(dump_dir.c_str(), &namelist, nullptr, alphasort);
    if (status < 0) {
      log->error("Internal error on scanning folder {}", dump_dir);
    } else {
      uint n = status;
      uint i = 1;
      while (++i < n) {
        if (std::remove((dump_dir + "/" + namelist[i]->d_name).c_str())) {
          log->error("Error on deletion file {}", namelist[i]->d_name);
        }
      }
      for (uint j = 0; j < n; ++j) {
        free(namelist[j]);
      }
      free(namelist);
    }
  }
}
