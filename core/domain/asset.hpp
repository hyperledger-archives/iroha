#ifndef __CORE_DOMAIN_ASSET_HPP_
#define __CORE_DOMAIN_ASSET_HPP_

#include <string>

class Asset {
    std::string name;
    std::string parentDomainName;
    int maxQuantity;
 public:

    // Constructor is only used by factory.
    Asset(
      std::string aName,
      std::string aParentDomainName,
      int aMaxQuantity
    ):
      name(aName),
      parentDomainName(aParentDomainName),
      maxQuantity(aMaxQuantity)
    {}

    // Write asset's logic
};

#endif
