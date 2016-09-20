#include "transfer_transaction.hpp"
#include "abstract_transfer_transaction.hpp"
#include <string>

namespace transfer_transaction {
class TransferTransaction : public AbstractTransaction {
    std::string hash;
    AbstractTransaction::TransactionType type;
};

};  // namespace transfer_transaction
