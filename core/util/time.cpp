#ifndef __TIME_HPP_
#define __TIME_HPP_

#include "time.hpp"

#include <ctime>
#include <string>


namespace datetime{

  std::string unixtime_str(){
    std::time_t result = std::time(nullptr);
    return std::to_string(result);
  }

  std::string date_str(){
    std::time_t result = std::time(nullptr);
    return std::asctime(std::localtime(&result));
  }

};

#endif
