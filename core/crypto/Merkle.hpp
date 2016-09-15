#ifndef CORE_CRYPTO_MERKLE_HPP_
#define CORE_CRYPTO_MERKLE_HPP_

namespace Merkle {
    class MerkleRoot {
        public:
            std::string rootNodeHash;
            std::vector<std::string> signatures;

        MerkleRoot(std::string hash, std::vector<std::string> sigs):
        rootNodeHash(hash),
        signatures(sigs)
        {}
    };

    void addLeaf(std:string leaf, std::vector<std::string> rootSignatures);
    void recomputeFromRepository();
}  // namespace Merkle

#endif  // CORE_CRYPTO_MERKLE_HPP_