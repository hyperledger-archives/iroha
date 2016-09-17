#include "TransactionRepository.hpp"

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

#include <string>
#include <memory>
#include <iostream>

#include <msgpack.hpp>
#include "../util/Logger.hpp"
#include "../crypto/Merkle.hpp"

namespace UnconfirmedTransactionRepository {

  std::shared_ptr<leveldb::DB> db;

  bool printStatus(leveldb::Status const status) {
    if (!status.ok()) {
      logger(status.ToString());
      return false;
    }
    return true;
  }

  AbstractTransaction convertTransaction(std::string const buffer) {
    AbstractTransaction tx;
    msgpack::object_handle unpacked = msgpack::unpack(buffer.data(), buffer.size());
    msgpack::object obj = unpacked.get();
    obj.convert(tx);
    return tx;
  }

  std::string convertBuffer(AbstractTransaction const tx) {
    msgpack::sbuffer buf;
    msgpack::pack(buf, tx);
    return buf.data();
  }

  void loadDb() {
    leveldb::DB* tmpDb;
    leveldb::Options options;
    options.create_if_missing = true;
    printStatus(leveldb::DB::Open(options, "/tmp/irohaUnconfirmedTxCache", &tmpDb)); //TODO: This path should be configurable
    db.reset(tmpDb);
  }

  bool add(std::string hash, AbstractTransaction const tx) {
    if (nullptr == db) {
      loadDb();
    }
    return printStatus(db->Put(leveldb::WriteOptions(), hash, convertBuffer(tx)));
  }

  bool remove(std::string hash) {
    if (nullptr == db) {
      loadDb();
    }
    return printStatus(db->Delete(leveldb::WriteOptions(), hash));
  }

bool update(std::string hash, AbstractTransaction const tx) {
    if (nullptr == db) {
      loadDb();
    }
    std::string tmpValue;
    if (printStatus(db->Get(leveldb::ReadOptions(), hash, &tmpValue))) {
      leveldb::WriteBatch batch;
      batch.Delete(hash);
      batch.Put(hash, convertBuffer(entity));

      return printStatus(db->Write(leveldb::WriteOptions(), &batch));
    }
    return false;
  }

  AbstractTransaction find(std::string hash) {
    if (nullptr == db) {
      loadDb();
    }
    AbstractTransaction value;
    std::string readData;
    printStatus(db->Get(leveldb::ReadOptions(), hash, &readData));
    return convertTransaction(readData);
  }

}  // namespace UnconfirmedTransactionRepository
