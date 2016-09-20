#ifndef __LOGGER_HPP_
#define __LOGGER_HPP_

namespace logger{

  #include <string>
  #include <iostream> // WIP
    
  #include "time.hpp"

  enum LogLevel{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FITAL
  };

  const LogLevel LOG_LEVEL = LggLevel::DEBUG;

  void logger(std::string name, std::string message){
    std::cout << time::time_str() << " ["<< name << "] "<< message << std::endl;
  }

};
#endif
