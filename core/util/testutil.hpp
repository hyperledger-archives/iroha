#ifndef __TEST_H_
#define __TEST_H_

#include <string.h>

#include <string>
#include <functional>
#include <iostream>

namespace util{

  int  finish();
  void test(const std::string test_name, std::function<bool()> f);
  void before(std::function<void()> f);
  void after(std::function<void()> f);

  template<typename T>
  bool equals(T a, T b){
    if(a == b){
      return true;
    }else{
      std::cout << a <<" != "<< b << std::endl;
      return false;
    }
  }

  bool equals(const unsigned char* a,const unsigned char*  b){
    if(strlen((char*)a) != strlen((char*)b)){
      return false;
    }
    for(size_t i=0;i<strlen((char*)a);i++){
      if( a[i] != b[i]){
        std::cout << a[i] <<" != "<< b[i] << std::endl;
        return false;
      }
    }
    return true;
  }
}

#endif
