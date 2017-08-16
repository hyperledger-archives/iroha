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
#ifndef __IROHA_LOGGER_LOGGER_HPP__
#define __IROHA_LOGGER_LOGGER_HPP__

#include <sstream>
#include <string>
#include <functional>
#include <algorithm>
#include <nonstd/optional.hpp>

#include <spdlog/spdlog.h>

namespace logger {

  using Logger = std::shared_ptr<spdlog::logger>;

  std::string red(const std::string &string);

  std::string yellow(const std::string &string);

  std::string output(const std::string &string);

  std::string input(const std::string &string);

  /**
   * Provide logger object
   * @param tag - tagging name for identifiing logger
   * @return logger object
   */
  Logger log(const std::string &tag);

  /**
   * Provide logger for using in test purposes;
   * This logger write data only for console
   * @param tag - tagging name for identifiing logger
   * @return logger object
   */
  Logger testLog(const std::string &tag);

  /**
   * Convert bool value to human readable string repr
   * @param value value for transformation
   * @return "true" or "false"
   */
  std::string boolRepr(bool value);

  /**
   * Converts object to bool and provides string repr of it
   * @tparam T - type of object, T must implement bool operator
   * @param val - value for convertation
   * @return string representation of bool object
   */
  template<typename T>
  std::string logBool(T val) {
    return boolRepr(bool(val));
  }

  /**
   * Function provide string representation of collection
   * @tparam Collection - type should implement for semantic
   * @tparam Lambda - function that transform argument to string
   * @param collection - bunch of objects
   * @param transform - function that convert object to string
   * @return string repr of collection
   */
  template<class Collection, class Lambda>
  std::string to_string(Collection collection, Lambda transform) {
    const std::string left_bracket = "{";
    const std::string right_bracket = "}";
    const std::string separator = ", ";

    std::string result = left_bracket;
    std::for_each(collection.begin(), collection.end(),
                  [&result, &transform, &separator](auto value) {
                    result += transform(value) + separator;
                  });
    result = result.substr(0, result.size() - separator.size());
    return result + right_bracket;
  }

  /**
   * Function provide string representation of optional value
   * @tparam Optional - type of optional
   * @tparam Lambda - function that consume value type and return std::string
   * @param opt - value wrapped by optional
   * @param transform - function that transforming value to std::string
   * @return string repr of value
   */
  template<class Optional, class Lambda>
  std::string opt_to_string(Optional opt, Lambda transform){
    const std::string null_value = "nullopt";
    return opt == nonstd::nullopt ? null_value : transform(*opt);
  }

}  // namespace logger

#endif
