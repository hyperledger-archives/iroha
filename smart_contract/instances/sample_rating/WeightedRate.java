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

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import static java.lang.Math.min;
import static java.lang.Math.max;

import repository.Repository;
import repository.KeyConstants;

public class WeightedRate {

    private static Repository repository = new Repository();

    /*
     * registerAccount(params, assetValue)
     *   This method is called by front App.
     *   repository.assetAdd() returns uuid. If adding asset fails, uuid will be empty.
     */
    public static String registerAccount(String publicKey, String accountName, String assetUuids[]) {
        /*
         * Future work is that this implementation is replaced with TransactionBuilder,
         * which only issue transaction to sumeragi. Sumeragi will update database
         * if transaction has been accepted.
         *
         * Example (draft):
         *   TxBuilder<Add<Account>>(
         *        params,
         *        assetUuids,
         *        "receiveConsensusResult",
         *   ).send();
         */

        HashMap<String, String> params = new HashMap<String, String>();
        params.put(KeyConstants.PublicKey, publicKey);
        params.put(KeyConstants.AccountName, accountName);

        String uuid = repository.accountAdd(
            params,
            assetUuids
        );
        return uuid;
    }

    public static Boolean attachAssetToAccount(String accountUuid, String assetUuid) {
        /*
         * Future work is that this implementation is replaced with TransactionBuilder,
         * which only issue transaction to sumeragi. Sumeragi will update database
         * if transaction has been accepted.
         *
         * Example (draft):
         *   TxBuilder<Add<Asset>, To<Account>>(
         *        params,
         *        assetValue,
         *        "receiveConsensusResult",
         *   ).send();
         */
        HashMap<String, String> params = new HashMap<String, String>();
        params.put(KeyConstants.Uuid, accountUuid);
        params.put(KeyConstants.AttachedAssetUuid, assetUuid);

        System.out.println("--------------------------------------------------");
        System.out.println(accountUuid);
        System.out.println(assetUuid);
        System.out.println("--------------------------------------------------");

        return repository.accountAttach(params);
    }

    public static List<String> enumerateAssetNamesOfAccount(String accountUuid) {
        HashMap<String, String> params = new HashMap<String, String>();
        params.put(KeyConstants.Uuid, accountUuid);
        String[] assetUuids = repository.accountValueFindByUuid(params);
        List<String> ret = new ArrayList<String>();
        System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        for (String uuid : assetUuids) {
            HashMap<String, String> params2 = new HashMap<String, String>();
            params2.put(KeyConstants.Uuid, uuid);
            System.out.println("Asset UUID: " + uuid);
            HashMap<String, String> asset = repository.assetInfoFindByUuid(params2);
            ret.add(asset.get(KeyConstants.DomainId) + "." + asset.get(KeyConstants.AssetName));
        }
        System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        return ret;
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
    public static String registerWeightedRateAsset(String domainId, String assetName) {

        HashMap<String, HashMap<String, String>> assetValue = new HashMap<String, HashMap<String, String>>();

        HashMap<String, String> rate = new HashMap<String, String>();
        rate.put("type", "double");
        rate.put("value", "10");

        HashMap<String, String> alpha = new HashMap<String, String>();
        alpha.put("type", "double");
        alpha.put("value", "1.25");

        HashMap<String, String> constant = new HashMap<String, String>();
        constant.put("type", "double");
        constant.put("value", "10");

        assetValue.put("rate", rate);
        assetValue.put("alpha", alpha);
        assetValue.put("constant", constant);

        HashMap<String, String> params = new HashMap<String, String>();
        params.put(KeyConstants.DomainId, domainId);
        params.put(KeyConstants.AssetName, assetName);
        params.put(KeyConstants.ContractName, "Test"); // Currently, KeyConstants.ContractName is not be used.

        // repository.assetAdd() returns uuid. If adding asset fails, uuid will be empty.
        String uuid = repository.assetAdd(
            params,
            assetValue
        );
        return uuid;
    }

    public static String findAssetUuidByNameInAccount(String assetName, String accountUuid) {
        HashMap<String, String> params = new HashMap<String, String>();
        params.put(KeyConstants.Uuid, accountUuid);
        String[] assetUuids = repository.accountValueFindByUuid(params);
        for (String uuid : assetUuids) {
            HashMap<String, String> params2 = new HashMap<String, String>();
            params2.put(KeyConstants.Uuid, uuid);
            HashMap<String, String> assetInfo = repository.assetInfoFindByUuid(params2);
            if (assetInfo.get(KeyConstants.AssetName).equals(assetName)) {
                return uuid;
            }
        }
        return "";
    }

    public static String showWeightedRateAsset(String uuid) {
        HashMap<String, String> params = new HashMap<String, String>();
        params.put(KeyConstants.Uuid, uuid);
        HashMap<String, HashMap<String, String>> assetValue = repository.assetValueFindByUuid(params);

        StringBuilder sb = new StringBuilder();
        sb.append("alpha: " + assetValue.get("alpha").get("value"));
        sb.append(", const: " + assetValue.get("constant").get("value"));
        sb.append(", rate:  " + assetValue.get("rate").get("value"));

        return new String(sb);
    }

    /*
            - linearIncrease()
                - rate <- max(1.0, min(1000.0, alpha * rate + constant))
            - hit(params)
                - alpha <- min(100.0, alpha * params["hit_rate"]);
            - miss(params)
                - alpha <- max(1.25, alpha * params["miss_rate"])
    */

    /*
    // Currently, sumeragi does not invoke receiveContractResult() because SC does not call transaction builder.
    public static void receiveConsensusResult() {
        
    }
    */

    public static void linearIncrease(String uuid) {

        HashMap<String, String> params = new HashMap<String, String>();
        params.put(KeyConstants.Uuid, uuid);
        HashMap<String, HashMap<String, String>> value = repository.assetValueFindByUuid(params);

        HashMap<String, String> rate = value.get("rate");
        if (!rate.get("type").equals("double")) throw new IllegalStateException("[FATAL] Mismatch type");
        Double rateValue = Double.parseDouble(rate.get("value"));

        HashMap<String, String> alpha = value.get("alpha");
        if (!alpha.get("type").equals("double")) throw new IllegalStateException("[FATAL] Mismatch type");
        Double alphaValue = Double.parseDouble(alpha.get("value"));

        HashMap<String, String> constant = value.get("constant");
        if (!constant.get("type").equals("double")) throw new IllegalStateException("[FATAL] Mismatch type");
        Double constantValue = Double.parseDouble(constant.get("value"));

        rateValue = Math.max(1.0, Math.min(1000.0, alphaValue * rateValue + constantValue));

        rate.put("value", String.valueOf(rateValue));
        value.put("rate", rate);

        repository.assetUpdate(params, value);
    }

    public static void hit(String uuid, Double hitRate) {

        HashMap<String, String> params = new HashMap<String, String>();
        params.put(KeyConstants.Uuid, uuid);
        HashMap<String, HashMap<String, String>> value = repository.assetValueFindByUuid(params);

        HashMap<String, String> alpha = value.get("alpha");
        if (!alpha.get("type").equals("double")) throw new IllegalStateException("[FATAL] Mismatch type");
        Double alphaValue = Double.parseDouble(alpha.get("value"));

        System.out.println("ALPHA VALUE: " + alphaValue);
        alphaValue = Math.min(100.0, alphaValue * hitRate);
        System.out.println("NEW ALPHA VALUE: " + alphaValue);

        alpha.put("value", String.valueOf(alphaValue));
        value.put("alpha", alpha);

        repository.assetUpdate(params, value);
    }

    public static void miss(String uuid, Double missRate) {
        HashMap<String, String> params = new HashMap<String, String>();
        params.put(KeyConstants.Uuid, uuid);
        HashMap<String, HashMap<String, String>> value = repository.assetValueFindByUuid(params);

        HashMap<String, String> alpha = value.get("alpha");
        if (!alpha.get("type").equals("double")) throw new IllegalStateException("[FATAL] Mismatch type");
        Double alphaValue = Double.parseDouble(alpha.get("value"));

        alphaValue = Math.min(1.25, alphaValue * missRate);

        alpha.put("value", String.valueOf(alphaValue));
        value.put("alpha", alpha);

        repository.assetUpdate(params, value);
    }

    public static void main(String[] args) {
        // For Test

        String[] assetUuids = new String[3];
        assetUuids[0] = registerWeightedRateAsset("MyCompany", "fstAsset");
        assetUuids[1] = registerWeightedRateAsset("MyCompany", "sndAsset");
        assetUuids[2] = registerWeightedRateAsset("MyCompany", "trdAsset");

        String uuidUser1 = registerAccount("Public Key", "User1", assetUuids);

        System.out.println("\n===========================================================================");
        List<String> enumeratedNames = enumerateAssetNamesOfAccount(uuidUser1);
        System.out.println("\n===========================================================================");

        for (String name : enumeratedNames) {
            System.out.println("Name: " + name);
        }
        System.out.println("===========================================================================");

        String secondAssetUuid = findAssetUuidByNameInAccount("sndAsset", uuidUser1);

        System.out.println("Initial value: " + showWeightedRateAsset(secondAssetUuid));

        try {

            String path = new File(".").getAbsoluteFile().getParent();
            File file = new File(path + "/WeightedRate.output");
            FileWriter filewriter = new FileWriter(file, true);

            for (int i = 0; i < 5; ++i) {
                hit(secondAssetUuid, 2.345);
                linearIncrease(secondAssetUuid);
                filewriter.write("Current value: " + showWeightedRateAsset(secondAssetUuid) + "\n");
            }

            for (int i = 0; i < 5; ++i) {
                miss(secondAssetUuid, 0.54321);
                linearIncrease(secondAssetUuid);
                filewriter.write("Current value: " + showWeightedRateAsset(secondAssetUuid) + "\n");
            }

            if (file.exists()){
                if (file.isFile() && file.canWrite()) {
                    filewriter.close();
                } else {
                    System.out.println("Cannot write file");
                }
            }

        } catch (IOException e) {
            System.out.println(e);
        }
    }

}