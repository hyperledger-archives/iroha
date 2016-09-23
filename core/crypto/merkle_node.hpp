#ifndef CORE_CRYPTO_MERKLE_HPP_
#define CORE_CRYPTO_MERKLE_HPP_

namespace merkle_node {
  class MerkleNode{
    std::string hash;
    std::string pairedNode;
    std::string parent;
    std::vector<std::string> children;
  };
};  // namespace merkle_node

#endif  // CORE_CRYPTO_MERKLE_HPP_
