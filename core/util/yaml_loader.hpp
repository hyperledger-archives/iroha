#ifndef __YAML_LOADER_HPP_
#define __YAML_LOADER_HPP_

#include <string>

#include "logger.hpp"
#include "terminate.hpp"

// Umm..., This is really util? not service?
// ToDo future work
// make yaml_service and separate yaml-cpp library from yaml service.
namespace yaml{

  class YamlLoader{
    std::string fileName;
   public:   
 
    YamlLoader(std::string fileName);
    
    template<typename T>
    T get(const std::string &root,const std::string &key);

    YamlLoader(YamlLoader const&) = delete;
    YamlLoader(YamlLoader&&) = delete;
    YamlLoader& operator =(YamlLoader const&) = delete;
    YamlLoader& operator =(YamlLoader&&) = delete;
  };

};

#endif
