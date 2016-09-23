#ifndef CORE_CRYPTO_MERKLE_HPP_
#define CORE_CRYPTO_MERKLE_HPP_

namespace merkle {

  struct MerkleNode{
    std::string hash;
    std::string pairedNode;
    std::string parent;
    std::vector<std::string> children;
  };

  struct MerkleRoot {
    std::string rootNodeHash;
    std::vector<std::string> signatures;

    MerkleRoot(std::string hash, std::vector<std::string> sigs):
      rootNodeHash(hash),
      signatures(sigs)
    {}
  };

    void addLeaf(std::string leaf, std::vector<std::string> rootSignatures);
}  // namespace merkle

#endif  // CORE_CRYPTO_MERKLE_HPP_
