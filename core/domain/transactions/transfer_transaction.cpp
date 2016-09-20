#include "TransferTransaction.hpp"
#include "AbstractTransferTransaction.hpp"
#include <string>

namespace TransferTransaction {
class TransferTransaction : public AbstractTransaction {
    std::string hash;
    AbstractTransaction::TransactionType type;
};

};  // namespace TransferTransaction
