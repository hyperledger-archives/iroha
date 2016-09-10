#include <string>
#include <iostream>
#include <string>
#include <memory>

#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#include <memory>
#include <vector>

#include "base64.hpp"

namespace base64 {

  const unsigned char* decode(std::string encoded) { //Decodes a base64 encoded string
    if(encoded.size() == 0){
      return (unsigned char*)"";
    }

    BIO *bio;

    int padding = 0;
    if(encoded[encoded.size()-1] == '='
        && encoded[encoded.size()-2] == '='){
      padding = 2;
    }else if(encoded[encoded.size()-1] == '='){
      padding = 1;
    }

    int length = (encoded.size()*3)/4 - padding;
    unsigned char* message = (unsigned char*)malloc(length + 1);
    message[length] = '\0';
    std::shared_ptr<BIO> b64(BIO_new(BIO_f_base64()),BIO_free_all);
    bio = BIO_new_mem_buf(encoded.c_str(), -1);
    bio = BIO_push(b64.get(), bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    int status = BIO_read(bio, message, encoded.size());
    if(status != length){
      std::cout << status <<" "<< length << "Decode failed \n";
      // ToDo Error 
    }
    return message;
  }

  std::string encode(const unsigned char* message) {
    if(strlen((const char*)message) == 0){
      return "";
    }

    BUF_MEM *bufMem;
    std::shared_ptr<BIO> b64(BIO_new(BIO_f_base64()),BIO_free_all);
    std::shared_ptr<BIO> bio(
      BIO_push(b64.get(), BIO_new(BIO_s_mem())),
      [](BIO* bio){ BIO_set_close(bio, BIO_CLOSE); }
    );

    BIO_set_flags(bio.get(), BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio.get(), message, strlen((const char*)message));
    BIO_flush(bio.get());
    BIO_get_mem_ptr(bio.get(), &bufMem);

    std::string result = std::string((*bufMem).data);
    return result;
  }


}  // namespace base64
