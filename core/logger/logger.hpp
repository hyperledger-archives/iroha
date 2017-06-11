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

#include <spdlog/spdlog.h>

namespace logger {

struct Logger {
    std::string log_name;
    std::shared_ptr<spdlog::logger> console;

    explicit Logger(std::string &&name);
    explicit Logger(const std::string &name);

    void debug(std::string &&msg);
    void debug(const std::string &msg);

    void info(std::string &&msg);
    void info(const std::string &msg);

    void warning(std::string &&msg);
    void warning(const std::string &msg);

    void error(std::string &&msg);
    void error(const std::string &msg);

};

}  // namespace logger

#endif
