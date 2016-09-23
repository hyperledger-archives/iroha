#ifndef __CORE_DOMAIN_ASSET_HPP_
#define __CORE_DOMAIN_ASSET_HPP_

#include <json.hpp>

class Asset {
 public:
  
  int getBalance(std::string accountName);

  std::vector<std::string> getDomainName();
  std::vector<std::string> getAssetNameList();

  nlohmann::json getAssetInfo();
  nlohmann::json getDomainInfo();

};

#endif
