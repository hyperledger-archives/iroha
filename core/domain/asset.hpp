#ifndef CORE_DOMAIN_ASSET_HPP_
#define CORE_DOMAIN_ASSET_HPP_

#include <string>

#include "../util/random.hpp"

namespace domain {
    namespace asset {

      class Asset {
        protected:
          std::string name;
          std::string parentDomainName;
          std::string uid;
        public:

          // Constructor is only used by factory.
          Asset(
            std::string aName,
            std::string aParentDomainName
          ):
            name(aName),
            uid(random_service::makeRandomHash()),
            parentDomainName(aParentDomainName)
          {}
  
          virtual ~Asset() = default;

          // Support move and copy.
          Asset(Asset const&) = default;
          Asset(Asset&&) = default;
          Asset& operator =(Asset const&) = default;
          Asset& operator =(Asset&&) = default;

      };

    };  // namespace asset
};  // namespace domain

#endif