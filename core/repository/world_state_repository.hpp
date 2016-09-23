#ifndef __WORLD_STATE_REPOSITORY_HPP_
#define __WORLD_STATE_REPOSITORY_HPP_

#include <vector>
#include <memory>
#include <string>

namespace repository{

  // Substance of Repository
  template<typename T>
  class WorldStateRepositoryWithLevelDB;

  // This is only interface.
  template<typename T>
  class WorldStateRepository {
    public:
      virtual ~WorldStateRepository();
      
      // not support move, copy. 
      WorldStateRepository(WorldStateRepository&&) = delete;
      WorldStateRepository& operator = (WorldStateRepository&&) = delete;
      WorldStateRepository(const WorldStateRepository&) = delete;
      WorldStateRepository& operator = (const WorldStateRepository&) = delete;
  
      virtual bool add(std::string key, T value) = 0;
      virtual bool update(std::string key, T newValue) = 0;
      virtual bool remove(std::string key) = 0;
      virtual std::vector<std::unique_ptr<T> > findAll(std::string key) = 0;
      virtual std::unique_ptr<T> findOne(std::string key) = 0;
      virtual std::unique_ptr<T> findOrElse(std::string key, T defaultVale) = 0;
      virtual bool isExist(std::string key) = 0;
  };

};

#endif
