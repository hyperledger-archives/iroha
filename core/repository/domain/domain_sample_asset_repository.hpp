#ifndef __CORE_REPOSITORY_DOMAIN_SAMPLE_ASSET_REPOSITORY_HPP__
#define __CORE_REPOSITORY_DOMAIN_SAMPLE_ASSET_REPOSITORY_HPP__

#include <string>
#include <memory>
#include <vector>

#include "../../model/sample_asset.hpp"

// I know only 'domain' and 'asset'.
namespace domain_sample_asset_repository {
    
    bool alreadyExists(std::string domainName);
    
    bool registerDomain(
        std::string accountUid,
        std::string domainName
    );

    bool thisAssetIsInDomain(
        std::string assetName,
        std::string domainName
    );

    bool join(
        const domain::asset::SampleAsset& asset,
        std::string domainName
    );
};
#endif  // CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
