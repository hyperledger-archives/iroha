#ifndef CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_
#define CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_

#include <string>
#include <msgpack.hpp>
#include "../domain/transactions/AbstractTransaction.hpp"

namespace UnconfirmedTransactionRepository {
  bool add(std::string hash, AbstractTransaction tx);
  bool update(std::string hash, AbstractTransaction tx);
  bool remove(std::string hash);

  AbstractTransaction find(std::string hash);
};

#endif  // CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_
