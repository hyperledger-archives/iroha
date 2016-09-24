#ifndef __CORE_DOMAIN_ASSET_HPP_
#define __CORE_DOMAIN_ASSET_HPP_

#include "asset.hpp"

namespace domain {

    namespace asset {

        class SampleAsset : public Asset {
          public:
            int maxQuantity;

            SampleAsset(
              std::string aName,
              std::string aParentDomainName,
              int aMaxQuantity
            ):
              Asset(aName, aParentDomainName),
              maxQuantity(aMaxQuantity)
            {}

        };

    }  // namespace asset
    
}  // namespace domain

#endif
