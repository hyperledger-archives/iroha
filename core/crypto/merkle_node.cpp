#include "merkle_node.hpp"

#include "hash.hpp"
#include "../repository/transaction_repository.hpp"

/**
* Implementation of a binary Merkle tree node;
*/
namespace merkle_node {
std::string hash;
std::string pairedNode;
std::string parent;
std::vector<std::string> children;
}  // namespace merkle_node
