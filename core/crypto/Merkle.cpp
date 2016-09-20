#include "Merkle.hpp"

#include "hash.hpp"
#include "../repository/merkle_repository.hpp"

/**
* Implementation of a binary Merkle tree （ハッシュ木）.
*/
namespace merkle {
    void addLeaf(MerkleRepository const merkleTree, std:string const leaf, std::vector<std::string> const rootSignatures) {
        
        std:::string const leafHash = Hash::sha3_256_hex(leaf);

    }

    // void recomputeAllFromRepository(TransactionRepository const txRepo) {
    //     std::vector<AbstractTransaction> currentLayer = make_unique(std::vector<AbstractTransaction>);
    //     for (std::shared_ptr<AbstractTransaction> tx : txRepo::getAllTransaction() {
    //         std:::string txHash = Hash::sha3_256_hex(tx::getRawData());
    //         currentLayer::add(txHash);
    //     }
        
    //     while (currentLayer::size() > 1) {
    //         for (int ndx = 0; ndx < currentLayer::size() - 1; ndx+=2) {
    //             Hash::sha3_256_hex(currentLayer[ndx], currentLayer[ndx+1]);
    //         }
    //     }
    // }
}  // namespace merkle
