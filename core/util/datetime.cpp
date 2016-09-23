#ifndef __TIME_HPP_
#define __TIME_HPP_

#include "datetime.hpp"

#include <ctime>

namespace datetime{

  std::string unixtime_str(){
    std::time_t result = std::time(nullptr);
    return std::to_string(result);
  }

  long int unixtime(){
    return static_cast<long int>(std::time(nullptr));
  }

  std::string date_str(){
    std::time_t result = std::time(nullptr);
    return std::asctime(std::localtime(&result));
  }

  std::string unixtime2date(long int unixtime){
    return std::asctime(std::localtime(&static_cast<time_t>(unixtime)));
  }
};

#endif
