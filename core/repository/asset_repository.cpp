#include "asset_repository.hpp"

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

#include <string>
#include <memory>
#include <iostream>

#include <msgpack.hpp>
#include "../util/logger.hpp"

namespace asset_repository {
std::shared_ptr<leveldb::DB> db;

bool printStatus(leveldb::Status const status) {
  if (!status.ok()) {
    logger(status.ToString());
      return false;
  }
  return true;
}

void loadDb() {
  leveldb::DB* tmpDb;
  leveldb::Options options;
  options.create_if_missing = true;
  printStatus(leveldb::DB::Open(options, "/tmp/irohaAssetDB", &tmpDb)); //TODO: This path should be configurable
  db.reset(tmpDb);
}

bool updateAccountState(std::string sendingAccount, std::string receivingAccount, std::string assetName, long long quantity) {
  if (nullptr == db) {
    loadDb();
  }
  bool sendingStatus = true;
  bool receivingStatus = true;

  if (nullptr != sendingAccount) {
    long long newBalance = getBalance() - quantity;
    // TODO: add error checking here negative balances, etc.
    sendingStatus = printStatus(db->Put(leveldb::WriteOptions(), sendingAccount + "_" + assetName, newBalance));
  }

  if (nullptr != receivingAccount) {
    long long newBalance = getBalance() + quantity;
    receivingStatus = printStatus(db->Put(leveldb::WriteOptions(), receivingAccount + "_" + assetName, newBalance));
  }
  
  return sendingStatus && receivingStatus;
}

long long getBalance(std::string const account, std::string const assetName) {
  if (nullptr == db) {
    loadDb();
  }

  long long readData;
  printStatus(db->Get(leveldb::ReadOptions(), account + "_" + assetName, &readData));
  return readData; // TODO: cast to long long, or can we get that directly from leveldb?
}
};  // namespace asset_repository
