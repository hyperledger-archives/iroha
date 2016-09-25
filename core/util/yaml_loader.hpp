#ifndef __YAML_LOADER_HPP_
#define __YAML_LOADER_HPP_

#include <yaml-cpp/yaml.h>
#include <string>

#include "logger.hpp"
#include "terminate.hpp"

// Umm..., This is really util? not service?
// ToDo future work
// make yaml_service and separate yaml-cpp library from yaml service.
namespace yaml{

  class YamlLoader{
   public:   
 
    explicit YamlLoader(std::string fileName) :
      config(YAML::LoadFile(std::move(fileName)))
    {}
    
    template<typename T>
    T get(const std::string &root,const std::string &key) {
      try{
       return config[root][key].as<T>();
      }catch(YAML::Exception& e){
        logger::fital("YamlLoader.get()", e.what());
        terminate::finish();
      }
    }

   private:
    YAML::Node config;

    YamlLoader(YamlLoader const&) = delete;
    YamlLoader(YamlLoader&&) = delete;
    YamlLoader& operator =(YamlLoader const&) = delete;
    YamlLoader& operator =(YamlLoader&&) = delete;
  };

};

#endif
