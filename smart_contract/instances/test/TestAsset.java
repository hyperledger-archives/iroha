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

package instances.test;

import repository.Repository;
import java.util.HashMap;

import static repository.KeyConstants.*;

public class TestAsset {

  private static Repository repository = new Repository();

  public static void printSuccess() {
    System.out.println("==============================================");
    System.out.println("Success");
    System.out.println("==============================================");    
  }

  public static void printFail(IllegalStateException e) {
    System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    System.err.println("Fail");
    System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    System.err.println(e.getMessage() + " in " + e.getClass().getName());
  }

  /******************************************************************************
   * Verify asset
   ******************************************************************************/

  private static void ensureIntegirityOfAssetValue(HashMap<String, HashMap<String, String>> assetValueParam,
                                                   HashMap<String, HashMap<String, String>> assetValueMap) throws IllegalStateException {

    for (HashMap.Entry<String, HashMap<String, String>> e : assetValueParam.entrySet()) {
      if (!e.getValue().get("value").equals(assetValueMap.get(e.getKey()).get("value"))) {
        if (!e.getValue().get("type").equals("double")) {
          System.out.println(e.getValue().get("type") + " vs " + assetValueMap.get(e.getKey()).get("type"));
          System.out.println(e.getValue().get("value") + " vs " + assetValueMap.get(e.getKey()).get("value"));
          throw new IllegalStateException("Mismatch asset value");
        }
        final Double Eps = 1e-5;
        final Double Diff = Math.abs(
            Double.parseDouble( e.getValue().get("value") )
          - Double.parseDouble( assetValueMap.get(e.getKey()).get("value")) );
        if (Diff >= Eps) {
          throw new IllegalStateException("Double value difference is " + Diff + ", over EPS(" + Eps + ")");
        } else {
          System.out.println("Warning: double value difference: " + Diff);
        }
      }
    }
  }

  public static void testAddAsset(HashMap<String, String> assetInfoParam,
                                  HashMap<String, HashMap<String, String>> assetValueParam) throws IllegalStateException {
    try {
      // Print received params
      System.out.println("----------------------------------------------");
      System.out.println("Params: " + assetInfoParam);
      System.out.println("Params domainId:   " + assetInfoParam.get(DomainId));
      System.out.println("Params assetName:  " + assetInfoParam.get(AssetName));
      System.out.println("Params AssetValue: " + assetValueParam);
      System.out.println("Params SCName:     " + assetInfoParam.get(ContractName));
      System.out.println("----------------------------------------------");
      
      // 1. Add asset.
      System.out.println("Call assetRepo.add()");

      String assetUuid = repository.assetAdd(
        assetInfoParam,
        assetValueParam
      );

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: assetUuid: " + assetUuid);
      System.out.println("----------------------------------------------");

      // 2. Find asset data by uuid.
      HashMap<String, String> uuidmap = new HashMap<String, String>();
      uuidmap.put(Uuid, assetUuid);      
      System.out.println("Call assetRepo.findByUuid()");
      HashMap<String, String> assetInfoMap = repository.assetInfoFindByUuid(uuidmap);
      HashMap<String, HashMap<String, String>> assetValueMap = repository.assetValueFindByUuid(uuidmap);

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: found domainId:   " + assetInfoMap.get(DomainId));
      System.out.println("Received from C++: found assetName:  " + assetInfoMap.get(AssetName));
      System.out.println("Received from C++: found assetValue: " + assetValueMap);
      System.out.println("Received from C++: found SCName:     " + assetInfoMap.get(ContractName));
      System.out.println("----------------------------------------------");

      // 3. Ensure the integrity.
      if (! assetInfoParam.get(DomainId).equals(assetInfoMap.get(DomainId)))
        throw new IllegalStateException("Mismatch domain id");

      if (! assetInfoParam.get(AssetName).equals(assetInfoMap.get(AssetName)))
        throw new IllegalStateException("Mismatch asset name");

      if (! assetInfoParam.get(ContractName).equals(assetInfoMap.get(ContractName)))
        throw new IllegalStateException("Mismatch smartcontract name");

      ensureIntegirityOfAssetValue(assetValueParam, assetValueMap);

      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }

  public static void testUpdateAsset(HashMap<String, String> params, HashMap<String, HashMap<String, String>> assetValueParam) throws IllegalStateException {
    try {

      // 1. Update Asset.
      repository.assetUpdate(params, assetValueParam);

      // 2. Find by the uuid.
      HashMap<String, String> uuidmap = new HashMap<String, String>();
      uuidmap.put(Uuid, params.get(Uuid));
      HashMap<String, HashMap<String, String>> assetValueMap = repository.assetValueFindByUuid(uuidmap);

      // 3. Ensure the integrity.
      ensureIntegirityOfAssetValue(assetValueParam, assetValueMap);

      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }

  public static void testRemoveAsset(HashMap<String, String> params) throws IllegalStateException {
    try {

      // 1. Remove Asset.
      repository.assetRemove(params);

      // 2. Ensure removed asset.
      if (repository.assetExists(params))
        throw new IllegalStateException("Failed to removing asset");

      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }

  private static void javaIntegrityCheckAddAsset() {
    try {
      HashMap<String, String> assetInfo = new HashMap<String, String>();
      assetInfo.put(DomainId,   "A domain id");
      assetInfo.put(AssetName,  "Currency");
      assetInfo.put(ContractName, "sample_sc_func");

      HashMap<String, HashMap<String, String>> assetValue = new HashMap<String, HashMap<String, String>>();

      HashMap<String, String> value;

      value = new HashMap<String, String>();
      value.put("type",  "int");
      value.put("value", "123456");
      assetValue.put("some_int_property", value);

      value = new HashMap<String, String>();
      value.put("type", "string");
      value.put("value", "karin");
      assetValue.put("your_favorite", value);

      value = new HashMap<String, String>();
      value.put("type", "boolean");
      value.put("value", "true");
      assetValue.put("is_on", value);

      value = new HashMap<String, String>();
      value.put("type", "double");
      value.put("value", String.valueOf(Double.parseDouble("3.1415926535897932384626433832795028841971")));
      assetValue.put("pi", value);

      testAddAsset(assetInfo, assetValue);

      System.out.println("==============================================");
      System.out.println("Success (from Java main)");
      System.out.println("==============================================");

    } catch(IllegalStateException e) {
      System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
      System.err.println("Fail (from Java main)");
      System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
      System.err.println(e.getMessage() + " in " + e.getClass().getName());      
    }
  }

  public static void main(String[] argv) {
    System.out.println("Hello in JAVA!");
    javaIntegrityCheckAddAsset();
    System.out.println("Call C++ gTest is also required.");
  }

}