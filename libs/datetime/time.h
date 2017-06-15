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

#ifndef IROHA_TIME_H
#define IROHA_TIME_H

#include <chrono>

namespace iroha {

namespace time {

using namespace std::chrono;


/**
 * Returns current UNIX timestamp represented in 4 bytes.
 * Represents number of seconds since epoch.
 */
int32_t now32() {
  system_clock::time_point tp = high_resolution_clock::now();
  system_clock::duration d = tp.time_since_epoch();

  auto secs_in_period = static_cast<double>(system_clock::period::num) /
                        system_clock::period::den;
  // d.count()          in periods
  // d.count() * (...)  is seconds
  return d.count() * secs_in_period;
}

/**
 * Returns current UNIX timestamp represented in 8 bytes.
 * Represents number of seconds since epoch.
 */
int64_t now64() {
  high_resolution_clock::time_point tp = high_resolution_clock::now();
  high_resolution_clock::duration d =
      duration_cast<microseconds>(tp.time_since_epoch());
  auto secs_in_period = static_cast<double>(system_clock::period::num) /
                        system_clock::period::den;
  // d.count()          in periods
  // d.count() * (...)  is seconds
  return d.count() * secs_in_period;
}

//TODO: get these to work
std::string unixtime_str() {
  std::time_t result = std::time(nullptr);
  return std::to_string(result);
}

std::uint64_t unixtime() {
  return static_cast<std::uint64_t>(std::time(nullptr));
}

std::string date_str() {
  std::time_t result = std::time(nullptr);
  return std::asctime(std::localtime(&result));
}

std::string unixtime2date(time_t unixtime) {
  return std::asctime(std::localtime(&unixtime));
}

}
}

#endif  // IROHA_TIME_H
