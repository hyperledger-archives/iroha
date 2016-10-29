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

#ifndef __LOGGER_HPP_
#define __LOGGER_HPP_

#include <string>
#include <iostream>
    
#include "datetime.hpp"
#include "logger.hpp"

namespace logger{

  void debug(
    const std::string &name,
    const std::string &message,
    std::ostream &out = std::cout) {
    out << datetime::unixtime_str() << \
      " DEBUG ["<< name << "] "<< message << std::endl;
  }

  void info(
    const std::string &name,
    const std::string &message,
    std::ostream &out = std::cout) {
    out << datetime::unixtime_str() << \
      " INFO ["<< name << "] "<< message << std::endl;
  }

  void warning(
    const std::string &name,
    const std::string &message,
    std::ostream &out = std::cout) {
    out << datetime::unixtime_str() << \
      " WARNING ["<< name << "] "<< message << std::endl;
  }

  void error(
    const std::string &name,
    const std::string &message,
    std::ostream &out = std::cout) {
    out << datetime::unixtime_str() << \
      " ERROR (-A-) ["<< name << "] "<< message << std::endl;
  }

  void fital(
    const std::string &name,
    const std::string &message,
    std::ostream& out = std::cout) {
    out << datetime::unixtime_str() << \
      " FITAL (`o') ["<< name << "] "<< message << std::endl;
  }

};
#endif
