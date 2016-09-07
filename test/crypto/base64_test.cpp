
#include "../../core/crypto/base64.hpp"
#include "../../core/util/test.hpp"

#include <iostream>
#include <string>

int main(){

  util::test("Encode normal char*", []() -> bool{
    std::string text = "Hey, I'm mizuki";
    std::cout<< base64::encode(text) << std::endl;
    return equals( base64::decode(base64::encode(text)), text);
  });

  return util::finish();
}
