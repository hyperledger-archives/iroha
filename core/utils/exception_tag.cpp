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

#include <stdexcept>

#include "exception_tag.hpp"

#if 0

namespace exception_tag {

ExceptionTag::~ExceptionTag() {}

/**
 * NoError::tag()
 * - This is for exception::None. This tag() assumes not to be called.
 */
std::string NoError::tag() const { return "NO_ERROR"; }

/**
 * Critical
 * - This is bug. It would help if you report by GitHub issue.
 */
std::string Critical::tag() const { return "CRITICAL"; }

std::string WontFix::tag() const { return "WONT_FIX"; }

std::string HelpWanted::tag() const { return "HELP_WANTED"; }

/**
 * ConfigError
 * - User defined configuration is wrong.
 */
std::string ConfigError::tag() const { return "CONFIG_ERROR"; }

}  // namespace exception_tag

#endif
