#ifndef CORE_DOMAIN_ACCOUNT_HPP_
#define CORE_DOMAIN_ACCOUNT_HPP_

#include <string>
#include <vector>

#include <msgpack.hpp>

#include "asset.hpp"
#include "../util/random.hpp"

#include <vector>

namespace domain{

  // This is domain.
  class AccountUser {
      std::string name;
      std::string publicKeyb64Encoded;
      std::string uid;
    public:

      // Use only factory.
      AccountUser(
        std::string aName,
        std::string aPublicKeyb64Encoded
      ):
        name(aName),
        uid(random_service::makeRandomHash()),
        publicKeyb64Encoded(aPublicKeyb64Encoded)
      {}

      // Support move and copy.
      AccountUser(AccountUser const&) = default;
      AccountUser(AccountUser&&) = default;
      AccountUser& operator =(AccountUser const&) = default;
      AccountUser& operator =(AccountUser&&) = default;

      // Account user can create a domain and has some a domain;
      bool createDomain(std::string);
      bool isOwnerOfDomin(std::string domainName);

      // Account User has some an asset.
      bool hasAssetInfo(std::string assetUrl);
      // Account user can add an asset to the domain if account user has this domain;
      bool joinAssetTo(Asset asset, std::string domain);

      // Account user can pay other user in any asset;
      // assetUrl := domain::domain::asset.
      // Of cause, account balance must not minus.
      bool pay(std::string to, int quantity, std::string assetUrl);
      
  };
}
#endif  // CORE_DOMAIN_ACCOUNT_HPP_
