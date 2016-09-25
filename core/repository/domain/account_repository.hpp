#ifndef __CORE_REPOSITORY_DOMAIN_ACCOUNT_ASSET_REPOSITORY_HPP__
#define __CORE_REPOSITORY_DOMAIN_ACCOUNT_ASSET_REPOSITORY_HPP__

#include <string>
#include <memory>
#include <vector>

#include "../../domain/account.hpp"

namespace account_repository {

    // SampleAsset has only quantity no logic, so this value is int.
    bool update_quantity( 
        std::string accountUid,
        int newValue,
        std::string assetName);

    std::unique_ptr<domain::AccountUser> findByUid(std::string);
    
    add_my_domain(
        const std::string& accountUid,
        const std::string& domainName
    );
    
};
#endif  // CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
