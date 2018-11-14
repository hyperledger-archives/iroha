/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_INTEGRATION_FRAMEWORK_PORT_GUARD_HPP
#define IROHA_INTEGRATION_FRAMEWORK_PORT_GUARD_HPP

#include <bitset>
#include <mutex>
#include <cstdint>

#include <boost/noncopyable.hpp>
#include <boost/optional/optional.hpp>

namespace integration_framework {

  class PortGuard final : public boost::noncopyable {
   public:
    using PortType = uint16_t;

    static constexpr PortType kMaxPort = 65535;

    ~PortGuard();

    /// Request a port in given boundaries, including them.
    PortType getPort(const PortType &min_value,
                     const PortType &max_value = kMaxPort);

    /// Request a port in given boundaries, including them.
    boost::optional<PortType> tryGetPort(const PortType &min_value,
                                         const PortType &max_value = kMaxPort);

   private:
    using UsedPorts = std::bitset<kMaxPort>;

    static UsedPorts all_used_ports_;
    static std::mutex all_used_ports_mutex_;

    UsedPorts instance_used_ports_;
  };

}

#endif /* IROHA_INTEGRATION_FRAMEWORK_PORT_GUARD_HPP */
