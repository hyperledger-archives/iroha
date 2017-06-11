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

#include "logger.hpp"

namespace logger {

    //enum class LogLevel { Debug = 0, Explore, Info, Warning, Error, Fatal };
    static const std::string level_names[]{
        "DEBUG", "INFO", "WARNING", "ERROR (-A-)"
    };

    Logger::Logger(std::string &&name):
      log_name(name)
    {};
    Logger::Logger(const std::string &name):
      log_name(name)
    {};

    void Logger::debug(std::string &&msg){
      console->debug(msg);
    }
    void Logger::debug(const std::string &msg){
      console->debug(msg);
    }

    void Logger::info(std::string &&msg){
      console->info(msg);
    }
    void Logger::info(const std::string &msg){
      console->info(msg);
    }

    void Logger::warning(std::string &&msg){
      console->warn(msg);
    }
    void Logger::warning(const std::string &msg){
      console->warn(msg);
    }

    void Logger::error(std::string &&msg){
      console->error(msg);
    }
    void Logger::error(const std::string &msg){
      console->error(msg);
    }
}