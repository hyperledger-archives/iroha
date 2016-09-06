#include "../util/exception.hpp"
#include "base64.hpp"
#include "hash.hpp"

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <cstring>
#include <algorithm>

#include <ed25519.h>


#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>

namespace signature{

  int padding = RSA_PKCS1_PADDING;

  const int RSABITNUM = 4096;
  const int RSABYTENUM = RSABITNUM/8;

  std::shared_ptr<RSA> loadPrivatekey(std::string keyfilename);
  std::shared_ptr<RSA> loadPublickey(std::string keyfilename);

  struct KeyPair{
    std::shared_ptr<RSA>  publicKey;
    std::shared_ptr<RSA> privateKey;
    std::string  publicFileName;
    std::string privateFileName;

    KeyPair(std::string t_publicKeyName,std::string t_privateKeyName):
      publicFileName(t_publicKeyName),
      privateFileName(t_privateKeyName),
      publicKey(loadPublickey(t_publicKeyName)),
      privateKey(loadPrivatekey(t_privateKeyName))
    {}
  };

  void openFile(std::string filename, std::shared_ptr<FILE> keyFile){
    if((keyFile = std::shared_ptr<FILE>(fopen( filename.c_str(), "r"))) == NULL) {
       throw exception::FileOpenException(filename);
    }
  }

  std::string convertKeyForJson(std::string keyData){
    std::replace( keyData.begin(), keyData.end(), ' ', '_');
    size_t c;
    while((c = keyData.find_first_of("\n")) != std::string::npos){
      keyData.erase(c,1);
    }
    return keyData;
  }

  std::string loadKeyFile(std::string keyName){
    std::ifstream ifs(keyName);
    if (ifs.fail()){
        throw exception::FileOpenException(keyName);
    }
    return std::string((std::istreambuf_iterator<char>(ifs)),  std::istreambuf_iterator<char>());
  }

  std::shared_ptr<FILE> open(std::string filename,char* type = (char*)"r"){
    std::shared_ptr<FILE> file( fopen(filename.c_str(), type), [](FILE *fp){ fclose(fp); });
    if(file == nullptr){
      throw exception::FileOpenException(filename);
    }
    return file;
  }

  bool createKeyPair(std::string filenamePrefix,std::string keyPath){
    std::ofstream publicOfs(keyPath +"/"+ filenamePrefix + "_public.pem");
    std::ofstream privateOfs(keyPath +"/"+ filenamePrefix + "_private.pem");

    unsigned char publicKey[32], privateKey[64], seed[32];

    ed25519_create_seed(seed);
    ed25519_create_keypair(publicKey, privateKey, seed);

    publicOfs << base64::encode(publicKey);
    privateOfs << base64::encode(privateKey);

    return true;
  }

  std::string sign(std::string message, std::string privateKeyName,std::string publicKeyName){
    auto privateKey = loadKeyFile(privateKeyName);
    auto publicKey  = loadKeyFile(publicKeyName);
    unsigned char signature[64];

    ed25519_sign(
      signature,
      reinterpret_cast<const unsigned char*>(message.c_str()),
      message.size(),
      reinterpret_cast<const unsigned char*>(base64::decode(publicKey)),
      reinterpret_cast<const unsigned char*>(base64::decode(privateKey))
    );
    return base64::encode(signature);
  }

  bool verify(std::string signature,std::string message, std::string publicKeyName){
      auto publicKey  = loadKeyFile(publicKeyName);
      return ed25519_verify(
        reinterpret_cast<const unsigned char*>(base64::decode(signature)),
        reinterpret_cast<const unsigned char*>(message.c_str()),
        message.size(),
        reinterpret_cast<const unsigned char*>(base64::decode(publicKey))
      );
  }

  bool test(){

    signature::createKeyPair("test", "../key");

    //unsigned char publicKey[32], privateKey[64], seed[32];

    //ed25519_create_seed(seed);
    //ed25519_create_keypair(publicKey, privateKey, seed);


    auto privateKey = loadKeyFile("../key/mizuki_private.pem");
    auto publicKey  = loadKeyFile("../key/mizuki_public.pem");
    unsigned char signature[64];

//    std::string message = "message";
    const unsigned char* message = reinterpret_cast<const unsigned char*>("message");
    ed25519_sign(
      signature,
      message,//reinterpret_cast<const unsigned char*>(message.c_str()),
      strlen((char*)message),//message.size(),
      reinterpret_cast<const unsigned char*>(base64::decode(publicKey)),
      reinterpret_cast<const unsigned char*>(base64::decode(privateKey))
    );

    return ed25519_verify(
      signature,
      message,//reinterpret_cast<const unsigned char*>(message.c_str()),
      strlen((char*)message),//message.size(),
      reinterpret_cast<const unsigned char*>(base64::decode(publicKey))
    );

  }

};
