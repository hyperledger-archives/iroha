#include "merkle_transaction_repository.hpp"

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

#include <string>
#include <memory>
#include <iostream>

#include <msgpack.hpp>
#include "../util/logger.hpp"
#include "../crypto/merkle_node.hpp"

namespace merkle_transaction_repository {

using abs_tx = abstract_transaction::AbstractTransaction;

std::shared_ptr<leveldb::DB> db;
std::unique_ptr<merkle::MerkleRoot> merkle;

bool printStatus(leveldb::Status const status) {
  if (!status.ok()) {
    logger::info("merkle_transaction",status.ToString());
      return false;
  }
  return true;
}

std::unique_ptr<abs_tx> convertTransaction(std::string const buffer) {
  std::unique_ptr<abs_tx> tx;
  msgpack::object_handle unpacked = msgpack::unpack(buffer.data(), buffer.size());
  msgpack::object obj = unpacked.get();
  obj.convert(tx);
  return tx;
}
                                                       
std::string convertBuffer(std::unique_ptr<abs_tx> tx) {
  msgpack::sbuffer buf;
  msgpack::pack(buf, tx);
  return buf.data();
}

void loadDb() {
  leveldb::DB* tmpDb;
  leveldb::Options options;
  options.create_if_missing = true;
  printStatus(leveldb::DB::Open(options, "/tmp/irohaTransactionDB", &tmpDb)); //TODO: This path should be configurable
  db.reset(tmpDb);
}

bool commit(std::unique_ptr<ConsensusEvent::ConsensusEvent> event) {
  if (nullptr == db) {
    loadDb();
  }
  std::vector<std::string> const merkleRootSignatures = std::move(event->merkleRootSignatures);
  //TODO: store the 2f+1 nodes' signatures for the merkle root

  // Update Merkle tree
  std::string const rawData = convertBuffer(std::move(event->tx));
  merkle::addLeaf(rawData);
  return printStatus(db->Put(leveldb::WriteOptions(), hash, rawData));
}

std::unique_ptr<abs_tx> findLeaf(std::string const hash) {
  if (nullptr == db) {
    loadDb();
  }
  std::string readData;
  printStatus(db->Get(leveldb::ReadOptions(), hash, &readData));
  return convertTransaction(readData);
}
};  // namespace merkle_transaction_repository
