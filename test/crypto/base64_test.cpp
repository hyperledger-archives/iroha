
#include "../../core/crypto/base64.hpp"
#include "../../core/util/testutil.hpp"

#include <iostream>
#include <string>

int main(){

  util::test("Encode normal char*", []() -> bool{
    unsigned char* text = (unsigned char*)"Hey, I'm mizuki";
    std::cout<< base64::encode(text) << std::endl;
    return util::equals( base64::decode(base64::encode(text)), text);
  });

  return util::finish();
}
