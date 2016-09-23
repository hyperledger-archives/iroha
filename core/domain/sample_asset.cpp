#ifndef __CORE_DOMAIN_ASSET_HPP_
#define __CORE_DOMAIN_ASSET_HPP_

#include <json.hpp>

#include "sample_asset.hpp"
#include "asset.hpp"
#include "../repository/world_state_repository.hpp"

namespace asset{

class SampleAsset : public Asset {

 public:
  
  int getBalance(std::string accountName){
    // ToDo
  }

  std::vector<std::string> getDomainName(){
    // ToDo
  }

  std::vector<std::string> getAssetNameList(){
    // ToDo
  }

  nlohmann::json getAssetInfo(){
    // ToDo  
  }

  nlohmann::json getDomainInfo(){
    // ToDo
  }

};

}  // namespace asset

#endif
