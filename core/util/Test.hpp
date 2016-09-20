#ifndef __TEST_H_
#define __TEST_H_

#include <string.h>

#include <string>
#include <functional>
#include <iostream>

namespace util {

  int  finish();
  void test(const std::string test_name, std::function<bool()> f);

  template<typename T>
  bool equals(T a, T b){
    if (a == b) {
      return true;
    } else {
      std::cout << a <<" != "<< b << std::endl;
      return false;
    }
  }

  bool equals(const unsigned char* a, const unsigned char*  b) {
    if (strlen((char*)a) != strlen((char*)b)) {
      return false;
    }
    for (int i = 0;i < strlen((char*)a); ++i) {
      if ( a[i] != b[i]) {
        std::cout << a[i] <<" != "<< b[i] << std::endl;
        return false;
      }
    }
    return true;
  }
};  // namespace util

#endif
