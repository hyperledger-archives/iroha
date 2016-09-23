#include "../../repository/world_state_repository.hpp"

namespace repository{

  // This is only interface.
  template<typename T>
  class WorldStateRepositoryWithLevelDB : public WorldStateRepository<T>{
    public:
      WorldStateRepositoryWithLevelDB(){

      }

      ~WorldStateRepositoryWithLevelDB(){

      }
      
      bool add(std::string key, T value) {

      }

      bool update(std::string key, T newValue) {

      }

      bool remove(std::string key) {

      }

      std::vector<std::unique_ptr<T> > 
        findAll(std::string key) {

      }

      std::unique_ptr<T> findOne(std::string key) {
    
      }

      std::unique_ptr<T> 
        findOrElse(
          std::string key,
          T defaultVale) {

      }

      bool isExist(std::string key) {
        
      }

  };

};
