
#include <string>
#include <functional>
#include <iostream>

#include <cassert>

namespace util{
  static int successful = 0;

  int finish(){
    return successful;
  }

  void test(const std::string test_name, std::function<bool()> f){
    static int count = 0;
    [f](std::string name) mutable{
      count++;
      std::cout <<"["<< count <<"]====[ "<< name <<" ]====\n";
      if(f()){
        std::cout <<"\x1b[32m Passed!! \x1b[39m\n";
      }else{
        std::cout <<"\x1b[31m Failed!! \x1b[39m\n";
        successful = 1;
        #ifdef ASSERT_HALT
          assert(0);
        #endif
      }
    }(test_name);
  }

}
