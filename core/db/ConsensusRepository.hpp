#ifndef __CONSENSUS_REPOSITORY__
#define __CONSENSUS_REPOSITORY__

#include "repository.hpp"
#include "../crypto/hash.hpp"

#include <string>
#include <iostream>
#include <memory>

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

static const std::string ALL_TEMPORARY_TRANSACTIONS = "ALL_TEMPORARY_TRANSACTIONS";
static const std::string LATEST_TRANSACTIONS = "LATEST_TRANSACTIONS";
static const std::string ALL_CONFIRM_TRANSACTIONS = "ALL_CONFIRM_TRANSACTIONS";
static const std::string LATEST_BLOCK_HASH = "LATEST_BLOCK_HASH";

class ConsensusRepository : public Repository<std::string>{
  leveldb::DB* db;
public:
  ConsensusRepository();
  ~ConsensusRepository();
  std::shared_ptr<std::string> findTemporaryAllTxData();
  std::shared_ptr<std::string> findAllTxData();
  bool updateTemporaryTx(std::string transaction);
  bool discordTxData();

  bool confirmTx();
  bool addBlock();

  std::string getLatestHash();
};

ConsensusRepository::ConsensusRepository(){
  leveldb::Options options;
  options.create_if_missing = true;
  printStatus(leveldb::DB::Open(options, "/tmp/testdb", &db));
}

ConsensusRepository::~ConsensusRepository(){
  delete db;
}

std::shared_ptr<std::string> ConsensusRepository::findTemporaryAllTxData(){
  std::string value;
  printStatus(db->Get(leveldb::ReadOptions(), ALL_TEMPORARY_TRANSACTIONS, &value));
  return std::shared_ptr<std::string>(new std::string(value));
}

bool ConsensusRepository::discordTxData(){
   return printStatus(db->Delete(leveldb::WriteOptions(), ALL_TEMPORARY_TRANSACTIONS));
}

bool ConsensusRepository::updateTemporaryTx(std::string transaction){
  std::string tmpValue;
  if(printStatus(db->Get(leveldb::ReadOptions(), ALL_TEMPORARY_TRANSACTIONS, &tmpValue))) {
    leveldb::WriteBatch batch;
    batch.Delete(ALL_TEMPORARY_TRANSACTIONS);

    tmpValue = tmpValue + "," + transaction;

    batch.Put(ALL_TEMPORARY_TRANSACTIONS, tmpValue);
    return printStatus(db->Write(leveldb::WriteOptions(), &batch));
  }
  return false;
}

bool ConsensusRepository::confirmTx(){
  std::string temporaryTx, confirmedTx, latestTx;
  if(printStatus(db->Get(leveldb::ReadOptions(), ALL_TEMPORARY_TRANSACTIONS, &temporaryTx))){
    if(printStatus(db->Get(leveldb::ReadOptions(), ALL_CONFIRM_TRANSACTIONS, &confirmedTx))){
      if(printStatus(db->Get(leveldb::ReadOptions(), LATEST_TRANSACTIONS,    &latestTx))){

        leveldb::WriteBatch batch;

        batch.Delete(ALL_TEMPORARY_TRANSACTIONS);
        batch.Delete(  ALL_CONFIRM_TRANSACTIONS);
        batch.Delete(       LATEST_TRANSACTIONS);

        confirmedTx = confirmedTx +","+ temporaryTx;
        latestTx = latestTx +","+ temporaryTx;

        batch.Put(ALL_CONFIRM_TRANSACTIONS, confirmedTx);
        batch.Put(     LATEST_TRANSACTIONS, latestTx);

        return printStatus(db->Write(leveldb::WriteOptions(), &batch));
      }
    }
  }
  return false;
}

bool ConsensusRepository::addBlock(){
  std::string latestHash, transaction;
  if(printStatus(db->Get(leveldb::ReadOptions(), LATEST_BLOCK_HASH, &latestHash))){
    if(printStatus(db->Get(leveldb::ReadOptions(), LATEST_TRANSACTIONS, &transaction))){
      leveldb::WriteBatch batch;
      batch.Delete(LATEST_BLOCK_HASH);

      // 一つ前のHashをくくりつけ新しいHashとする
      std::string txHash = hash::sha3_256_hex(transaction + latestHash);

      batch.Put(LATEST_BLOCK_HASH, txHash);
      return printStatus(db->Write(leveldb::WriteOptions(), &batch));
    }
  }
  return false;
}

std::string ConsensusRepository::getLatestHash(){
  std::string latestHash;
  printStatus(db->Get(leveldb::ReadOptions(), LATEST_BLOCK_HASH, &latestHash));
  return latestHash;
}

#endif
