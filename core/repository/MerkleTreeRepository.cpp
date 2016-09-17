#include "MerkleTreeRepository.hpp"

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

#include <string>
#include <memory>
#include <iostream>

#include <msgpack.hpp>
#include "../util/Logger.hpp"
#include "../crypto/Merkle.hpp"

namespace MerkleTreeRepository {
std::shared_ptr<leveldb::DB> db;
std::unique_ptr<Merkle> merkle;

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
  printStatus(leveldb::DB::Open(options, "/tmp/irohaMerkleTree", &tmpDb)); //TODO: This path should be configurable
  db.reset(tmpDb);
}

bool commit(ConsensusEvent const event) {
  if (nullptr == db) {
    loadDb();
  }
  AbstractTransaction event->tx;
  std::vector<std::string> signatures = event->signatures;
  // Update Merkle tree
  std::string const rawData = convertBuffer(tx);
  merkle::addLeaf(db, rawData);
  return printStatus(db->Put(leveldb::WriteOptions(), hash, rawData));
}

AbstractTransaction find(std::string const hash) {
  if (nullptr == db) {
    loadDb();
  }
  AbstractTransaction value;
  std::string readData;
  printStatus(db->Get(leveldb::ReadOptions(), hash, &readData));
  return convertTransaction(readData);
}
}  // namespace MerkleTreeRepository
