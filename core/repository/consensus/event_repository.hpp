#ifndef CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_
#define CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_

#include <string>
#include "../../model/transactions/abstract_transaction.hpp"

namespace repository{
namespace event {
  bool add(std::string hash, const abstract_transaction::AbstractTransaction& tx);
  bool update(std::string hash, const abstract_transaction::AbstractTransaction& tx);
  bool remove(std::string hash);
  bool empty();
  std::vector<
    std::unique_ptr<abstract_transaction::AbstractTransaction>
  > findAll();
  std::unique_ptr<abstract_transaction::AbstractTransaction>& find(std::string hash);
};  // namespace unconfirmed_transaction_repository
};
#endif  // CORE_REPOSITORY_UNCONFIRMEDTRANSACTIONREPOSITORY_HPP_
