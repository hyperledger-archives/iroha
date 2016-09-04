#ifndef __SIGNATURE_H__
#define __SIGNATURE_H__

#include <string>
#include <memory>

#include "../domain/entity.hpp"

namespace signature{

  //=== Deprecated use for debug. ===
  bool verify(std::string signature,std::string message, std::string publicKeyName);
  std::string sign(std::string message,std::string privateKeyName,std::string publicKeyName);
  bool generateKeyPair(std::string filenamePrefix,std::string keyPath);
  //===

  template<typename T>
  bool verify(std::string signature,std::string message,T dummy){
    //ToDo throw illegal type exception
  }
  template<typename T>
  std::string sign(std::string message,T dummy){
    //ToDo throw illegal type exception
  }
  template<typename T>
  std::string generateKeyPair(T dummy){
    //ToDo throw illegal type exception
  }

  template<>
  bool verify<std::shared_ptr<Entity>>(
    const std::string signature,
    const std::string message,
    const std::shared_ptr<Entity> entity){
    //ToDo
  }
  template<>
  std::string sign<std::shared_ptr<Entity>>(
    const std::string message,
    const std::shared_ptr<Entity> entity){
    //ToDo
  }
  template<>
  std::string generateKeyPair<std::shared_ptr<Entity>>(
    const std::shared_ptr<Entity> entity){
    //ToDo throw illegal type exception
  }

};

#endif
