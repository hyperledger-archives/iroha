#include <string>
#include <iostream>
#include <string>
#include <memory>

#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#include <cstring>

#include <memory>
#include <vector>

#include "base64.hpp"

namespace base64 {

  std::shared_ptr<unsigned char[]> decode(std::string encoded) { //Decodes a base64 encoded string

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
    bio = BIO_new_mem_buf((void*)encoded.c_str(), -1);
    bio = BIO_push(b64.get(), bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    int status = BIO_read(bio, message, encoded.size());
    if(status != length){
      std::cout << status <<" "<< length << "Decode failed \n";
      // ToDo Error 
    }
    return std::make_shared<unsigned char[]>(message);
  }

  std::string encode(std::shared_ptr<unsigned char[]> message) {
    if(strlen((const char*)message.get()) == 0){
      return "";
    }

    BUF_MEM *bufMem;
    std::shared_ptr<BIO> b64(BIO_new(BIO_f_base64()),BIO_free_all);
    std::shared_ptr<BIO> bio(
      BIO_push(b64.get(), BIO_new(BIO_s_mem())),
      [](BIO* bio){ BIO_set_close(bio, BIO_CLOSE); }
    );

    BIO_set_flags(bio.get(), BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio.get(), message.get(), strlen((const char*)message.get()));
    BIO_flush(bio.get());
    BIO_get_mem_ptr(bio.get(), &bufMem);

    std::string result = std::string((*bufMem).data);
    return result;
  }


}  // namespace base64
