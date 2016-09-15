#include "Merkle.hpp"

#include "Hash.hpp"
#include "../repository/TransactionRepository.hpp"

/**
* Implementation of a binary Merkle tree （ハッシュ木）.
*/
namespace Merkle {

    void addLeaf(std:string leaf, std::vector<std::string> rootSignatures) {
        std:::string = Hash::sha3_256_hex();
    }

    void recomputeFromRepository(TransactionRepository txRepo) {
        for (std::shared_ptr<AbstractTransaction> tx : txRepo::getAllTransaction() {
            std:::string = Hash::sha3_256_hex(tx::getRawData());
        }
    }
}  // namespace Merkle