//
// Created by 五十嵐太清 on 2016/12/08.
//

#ifndef IROHA_ASSET_REPOSITORY_HPP_H
#define IROHA_ASSET_REPOSITORY_HPP_H
namespace repository{
    namespace Asset {
        template<typename T>
        bool add(std::string key, T value);

        template<typename T>
        bool update(std::string key, T newValue);

        // This is OK...??
        template<typename T>
        bool remove(std::string key);

        template<typename T>
        std::vector <std::unique_ptr<T>> findAll(std::string key);

        template<typename T>
        std::unique_ptr <T> findOne(std::string key);

        template<typename T>
        std::unique_ptr <T> findOrElse(std::string key, T defaultVale);

        template<typename T>
        bool isExist(std::string key);
    }
}
#endif //IROHA_ASSET_REPOSITORY_HPP_H
