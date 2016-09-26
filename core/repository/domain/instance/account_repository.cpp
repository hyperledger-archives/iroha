
#include "../../../model/account.hpp"
#include "../account_repository.hpp"
#include "../../world_state_repository.hpp"

#include <msgpack.hpp>

namespace repository{
namespace account_repository {

    // Umm..., message pack is in infra....
    // wrong knowledge range? 
    // I think message pack is some as std::string. Do you think?
    std::unique_ptr<domain::AccountUser> convertAccountUser(const std::string &buffer) {
        std::unique_ptr<domain::AccountUser>au;
        msgpack::object_handle unpacked = msgpack::unpack(buffer.data(), buffer.size());
        msgpack::object obj = unpacked.get();
        obj.convert(au);
        return std::move(au);
    }
    std::string convertBuffer(const std::unique_ptr<domain::AccountUser>& au) {
        msgpack::sbuffer buf;
        msgpack::pack(buf, au);
        return std::move(buf.data());
    }

    // SampleAsset has only quantity no logic, so this value is int.
    bool update_quantity( 
        std::string accountUid,
        int newValue,
        std::string assetName) {
        std::unique_ptr<domain::AccountUser> accountUser = convertAccountUser(
            world_state_repository::find(accountUid)
        );
        if(
            accountUser->sampleAssetQuantitiesWhatIHaveAccount.find(assetName) != 
            accountUser->sampleAssetQuantitiesWhatIHaveAccount.end()
        ){
            accountUser->sampleAssetQuantitiesWhatIHaveAccount.at(assetName) = newValue;
            return world_state_repository::update(accountUid, convertBuffer(std::move(accountUser)));
        } else {
            return false;
        }
    }

    std::unique_ptr<domain::AccountUser> findByUid(std::string accountUid) {
        return convertAccountUser(
            world_state_repository::find(accountUid)
        );
    }
    
    bool add_my_domain(
        const std::string& accountUid,
        const std::string& domainName
    ){
        std::unique_ptr<domain::AccountUser> accountUser = convertAccountUser(
            world_state_repository::find(accountUid)
        );
        accountUser->hasDomainNames.push_back(domainName);
        return world_state_repository::update(accountUid, convertBuffer(std::move(accountUser)));
    }

};
};