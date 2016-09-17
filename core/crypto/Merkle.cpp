#include "Merkle.hpp"

#include "Hash.hpp"
#include "../repository/TransactionRepository.hpp"

/**
* Implementation of a binary Merkle tree （ハッシュ木）.
*/
namespace Merkle {

    void addLeaf(TransactionRepository const txRepo, std:string const leaf, std::vector<std::string> const rootSignatures) {
        
        std:::string const leafHash = Hash::sha3_256_hex(leaf);
    }

    void recomputeFromRepository(TransactionRepository const txRepo) {
        std::vector<AbstractTransaction> currentLayer = make_unique(std::vector<AbstractTransaction>);
        for (std::shared_ptr<AbstractTransaction> tx : txRepo::getAllTransaction() {
            std:::string = Hash::sha3_256_hex(tx::getRawData());
        }
    }
}  // namespace Merkle