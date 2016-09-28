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

    template <>
    std::string YamlLoader::get<
        std::string
    >(const std::string& root, const std::string& key){
        YAML::Node config = YAML::LoadFile(std::move(fileName));
        try{
            return config[root][key].as<std::string>();
        }catch(YAML::Exception& e){
            logger::fital("YamlLoader.get()", e.what());
            terminate::finish();
        }
    }

    template <> 
    std::vector<peer::Node> YamlLoader::get<
        std::vector<peer::Node>
    >(const std::string& root, const std::string& key){
        YAML::Node config = YAML::LoadFile(std::move(fileName));
        try{
            return config[root][key].as<std::vector<peer::Node>>();
        }catch(YAML::Exception& e){
            logger::fital("YamlLoader.get()", e.what());
            terminate::finish();
        }        
    }

    template <>
    std::vector<std::string> YamlLoader::get<
        std::vector<std::string>
    >(const std::string& root, const std::string& key);

};

namespace YAML {
template<>
struct convert<peer::Node> {
  static Node encode(const peer::Node& rhs) {
    Node node;
    node.push_back(rhs.getIP());
    node.push_back(rhs.getPublicKey());
    return node;
  }

  static bool decode(const Node& node, peer::Node& rhs) {
    if(!node.IsMap()) {
      return false;
    }
    rhs.ip = node["ip"].as<std::string>();
    rhs.publicKey = node["publicKey"].as<std::string>();
    return true;
  }
};
}