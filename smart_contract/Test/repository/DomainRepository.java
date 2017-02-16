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
    public static native void     accountUpdateQuantity(String uuid, String assetName, long newValue);
    public static native void     accountAttach(String uuid, String assetName, long assetDefault);
    public static native HashMap<String,String> accountFindByUuid(String uuid);
    public static native String   accountAdd(String publicKey, String alias);

    // Asset
    public static native String   assetAdd(String publicKey, String assetName, String value);
    public static native HashMap<String,String> assetFindByUuid(String key);
    public static native void     assetUpdate(String publicKey, String assetName, String newValue);
    public static native void     assetRemove(String publicKey, String assetName);

}
