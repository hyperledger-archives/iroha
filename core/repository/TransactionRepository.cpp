#include "TransactionRepository.hpp"

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

#include <string>
#include <memory>
#include <iostream>

#include <msgpack.hpp>
#include "../util/Logger.hpp"
#include "../crypto/Merkle.hpp"

namespace TransactionRepository {

  std::shared_ptr<leveldb::DB> db;

  bool printStatus(leveldb::Status const status) {
    if (!status.ok()) {
      logger(status.ToString());
      return false;
    }
    return true;
  }

  AbstractTransaction convertEntity(std::string buffer) {
    Entity entity;
    msgpack::object_handle oh = msgpack::unpack(buffer.data(), buffer.size());
    msgpack::object obj = oh.get();
    obj.convert(entity);
    return entity;
  }

  std::string convertBuffer(AbstractTransaction const abstractTransaction) {
    msgpack::sbuffer buf;
    msgpack::pack(buf, entity);
    return buf.data();
  }

  void loadDb() {
    leveldb::DB* tmpDb;
    leveldb::Options options;
    options.create_if_missing = true;
    printStatus(leveldb::DB::Open(options, "/tmp/irohaTransactionDB", &tmpDb));
    db.reset(tmpDb);
  }

  bool add(std::string hash, AbstractTransaction tx) {
    if (db == nullptr) loadDb();
    return printStatus(
      db->Put(leveldb::WriteOptions(), hash, convertBuffer(entity)));
  }

  bool remove(std::string hash) {
    if (db == nullptr) loadDb();
    return printStatus(db->Delete(leveldb::WriteOptions(), hash));
  }

bool update(std::string hash, AbstractTransaction tx) {
    if (db == nullptr){
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
    if (db == nullptr) {
      loadDb();
    }
    AbstractTransaction value;
    std::string readData;
    printStatus(db->Get(leveldb::ReadOptions(), hash, &readData));
    return convertEntity(readData);
  }

}  // namespace TransactionRepository
