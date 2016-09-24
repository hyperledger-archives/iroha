#include <string>
#include <vector>

#include "../account.hpp"
#include "../asset.hpp"
#include "../../util/random.hpp"

// Include asset repository
#include "../../../repository/domain/domain_repository.hpp"
#include "../../../repository/domain/account_repository.hpp"

// Include asset publisher
#include "../../../publisher/asset_publisher.hpp"

#include <vector>

namespace domain{

    // Umm, this should return whether resister failed or already exists?
    // Personally, this should only return whether resister failed or not.
    // この関数は登録失敗か既に存在しているかの状態を伝えたほうが良い？
    // 個人的には登録したかどうかを返すのがこの関数の責務だと思うが。
    bool AccountUser::registerDomain(const std::string &domainName) {
        if(domain_sample_asset_repository::alreadyExists(domainName)) {
            return domain_asset_repository::register(
                this->uid,
                domainName
            );
        }else{
            return false;
        }
    }

    bool AccountUser::isOwnerOfDomain(const std::string &domainName) {
        // not exist == domain nobody has
        if(domain_sample_asset_repository::aleadyExists(domainName)){
            for(auto& domain : this->hasDomainNames){
                if(domain == domainName){
                     return true;
                }
            }   
            return false;
        }else{
            return false;
        }
    }

    // Umm, this should return whether resister failed or already exists?
    // Personally, this should only return whether resister failed or not.
    // 上と同じく。
    bool joinSampleAssetTo(const asset::SampleAsset &sampleAsset, const std::string &domain) {
        if(!domain_sample_asset_repository::thisAssetIsInDomain(asset.name, domain)) {
            return domain_sample_asset_repository::join( asset, domain);
        }else{
            return false;
        }
    }

    bool pay(const std::string &to,const int quantity,const std::string &assetUrl) {
        std::unique_ptr<AccounuUser> receiverAccount = account_repository::findByUid(to);
        if(account != nullptr){
            // WIP
            std::string assetName = url_service::getAssetNameFromUrl(assetUrl);
            int myBalance = this->sampleAssetQuantitiesWhatIHaveAccount.at(assetName);
            int receiverBalance = receiverAccount->sampleAssetQuantitiesWhatIHaveAccount.at(assetName);
            if(myBalance > quantity){

                account_repository::update_quantity( 
                    receiverAccount->uid,
                    receiverBalance + quantity,
                    assetName
                );

                account_repository::update_quantity( 
                    this->uid,
                    myBalance - quantity,
                    assetName
                );      

                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
      
}