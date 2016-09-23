#ifndef __CORE_DOMAIN_ASSET_HPP_
#define __CORE_DOMAIN_ASSET_HPP_

#include <json.hpp>
#include "asset.hpp"


namespace asset {

class SampleAsset : public Asset {
 public:
  
  int getBalance(std::string accountName);

  std::vector<std::string> getDomainName();
  std::vector<std::string> getAssetNameList();

  nlohmann::json getAssetInfo();
  nlohmann::json getDomainInfo();

  bool updateAccountState(
    std::string sendingAccount,
    std::string receivingAccount,
    std::string assetName,
    long long int quantity
  );

};

}  // namespace asset

#endif
