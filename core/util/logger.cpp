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

#include <string>
#include <iostream>
    
#include "datetime.hpp"
#include "logger.hpp"

namespace logger {


    #define LOGGER_DEF_IMPL(LoggerName, UseLevel, HasPrefix, LogType)               \
    LoggerName::LoggerName(std::string&& caller) noexcept                           \
      : caller(std::move(caller)),                                                  \
        uncaught(STD_UNC_EXC())                                                     \
    {}                                                                              \
    LoggerName::~LoggerName() {                                                     \
        if  ( COND_UNC_EXC                                                          \
              &&                                                                    \
              static_cast<int>(detail::LOG_LEVEL) <= static_cast<int>(UseLevel)     \
            ) {                                                                     \
            const auto useCErr = static_cast<int>(LogLevel::ERROR) <= static_cast<int>(UseLevel);   \
            ( useCErr ? std::cerr : std::cout )                                     \
                        << datetime::unixtime_str()                                 \
                        << (HasPrefix ?                                             \
                            std::string(" ") + LogType + " [" + caller + "] "       \
                            :                            "["  + caller + "] "       \
                           )                                                        \
                        << stream.str() << std::endl;                       	\
        }                                                                   	\
    }

    LOGGER_DEF_IMPL(debug,   LogLevel::DEBUG,    true,   "DEBUG")
    LOGGER_DEF_IMPL(info,    LogLevel::INFO,     true,   "INFO")
    LOGGER_DEF_IMPL(warning, LogLevel::WARNING,  true,   "WARNING")
    LOGGER_DEF_IMPL(error,   LogLevel::ERROR,    true,   "ERROR (-A-)")
    LOGGER_DEF_IMPL(fatal,   LogLevel::FATAL,    true,   "FATAL (`o')")
    LOGGER_DEF_IMPL(explore, LogLevel::EXPLORE,  false,  "(EXPLORE)")
}