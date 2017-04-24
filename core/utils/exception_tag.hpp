/*
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
 * http://soramitsu.co.jp
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *          http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_UTILS_EXCEPTION_TAG_HPP_
#define IROHA_UTILS_EXCEPTION_TAG_HPP_

#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>

namespace exception_tag {

class ExceptionTag {
 public:
  virtual std::string tag() const;
};

class Critical : public ExceptionTag {
 public:
  std::string tag() const override;
};

class WontFix : public ExceptionTag {
 public:
  std::string tag() const override;
};

class HelpWanted : public ExceptionTag {
 public:
  std::string tag() const override;
};

class ConfigError : public ExceptionTag {
 public:
  std::string tag() const override;
};

}  // namespace exception_tag

#endif  // IROHA_UTILS_EXCEPTION_TAG_HPP_
