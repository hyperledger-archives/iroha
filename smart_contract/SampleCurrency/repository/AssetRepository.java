/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
     http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package repository;

public class AssetRepository {

    static {
        System.loadLibrary("AssetRepository");
    }
    
//    public static native void save(String key,String value);
    public static native void     add(String publicKey, String assetName, String value);
    public static native String   findOne(String key); // pointerでなくStringに出力してから受け取る
    public static native void     update(String publicKey, String assetName, String newValue);
    public static native void     remove(String publicKey, String assetName);
    //std::vector<std::unique_ptr<T> > findAll(std::string key);
    //std::unique_ptr<T> findOrElse(std::string key, T defaultVale);
    //bool isExist(std::string key);

}