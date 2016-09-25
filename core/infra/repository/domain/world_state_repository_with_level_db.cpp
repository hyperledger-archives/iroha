#include "../../../repository/world_state_repository.hpp"
#include "../../../util/exception.hpp"

namespace repository{

  // ToDo
  template<typename T>
  class WorldStateRepositoryWithLevelDB : public WorldStateRepository<T>{
    public:
      WorldStateRepositoryWithLevelDB(){

      }

      ~WorldStateRepositoryWithLevelDB(){

      }
      
      bool add(std::string key, T value) {
        throw exception::NotImplementedException(__func__,__FILE__);
      }

      bool update(std::string key, T newValue) {
        throw exception::NotImplementedException(__func__,__FILE__);
      }

      bool remove(std::string key) {
        throw exception::NotImplementedException(__func__,__FILE__);
      }

      std::vector<std::unique_ptr<T> > 
        findAll(std::string key)
      {
        throw exception::NotImplementedException(__func__,__FILE__);
      }

      std::unique_ptr<T> findOne(std::string key) {
        throw exception::NotImplementedException(__func__,__FILE__);    
      }

      std::unique_ptr<T> 
        findOrElse(
          std::string key,
          T defaultVale) {
        throw exception::NotImplementedException(__func__,__FILE__);
      }

      bool isExist(std::string key) {
        
      }

  };

};
