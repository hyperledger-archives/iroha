#ifndef CORE_CRYPTO_MERKLE_HPP_
#define CORE_CRYPTO_MERKLE_HPP_

namespace merkle {
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
}  // namespace merkle

#endif  // CORE_CRYPTO_MERKLE_HPP_
