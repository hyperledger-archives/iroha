
#include <string>

namespace domain {
    namespace asset {

      class Asset {
        protected:
          std::string name;
          std::string parentDomainName;
        public:

          // Constructor is only used by factory.
          Asset(
            std::string aName,
            std::string aParentDomainName
          ):
            name(aName),
            parentDomainName(aParentDomainName)
          {}
  
          virtual ~Asset() = default;

          // Support move and copy.
          Asset(Asset const&) = default;
          Asset(Asset&&) = default;
          Asset& operator =(Asset const&) = default;
          Asset& operator =(Asset&&) = default;

      };

    };  // namespace asset
};  // namespace domain