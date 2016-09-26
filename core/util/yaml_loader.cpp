#include <yaml-cpp/yaml.h>
#include <string>

#include "logger.hpp"
#include "terminate.hpp"
#include "yaml_loader.hpp"
#include "../service/peer_service.hpp"

// Umm..., This is really util? not service?
// ToDo future work
// make yaml_service and separate yaml-cpp library from yaml service.
namespace yaml{


    YamlLoader::YamlLoader(std::string afileName) :
      fileName(fileName)
    {}
    

    template<typename T>
    T YamlLoader::get(const std::string &root,const std::string &key) {
        YAML::Node config = YAML::LoadFile(std::move(fileName));
        try{
            return config[root][key].as<T>();
        }catch(YAML::Exception& e){
            logger::fital("YamlLoader.get()", e.what());
            terminate::finish();
        }
    }
/*
    template <>
    std::string YamlLoader::get<
        std::string
    >(std::string root, std::string key);

    template std::vector<peer::Node> YamlLoader::get<
        std::vector<peer::Node>
    >(std::string root, std::string key);

    template std::vector<std::string> YamlLoader::get<
        std::vector<std::string>
    >(std::string root, std::string key);    
*/    
};