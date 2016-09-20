#ifndef CORE_REPOSITORY_TRANSACTIONREPOSITORY_HPP_
#define CORE_REPOSITORY_TRANSACTIONREPOSITORY_HPP_

#include <string>
#include <msgpack.hpp>
#include "../domain/transactions/AbstractTransaction.hpp"

namespace TransactionRepository {
  bool add(std::string hash, AbstractTrannsaction tx);
  bool update(std::string hash, AbstractTrannsaction tx);
  bool remove(std::string hash);

  AbstractTrannsaction find(std::string hash);
};

#endif  // CORE_REPOSITORY_TRANSACTIONREPOSITORY_HPP_
