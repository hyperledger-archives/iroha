
#include "../../../repository/world_state_repository.hpp"
#include "../../../util/exception.hpp"

#include "../../../util/logger.hpp"

#include <leveldb/write_batch.h>
#include <leveldb/db.h>


namespace repository{

  // Level DB is known only to me.
  namespace world_state_repository {

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
          options.error_if_exists = false;        
          options.create_if_missing = true;
          loggerStatus(leveldb::DB::Open(options, "/tmp/irohaDB", &tmpDb)); //TODO: This path should be configurable
          db.reset(tmpDb);
      }

      bool add(const std::string& key, const std::string& value){
          return loggerStatus( db->Put(leveldb::WriteOptions(), key, value));
      }

      bool update(const std::string& key, const std::string& value){
          std::string dummy;
          if( loggerStatus(db->Get(leveldb::ReadOptions(), key, &dummy)) ){
              leveldb::WriteBatch batch;
              batch.Delete(key);
              batch.Put(key, value);
              return loggerStatus( db->Write(leveldb::WriteOptions(), &batch));
          }
      }

      bool remove(const std::string& key){
          return loggerStatus(db->Delete(leveldb::WriteOptions(), key));
      }

      std::string find(const std::string& key){
          std::string readData;
          loggerStatus(db->Get(leveldb::ReadOptions(), key, &readData));
          if(readData != ""){
              return readData;
          } else {
              return "";
          }
      }

      std::string findOrElse(
        const std::string& key,
        const std::string& defaultValue
      ) {
          std::string result = "";
          loggerStatus(db->Get(leveldb::ReadOptions(), key, &result));          
          if(result == "") {
              return defaultValue;
          } else {
              return result;
          }
      }

      bool isExist(const std::string& key){
          std::string result = "";
          loggerStatus(db->Get(leveldb::ReadOptions(), key, &result));          
          if(result == "") {
              return false;
          } else {
              return true;
          }
      }
      
  };

};
