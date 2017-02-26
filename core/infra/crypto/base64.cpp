/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <crypto/base64.hpp>

namespace base64 {

namespace vendor {
  /*
   base64_encode(), base64_decode()
   Copyright (C) 2004-2008 René Nyffenegger
   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.
   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:
   1. The origin of this source code must not be misrepresented; you must not
   claim that you wrote the original source code. If you use this source code
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original source code.
   3. This notice may not be removed or altered from any source distribution.
   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
   */
  #include <string.h>
  #include <stdlib.h>
  #include <ctype.h>

  static const char* base64_chars =
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "abcdefghijklmnopqrstuvwxyz"
                 "0123456789+/";

  static inline int is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
  }
  // Update argument is std::vector<unsigned char>
  std::string base64_encode(const std::vector<unsigned char> &bytes_to_encode, unsigned int in_len){
    // Update ret is std::vector<char>
    std::vector<char> ret((int)(in_len * 2.8));
    int pointer = 0; // Update Add new value.
    int pointer_of_bytes_to_encode = 0; // Update Add new value.
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
      char_array_3[i++] = bytes_to_encode[pointer_of_bytes_to_encode];
      pointer_of_bytes_to_encode++;

      if (i == 3) {
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for(i = 0; (i <4) ; i++){
          ret[pointer] = base64_chars[char_array_4[i]]; // Update use index
          pointer++; // Update increments pointer
        }
        i = 0;
      }
    }

    if (i)
    {
      for(j = i; j < 3; j++)
        char_array_3[j] = '\0';

      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (j = 0; (j < i + 1); j++){
        ret[pointer] = base64_chars[char_array_4[j]]; // Update use index
        pointer++; // Update increments pointer
      }

      while((i++ < 3)){
        ret[pointer] = '=';// Update use index
        pointer++; // Update increments pointer
      }
    }
    std::string res_str;
    for(auto c : ret){
      res_str += c;
    }
    return res_str;
  }

  // add find function instead of string's find()
  int base64_chars_find(char c){
    for(size_t i=0;i < strlen(base64_chars); i++){
      if(c == base64_chars[i]) return i;
    }
    return -1;
  }

  std::vector<unsigned char> base64_decode(const std::string &encoded_string){
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::vector<unsigned char> ret((int)(in_len)); // use char*
    int pointer = 0; // add index value

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
      char_array_4[i++] = encoded_string[in_]; in_++;
      if (i ==4) {
        for (i = 0; i <4; i++)
          char_array_4[i] = base64_chars_find(char_array_4[i]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (i = 0; (i < 3); i++){
          ret[pointer] = char_array_3[i]; // use index
          pointer++;
        }
        i = 0;
      }
    }

    if (i) {
      for (j = i; j <4; j++)
        char_array_4[j] = 0;

      for (j = 0; j <4; j++)
        char_array_4[j] = base64_chars_find(char_array_4[j]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (j = 0; (j < i - 1); j++){
        ret[pointer] = char_array_3[j]; // use index
        pointer++;
      }
    }
    return ret;
  }
}  // namespace vendor

  const std::string encode(const std::vector<unsigned char> &message) {
    return vendor::base64_encode(
      message,
      message.size()
    );
  }

  std::vector<unsigned char> decode(const std::string &enc) {
    return vendor::base64_decode(enc);
  }

}  // namespace base64
