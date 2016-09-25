#ifndef CORE_DOMAIN_ACCOUNT_HPP_
#define CORE_DOMAIN_ACCOUNT_HPP_

#include <string>
#include <vector>

#include <msgpack.hpp>

#include "sample_asset.hpp"
#include "../util/random.hpp"

#include <vector>

namespace domain {

  // This is domain.
  class AccountUser {
      std::string name;
      std::string publicKeyb64Encoded;
      std::string uid;

    public:
      // Account user can have some domain. 
      std::vector<std::string> hasDomainNames;

      // SampleAsset has only quantity no logic, so this value is int.
      std::unordered_map<
        std::string,
        int
      > sampleAssetQuantitiesWhatIHaveAccount;

      // This user created asset. 
      std::vector<std::string> myAssetNames;

      // Umm... This is infra range knowledge       
      MSGPACK_DEFINE( name, publicKeyb64Encoded, uid);

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

      // Account user can register a domain and has some a domain;
      bool registerDomain(std::string);
      bool isOwnerOfDomin(std::string domainName);

      // Account user can add an asset to the domain if account user has this domain;
      bool joinSampleAssetTo(asset::SampleAsset sampleAsset, std::string domain);

      // Account user can pay other user in any asset;
      // assetUrl := domain::domain::asset.
      // Of cause, account balance must not minus.
      bool pay(std::string to, int quantity, std::string assetUrl);
      
  };
}
#endif  // CORE_DOMAIN_ACCOUNT_HPP_
