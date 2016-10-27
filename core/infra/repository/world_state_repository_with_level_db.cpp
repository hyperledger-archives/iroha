/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "../../../repository/world_state_repository.hpp"
#include "../../../util/exception.hpp"

#include "../../../util/logger.hpp"

#include <leveldb/write_batch.h>
#include <leveldb/db.h>
#include <tuple>

// +------------------------------------------------+
// | Repository should save string to any database. |
// +------------------------------------------------+
// |                                                |
// | I know ...                                     |
// |  - leveldb                                     |
// |                                                |
// | I don't know                                   |
// |  - json formats or data model                  |
// |                                                |
// +------------------------------------------------+
namespace repository {

  // Level DB is known only to me.
  namespace world_state_repository {

      namespace detail {

              static std::unique_ptr<leveldb::DB> db = nullptr;

              bool loggerStatus(leveldb::Status const status) {
                  if (!status.ok()) {
                      logger::info(__FILE__, status.ToString());
                      return false;
                  }
                  return true;
              }

              void loadDb() {
                  leveldb::DB *tmpDb;
                  leveldb::Options options;
                  options.error_if_exists = false;
                  options.create_if_missing = true;
                  loggerStatus(leveldb::DB::Open(options, "/tmp/irohaDB", &tmpDb)); //TODO: This path should be configurable
                  db.reset(tmpDb);
              }
          }

      bool add(const std::string &key, const std::string &value) {
          if (nullptr == detail::db) {
              detail::loadDb();
          }

          return detail::loggerStatus(detail::db->Put(leveldb::WriteOptions(), key, value));
      }

      bool addBatch(const std::vector<std::tuple<T, T>> &tuples) {
          if (nullptr == detail::db) {
              detail::loadDb();
          }

          leveldb::WriteBatch batch;

          for (const tuple : tuples) {
              batch.Put(std::get<0>(tuple), std::get<1>(tuple));
          }

          return detail::loggerStatus(detail::db->Write(leveldb::WriteOptions(), &batch));
      }

      bool update(const std::string &key, const std::string &value) {
          if (nullptr == detail::db) {
              detail::loadDb();
          }

          std::string dummy;
          if (detail::loggerStatus(detail::db->Get(leveldb::ReadOptions(), key, &dummy))) {
              leveldb::WriteBatch batch;
              batch.Delete(key);
              batch.Put(key, value);
              return detail::loggerStatus(detail::db->Write(leveldb::WriteOptions(), &batch));
          }
          return false;
      }

      bool remove(const std::string &key) {
          if (nullptr == detail::db) {
              detail::loadDb();
          }
          return detail::loggerStatus(detail::db->Delete(leveldb::WriteOptions(), key));
      }

      std::string find(const std::string &key) {
          if (nullptr == detail::db) {
              detail::loadDb();
          }

          std::string readData;
          detail::loggerStatus(detail::db->Get(leveldb::ReadOptions(), key, &readData));
          if (readData != "") {
              return readData;
          } else {
              return "";
          }
      }

      std::string findOrElse(
              const std::string &key,
              const std::string &defaultValue
      ) {
          if (nullptr == detail::db) {
              detail::loadDb();
          }

          std::string result = "";
          detail::loggerStatus(detail::db->Get(leveldb::ReadOptions(), key, &result));
          if (result == "") {
              return defaultValue;
          } else {
              return result;
          }
      }

      bool exists(const std::string &key) {
          if (nullptr == detail::db) {
              detail::loadDb();
          }

          std::string result = "";
          detail::loggerStatus(detail::db->Get(leveldb::ReadOptions(), key, &result));
          return result == "";
      }

      unsigned long long recordCount() {
          if (nullptr == detail::db) {
              detail::loadDb();
          }
          
          std::string result = "";
          detail::loggerStatus(detail::db->Get(leveldb::ReadOptions(), "lastOrder", &result));
          return result != "" ? std::strtoull(result) : 0;
      }
  };
};
