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

import java.util.HashMap;

public class Repository {

    static {
        System.loadLibrary("Repository");
    }

    /*
     * Account
     */
    public static native String accountAdd(String publicKey, String name, String[] assets);
    public static native Boolean accountAttach(String uuid, String asset);
    public static native Boolean accountUpdate(String uuid, String name, String[] assets);
    public static native Boolean accountRemove(String uuid);
    public static native HashMap<String, String> accountInfoFindByUuid(String uuid);
    public static native String[] accountValueFindByUuid(String uuid);
    public static native Boolean accountExists(String uuid);

    /*
     * Domain
     */
    public static native String domainAdd(String ownerPublicKey, String name);
    public static native Boolean domainUpdate(String uuid, String name);
    public static native Boolean domainRemove(String uuid);
    public static native HashMap<String, String> domainFindByUuid(String uuid);
    public static native Boolean domainExists(String uuid);

    /*
     * Asset
     */
    public static native String assetAdd(String domain, String name,
                                         HashMap<String, HashMap<String, String>> value,
                                         String smartContractName);
    public static native Boolean assetUpdate(String uuid, HashMap<String, HashMap<String, String>> value);
    public static native Boolean assetRemove(String uuid);
    public static native HashMap<String, String> assetInfoFindByUuid(String uuid);
    public static native HashMap<String, HashMap<String, String>> assetValueFindByUuid(String uuid);
    public static native Boolean assetExists(String uuid);

    /*
     * SimpleAsset
     */
    public static native String simpleAssetAdd(String domain, String name,
                                               HashMap<String, String> value,
                                               String smartContractName);
    public static native Boolean simpleAssetUpdate(String uuid, HashMap<String, String> value);
    public static native Boolean simpleAssetRemove(String uuid);
    public static native HashMap<String, String> simpleAssetInfoFindByUuid(String uuid);
    public static native HashMap<String, String> simpleAssetValueFindByUuid(String uuid);
    public static native Boolean simpleAssetExists(String uuid);

    /*
     * Peer
     */
    public static native String peerAdd(String publicKey, String name, String[] assets);
    public static native Boolean peerAttach(String uuid, String asset);
    public static native Boolean peerUpdate(String uuid, String[] assets);
    public static native Boolean peerRemove(String uuid);
    public static native HashMap<String, String> peerFindByUuid(String uuid);
    public static native Boolean peerExists(String uuid);
}
