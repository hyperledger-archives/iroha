#ifndef __LOGGER_HPP_
#define __LOGGER_HPP_

#include <string>
#include <iostream>
    
#include "time.hpp"

namespace logger{

  enum class LogLevel{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FITAL
  };

  const LogLevel LOG_LEVEL = LogLevel::DEBUG;

  void debug(
    const std::string &name,
    const std::string &message,
    std::ostream &out = std::cout);

  void info(
    const std::string &name,
    const std::string &message,
    std::ostream &out = std::cout);

  void warning(
    const std::string &name,
    const std::string &message,
    std::ostream &out = std::cout);

  void error(
    const std::string &name,
    const std::string &message,
    std::ostream &out = std::cout);

  void fital(
    const std::string &name,
    const std::string &message,
    std::ostream& out = std::cout);

};
#endif
