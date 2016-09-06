#ifndef __ENTITY_REPOSITORY__
#define __ENTITY_REPOSITORY__

#include <string>

namespace EntityRepository{
  bool add(std::string uuid,Entity e);
  bool update(std::string uuid,Entity e);
  bool remove(std::string uuid);

  Entity find(std::string uuid);
};

#endif
