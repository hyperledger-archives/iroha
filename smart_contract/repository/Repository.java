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
    public static native String accountAdd(HashMap<String, String> params, String[] assets);
    public static native Boolean accountAttach(HashMap<String, String> params);
    public static native Boolean accountUpdate(HashMap<String, String> params, String[] assets);
    public static native Boolean accountRemove(HashMap<String, String> params);
    public static native HashMap<String, String> accountInfoFindByUuid(HashMap<String, String> params);
    public static native String[] accountValueFindByUuid(HashMap<String, String> params);
    public static native Boolean accountExists(HashMap<String, String> params);

    /*
     * Domain
     */
    public static native String domainAdd(HashMap<String, String> params);
    public static native Boolean domainUpdate(HashMap<String, String> params);
    public static native Boolean domainRemove(HashMap<String, String> params);
    public static native HashMap<String, String> domainFindByUuid(HashMap<String, String> params);
    public static native Boolean domainExists(HashMap<String, String> params);

    /*
     * Asset
     */
    public static native String assetAdd(HashMap<String, String> params,
                                         HashMap<String, HashMap<String, String>> value);
    public static native Boolean assetUpdate(HashMap<String, String> params, HashMap<String, HashMap<String, String>> value);
    public static native Boolean assetRemove(HashMap<String, String> params);
    public static native HashMap<String, String> assetInfoFindByUuid(HashMap<String, String> params);
    public static native HashMap<String, HashMap<String, String>> assetValueFindByUuid(HashMap<String, String> params);
    public static native Boolean assetExists(HashMap<String, String> params);

    /*
     * SimpleAsset
     */
    public static native String simpleAssetAdd(HashMap<String, String> params,
                                               HashMap<String, String> value);
    public static native Boolean simpleAssetUpdate(HashMap<String, String> params, HashMap<String, String> value);
    public static native Boolean simpleAssetRemove(HashMap<String, String> params);
    public static native HashMap<String, String> simpleAssetInfoFindByUuid(HashMap<String, String> params);
    public static native HashMap<String, String> simpleAssetValueFindByUuid(HashMap<String, String> params);
    public static native Boolean simpleAssetExists(HashMap<String, String> params);

    /*
     * Peer
     */
    public static native String peerAdd(HashMap<String, String> params, HashMap<String, String> trust);
    public static native Boolean peerUpdate(HashMap<String, String> params, HashMap<String, String> trust);
    public static native Boolean peerRemove(HashMap<String, String> params);
    public static native HashMap<String, String> peerInfoFindByUuid(HashMap<String, String> params);
    public static native HashMap<String, String> peerTrustFindByUuid(HashMap<String, String> params);
    public static native Boolean peerExists(HashMap<String, String> params);
}
