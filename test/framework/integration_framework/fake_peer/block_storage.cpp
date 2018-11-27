/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "framework/integration_framework/fake_peer/block_storage.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/max_element.hpp>
#include "backend/protobuf/block.hpp"
#include "framework/integration_framework/fake_peer/fake_peer.hpp"

/// Emplace to a map.
/// @return true if overwritten.
template <typename MapType, typename KeyType, typename ValueType>
static bool emplaceCheckingOverwrite(MapType &map,
                                     const KeyType &key,
                                     const ValueType &value) {
  const auto it = map.find(key);
  if (it == map.end()) {
    map.emplace(key, value);
    return false;
  } else {
    map.emplace_hint(map.erase(it), key, value);
    return true;
  }
}

namespace integration_framework {
  namespace fake_peer {

    void BlockStorage::storeBlock(const BlockPtr &block) {
      if (emplaceCheckingOverwrite(blocks_by_height_, block->height(), block)) {
        getLogger()->warn("Overwriting block with height {}.", block->height());
      }
      if (emplaceCheckingOverwrite(blocks_by_hash_, block->hash(), block)) {
        getLogger()->warn("Overwriting block with hash {}.",
                          block->hash().toString());
      }
    }

    BlockStorage::BlockPtr BlockStorage::getBlockByHeight(
        BlockStorage::HeightType height) const {
      const auto found = blocks_by_height_.find(height);
      if (found == blocks_by_height_.end()) {
        getLogger()->info(
            "Requested block with height {} not found in block storage.",
            height);
        return {};
      }
      return found->second;
    }

    BlockStorage::BlockPtr BlockStorage::getBlockByHash(
        const BlockStorage::HashType &hash) const {
      const auto found = blocks_by_hash_.find(hash);
      if (found == blocks_by_hash_.end()) {
        getLogger()->info(
            "Requested block with hash {} not found in block storage.",
            hash.toString());
        return {};
      }
      return found->second;
    }

    BlockStorage::BlockPtr BlockStorage::getTopBlock() const {
      if (blocks_by_height_.empty()) {
        getLogger()->info(
            "Requested top block, but the block storage is empty.");
        return {};
      }
      return blocks_by_height_.at(*boost::range::max_element(
          blocks_by_height_ | boost::adaptors::map_keys));
    }

    void BlockStorage::claimUsingPeer(const std::shared_ptr<FakePeer> &peer) {
      auto found = std::find_if(
          using_peers_.begin(), using_peers_.end(), [&peer](const auto &wptr) {
            return wptr.lock() == peer;
          });
      if (found != using_peers_.end()) {
        getLogger()->warn(
            "Requested adding of a using fake peer ({}) "
            "that was already added before.",
            peer->getAddress());
        return;
      }
      using_peers_.emplace_back(peer);
    }

    void BlockStorage::claimNotUsingPeer(
        const std::shared_ptr<FakePeer> &peer) {
      auto found = std::find_if(
          using_peers_.begin(), using_peers_.end(), [&peer](const auto &wptr) {
            return wptr.lock() == peer;
          });
      if (found == using_peers_.end()) {
        getLogger()->warn(
            "Requested removal of a using fake peer "
            "without previously registering it.");
        return;
      }
      using_peers_.erase(found);
    }

    logger::Logger BlockStorage::getLogger() const {
      std::vector<std::string> using_peers_addresses;
      auto it = using_peers_.begin();
      while (it != using_peers_.end()) {
        auto peer = it->lock();
        if (!peer) {
          it = using_peers_.erase(it);
        } else {
          using_peers_addresses.emplace_back(peer->getAddress());
          ++it;
        }
      }
      return logger::log("Fake peer block storage used by ["
                         + boost::algorithm::join(using_peers_addresses, ", ")
                         + "]");
    }

  }  // namespace fake_peer
}  // namespace integration_framework
