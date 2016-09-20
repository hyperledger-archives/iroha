#ifndef __TIME_HPP_
#define __TIME_HPP_

namespace time{

  #include <string>

  #include <time.h>
  
  std::string time_str(){
    time_t timer;
    struct tm *t_st;
    time(&timer);
    return ctime(time);
  }
};

#endif
