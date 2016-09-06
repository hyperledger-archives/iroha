#include <SimpleFIPS202.h>
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

namespace base64{

  unsigned char* decode(std::string encoded) { //Decodes a base64 encoded string
    std::cout <<"encoded["<< encoded <<"] will decode"<< std::endl;
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
      [](BIO* bio){BIO_set_close(bio, BIO_CLOSE);}
    );

    BIO_set_flags(bio.get(), BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio.get(), message, strlen((const char*)message));
    BIO_flush(bio.get());
    BIO_get_mem_ptr(bio.get(), &bufMem);

    std::string result = std::string((*bufMem).data);
    std::cout <<"encoded["<< result << "]\n";
    return result;
  }

  // ２回目でなぜか落ちる。できればこちらを使いたい
  std::string encode_bad(const unsigned char* msg){
    std::string message = reinterpret_cast<const char*>(msg);
    message += '\n';
    message += '\0';

    auto bptr = BUF_MEM_new();

    std::shared_ptr<BIO> b64( BIO_new(BIO_f_base64()), [](BIO *b64){ BIO_free_all(b64); });

    auto bmem = BIO_new(BIO_s_mem());
    BIO_push( b64.get(), bmem);
    BIO_write( b64.get(), message.c_str(), message.size());
    BIO_flush( b64.get());
    BIO_get_mem_ptr( b64.get(), &bptr);
    char *buffer = (char *)malloc(bptr->length);
    memcpy(buffer, bptr->data, bptr->length - 1);
    buffer[bptr->length - 1] = '\0';
    return std::string(buffer);
  }

  // ２回目でなぜか落ちる。できればこちらを使いたい
  const unsigned char* decode_bad(std::string in){
    BIO *buff;
    std::shared_ptr<BIO> b64( BIO_new(BIO_f_base64()), [](BIO *b64){ BIO_free_all(b64); });
    buff = BIO_new_mem_buf((void *)in.c_str(), in.size());
    buff = BIO_push(b64.get(), buff);
    auto out = (unsigned char*) malloc(in.size() * sizeof(unsigned char));
    BIO_set_flags(buff, BIO_FLAGS_BASE64_NO_NL);
    return out;
  }

};
