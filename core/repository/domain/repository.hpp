#ifndef __CORE_REPOSITORY_DOMAIN_SAMPLE_ASSET_REPOSITORY_HPP__
#define __CORE_REPOSITORY_DOMAIN_SAMPLE_ASSET_REPOSITORY_HPP__

#include <string>
#include <memory>
#include <vector>

#include "../../domain/sample_asset.hpp"

namespace domain_repository {
    
    template<typename T>
    bool add(std::string key,T value);
    
    template<typename T>
    bool update(std::string key,T newValue);

    // This is OK...??
    template<typename T>
    bool remove(std::string key);
    
    template<typename T>
    std::vector<std::unique_ptr<T> > findAll(std::string key);

    template<typename T>
    std::unique_ptr<T> findOne(std::string key);
    
    template<typename T>
    std::unique_ptr<T> findOrElse(std::string key, T defaultVale);

    template<typename T>
    bool isExist(std::string key);

};
#endif  // CORE_REPOSITORY_MERKLETRANSACTIONREPOSITORY_HPP_
