#ifndef __ENTITY_REPOSITORY__
#define __ENTITY_REPOSITORY__

#include <string>

namespace EntityRepository{
  void add(Entity e);
  void update(std::string uuid,Entity e);
  void delete(std::string uuid);

  Entity find(std::string uuid);
};

#endif
