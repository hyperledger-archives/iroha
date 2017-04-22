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

#include "config_utils.hpp"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utils/logger.hpp>

namespace config {
const char* get_iroha_home() {
  auto iroha_home = getenv("IROHA_HOME");
  if (!iroha_home) {
    logger::fatal("config") << "Set environment variable IROHA_HOME";
    exit(EXIT_FAILURE);
  }

  auto iroha_home_with_slash = std::string(iroha_home);
  if (iroha_home_with_slash.back() != '/') {
    iroha_home_with_slash += '/';
  }

  struct stat info;
  if (stat(iroha_home_with_slash.c_str(), &info) != 0) {
    logger::fatal("config")
        << "Cannot access IROHA_HOME directory. Does it exist?";
    exit(EXIT_FAILURE);
  }

  return iroha_home_with_slash.c_str();
}
}
