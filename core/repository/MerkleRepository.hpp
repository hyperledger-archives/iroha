#ifndef CORE_REPOSITORY_MERKLEREPOSITORY_HPP_
#define CORE_REPOSITORY_MERKLEREPOSITORY_HPP_

#include <string>
#include <msgpack.hpp>
#include "Entity.hpp"

namespace MerkleRepository {
  bool add(std::string uuid, Entity e);
  bool update(std::string uuid, Entity e);
  bool remove(std::string uuid);

  Entity find(std::string uuid);
};

#endif  // CORE_REPOSITORY_MERKLEREPOSITORY_HPP_
