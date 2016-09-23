

#include <memory>
#include <string>

#include "../util/logger.hpp"

namespace asset {

class Asset {
  protected:
    // not support copy
    Asset(Asset const&) = delete;
    Asset& operator =(Asset const&) = delete;
};

}; // namespace asset