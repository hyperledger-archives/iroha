
#ifndef IROHA_PROTO_PEER_BUILDER_HPP
#define IROHA_PROTO_PEER_BUILDER_HPP

#include "utils/polymorphic_wrapper.hpp"
#include "interfaces/common_objects/peer.hpp"
#include "backend/protobuf/common_objects/peer.hpp"
#include "primitive.pb.h"

namespace shared_model {
  namespace proto {
    class PeerBuilder {
     public:
      shared_model::proto::Peer build() {
        return shared_model::proto::Peer(peer_);
      }

      PeerBuilder &address(const interface::types::AddressType &address) {
        peer_.set_address(address);
        return *this;
      }

      PeerBuilder &pubkey(const interface::types::PubkeyType &key) {
        peer_.set_peer_key(shared_model::crypto::toBinaryString(key));
        return *this;
      }

     private:
      iroha::protocol::Peer peer_;
    };
  }
}
#endif //IROHA_PROTO_PEER_BUILDER_HPP
