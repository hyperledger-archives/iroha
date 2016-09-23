#include <SimpleFIPS202.h>
#include <string>

#include "hash.hpp"

namespace hash {
  std::string sha3_256_hex(std::string message) {
    char code[] =
      {'0', '1', '2', '3', '4', '5', '6', '7', '8',
        '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    unsigned char output[64];
    SHA3_256(
      output,
      (const unsigned char *)message.c_str(),
      message.size());

    std::string res = "";
    char front, back;
    for (int i = 0; i < 32; i++) {
      front = (output[i] & 240) >> 4;
      back =  output[i] &  15;
      res += code[front];
      res += code[back];
    }
    return res;
  }

  std::string sha3_512_hex(std::string message) {
    char code[] =
      {'0', '1', '2', '3', '4', '5', '6', '7', '8',
        '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    unsigned char output[128];
    SHA3_512(
      output,
      (const unsigned char *)message.c_str(),
      message.size());

    std::string res = "";
    char front, back;
    for (int i = 0; i < 64; i++) {
      front = (output[i] & 240) >> 4;
      back =  output[i] &  15;
      res += code[front];
      res += code[back];
    }
    return res;
  }

}  // namespace hash
