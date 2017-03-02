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

package instances.sample_rating;

import java.util.HashMap;

import repository.Repository;
import repository.KeyConstants;

/*
- WeightingRate.java
    - 要件：
        - 初期条件: rate = 10, alpha = 1.25, constant = 10
        - invokeされるメソッド
            - linearIncrease()
                - rate <- max(1.0, min(1000.0, alpha * rate + constant))
            - hit(params)
                - alpha <- min(100.0, alpha * params["hit_rate"]);
            - miss(params)
                1. counter <- max(0, counter - 1)
                2. alpha <- max(1.25, alpha * params["miss_rate"])
    - 以下を10回繰り返す
        - 10回 hit(), miss() を実行
        - linearIncrease() を実行
        - Rを表示
*/

/*
message SimpleAsset {
  string domain = 1;
  string name = 2;
  BaseObject value = 3;
  string smartContractName = 4;
}
message Domain {
  string ownerPublicKey = 1;
  string name = 2;
}

message Trust{
  double value = 1;
  bool isOk = 2;
}

message Peer {
  string publicKey = 1;
  string address = 2;
  Trust trust = 3;
}
*/
public class WeightedRate {

    private static Repository repository = new Repository();

    /*
        Asset
        {
          string domain
          string name
          map<string, BaseObject> value
          string smartContractName
        }

        front Asset API
        {
          "assetUuid" : "sha3",
          "name" : "iroha",
          "domain" : "hyperledger",
          "creator" : "base64_public_key",
          "signature" : "b64encoded?_signature",
          "timestamp" : 12345
        }
    */
    /*
     * registerAccount(params, assetValue)
     *   This method is called by front App.
     *   repository.assetAdd() returns uuid. If adding asset fails, uuid will be empty.
     */
    public static String registerAccount(HashMap<String, String> accountInfo, String[] assetUuids) {
        /*
         * Currently, repository makes database be updated directly, it doesn't issue Transaction.
         * For future work, this program is replaced by TransactionBuilder, which only issue transaction
         * to sumeragi, not update database. Database will be updated by suemragi.
         *
         * Example (draft):
         *   TxBuilder<Add<Asset>>(
         *        params.get(KeyConstants.DomainId),
         *        params.get(KeyConstants.AssetName),
         *        assetValue,
         *        "receiveConsensusResult",
         *   ).send();
         */
        String uuid = repository.accountAdd(
            params.get(KeyConstants.PublicKey),
            params.get(KeyConstants.AccountName),
            assetUuids
        );
        return uuid;
    }

    /*
     * For transaction builder (future work)
     * 
     * public static void receiveConsensusResult(HashMap<String, String> consensusResult) {
     *   if (consensusResult.get("result").equals("Accepted")) {
     *     // some update query for sumeragi needs to be written.
     *   }
     * }
     */

    /*
     * registerWeightedRateAsset(params)
     *   This method is called by front App.
     *
     *   Initial guess: rate = 10, alpha = 1.25, constant = 10
     */
    public static String registerWeightedRateAsset(HashMap<String, String> params) {

        HashMap<String, HashMap<String, String>> assetValue = new HashMap<String, HashMap<String, String>>();

        HashMap<String, String> rate = new HashMap<String, String>();
        rate.put("type", "double");
        rate.put("value", params("rate").get());

        HashMap<String, String> constant = new HashMap<String, String>();
        rate.put("type", "double");
        rate.put("value", params("constant").get());

        HashMap<String, String> alpha = new HashMap<String, String>();
        alpha.put("type", "double");
        alpha.put("value", "1.25");

        // repository.assetAdd() returns uuid. If adding asset fails, uuid will be empty.
        String uuid = repository.assetAdd(
            params.get(KeyConstants.DomainId),
            params.get(KeyConstants.AssetName),
            assetValue,
            "" // params.get(KeyConstants.ContractName) // Currently, KeyConstants.ContractName is not be used.
        );
        return uuid;
    }

    /*
            - linearIncrease()
                - rate <- max(1.0, min(1000.0, alpha * rate + constant))
            - hit(params)
                - alpha <- min(100.0, alpha * params["hit_rate"]);
            - miss(params)
                1. counter <- max(0, counter - 1)
                2. alpha <- max(1.25, alpha * params["miss_rate"])
    */

    public static void receiveConsensusResult() {
        
    }

    public static void linearIncrease() {
        
    }

    public static void hit(HashMap<String, String> params) {

    }

    public static void miss(HashMap<String, String> params) {

    }

}