#ifndef __REPOSITORY__
#define __REPOSITORY__

#include <string>
#include <iostream>
#include <memory>

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

template<typename T>
class Repository{
  leveldb::DB* db;
public:
  Repository();
  ~Repository();
  bool add(std::string key,T value);
  bool update(std::string key,T value);
  bool remove(std::string key);

  std::shared_ptr<T> find(std::string key);
};

bool printStatus(leveldb::Status status){
  if(!status.ok()){
    std::cerr << status.ToString() << std::endl;
    return false;
  }
  return true;
}

template<typename T>
Repository<T>::Repository(){
  leveldb::Options options;
  options.create_if_missing = true;
  printStatus(leveldb::DB::Open(options, "/tmp/testdb", &db));
}

template<typename T>
Repository<T>::~Repository(){
  delete db;
}

template<typename T>
bool Repository<T>::add(std::string key,T value){
  return printStatus(db->Put(leveldb::WriteOptions(), key, value));
}

template<typename T>
bool Repository<T>::remove(std::string key){
   return printStatus(db->Delete(leveldb::WriteOptions(), key));
}

template<typename T>
bool Repository<T>::update(std::string key,T value){
  T tmpValue;
  if(printStatus(db->Get(leveldb::ReadOptions(), key, &tmpValue))) {
    leveldb::WriteBatch batch;
    batch.Delete(key);
    batch.Put(key, value);
    return printStatus(db->Write(leveldb::WriteOptions(), &batch));
  }
  return false;
}

template<typename T>
std::shared_ptr<T> Repository<T>::find(std::string key){
  T value;
  printStatus(db->Get(leveldb::ReadOptions(), key, &value));
  return std::make_shared<T>(value);
}

#endif
