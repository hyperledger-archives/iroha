#ifndef CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_
#define CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_

#include <string>
#include <msgpack.hpp>
#include "../domain/transactions/abstract_transaction.hpp"

namespace unconfirmed_transaction_repository {
  bool add(std::string hash, AbstractTransaction tx);
  bool update(std::string hash, AbstractTransaction tx);
  bool remove(std::string hash);

  AbstractTransaction find(std::string hash);
};  // namespace unconfirmed_transaction_repository

#endif  // CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_
