#ifndef __TIME_HPP_
#define __TIME_HPP_

#include <string>

namespace datetime {

  long int unixtime();
  std::string unixtime_str();
  std::string date_str();
  
  std::string unixtime2date(long int unixtime);
};

#endif
