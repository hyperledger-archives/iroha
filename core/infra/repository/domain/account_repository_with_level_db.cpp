
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <msgpack.hpp>

#include <string>
#include <memory>
#include <iostream>

#include "../../../util/logger.hpp"
#include "../../../domain/account.hpp"

namespace account_repository {

    std::unique_ptr<leveldb::DB> db;

    bool loggerStatus(leveldb::Status const status) {
        if (!status.ok()) {
            logger::info("merkle_transaction",status.ToString());
            return false;
        }
        return true;
    }

    void loadDb() {
        leveldb::DB* tmpDb;
        leveldb::Options options;
        options.create_if_missing = true;
        loggerStatus(leveldb::DB::Open(options, "/tmp/irohaDB", &tmpDb)); //TODO: This path should be configurable
        db.reset(tmpDb);
    }

    std::unique_ptr<domain::AccountUser> convertAccountUser(const std::string &buffer) {
        std::unique_ptr<domain::AccountUser> au;
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

    std::unique_ptr<domain::AccountUser> findByUid(std::string accountUid){
        std::string readData;
        loggerStatus(db->Get(leveldb::ReadOptions(), accountUid, &readData));
        if(readData != ""){
            return convertAccountUser(std::move(readData));
        }else{
            return nullptr;
        }
    }

    // This don't validate whether already exists or not.
    bool add_my_domain(
        const std::string& accountUid,
        const std::string& domainName
    ) {
        if (nullptr == db) {
            loadDb();
        }
        std::unique_ptr<domain::AccountUser> accountUser = findByUid(accountUid);
        accountUser->hasDomainNames.push_back(domainName);
        return loggerStatus(db->Put(leveldb::WriteOptions(), accountUid, convertBuffer(accountUser)));
    }
    
    // SampleAsset has only quantity no logic, so this value is int.
    bool update_quantity( 
        std::string accountUid,
        int newValue,
        std::string assetName
    ) {
        if (nullptr == db) {
            loadDb();
        }

        std::unique_ptr<domain::AccountUser> accountUser = findByUid(accountUid);
        if(
            accountUser->sampleAssetQuantitiesWhatIHaveAccount.find(assetName) != 
            accountUser->sampleAssetQuantitiesWhatIHaveAccount.end()
        ){
            accountUser->sampleAssetQuantitiesWhatIHaveAccount.at(assetName) = newValue;
            return loggerStatus(db->Put(leveldb::WriteOptions(), accountUid, convertBuffer(accountUser)));
        } else {
            return false;
        }
    }

};