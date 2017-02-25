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

package test.repository;

import java.util.HashMap;

public class DomainRepository {

    static {
        System.loadLibrary("DomainRepository");
    }

    // Account
    public static native String accountAdd(String publicKey, String name, String[] assets);
    public static native Boolean accountAttach(String uuid, String asset);
    public static native Boolean accountUpdate(String uuid, String[] assets);
    public static native Boolean accountRemove(String uuid);
    public static native HashMap<String, String> accountInfoFindByUuid(String uuid);
    public static native String[] accountValueFindByUuid(String uuid);
    public static native Boolean accountExists(String uuid);

    /*
     * Asset
     * TODO: Replace HashMap with JSON.
     */
    public static native String assetAdd(String domain, String name,
                                         HashMap<String, HashMap<String, String>> value,
                                         String smartContractName);
    public static native Boolean assetUpdate(String uuid, HashMap<String, HashMap<String, String>> value);
    public static native Boolean assetRemove(String uuid);
//    public static native HashMap<String,String>[] findAll(String uuid);
    public static native HashMap<String, String> assetInfoFindByUuid(String uuid);
    public static native HashMap<String, HashMap<String, String>> assetValueFindByUuid(String uuid);
    public static native Boolean assetExists(String uuid);
}
