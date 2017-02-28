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

public class TestSimpleAsset {

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

  private static void ensureIntegirityOfSimpleAssetValue(HashMap<String, String> lhs,
                                                         HashMap<String, String> rhs) throws IllegalStateException {

      if (! lhs.equals(rhs)) {
        if (! lhs.get("type").equals("double")) {
          System.out.println(lhs.get("type")  + " vs " + rhs.get("type"));
          System.out.println(lhs.get("value") + " vs " + rhs.get("value"));
          throw new IllegalStateException("Mismatch simple asset");
        }

        final Double Eps = 1e-5;
        final Double Diff = Math.abs(
            Double.parseDouble( lhs.get("value") )
          - Double.parseDouble( rhs.get("value") )
        );
        if (Diff >= Eps) {
          throw new IllegalStateException("Double value difference is " + Diff + ", over EPS(" + Eps + ")");
        } else {
          System.out.println("Warning: double value difference: " + Diff);
        }
      }
  }

  /******************************************************************************
   * Verify simple asset
   ******************************************************************************/

  public static void testAddSimpleAsset(HashMap<String, String> params, HashMap<String, String> value) throws IllegalStateException {
    try {

      System.out.println("----------------------------------------------");
      System.out.println("Params params: " + params);
      System.out.println("Params value:  " + value);
      System.out.println("----------------------------------------------");
      
      // 1. Add simpleAsset.
      System.out.println("Call simpleAssetAdd.add()");

      String uuid = repository.simpleAssetAdd(
        params,
        value
      );
      
      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: uuid: " + uuid);
      System.out.println("----------------------------------------------");

      // 2. Find asset data by uuid.
      HashMap<String, String> uuidmap = new HashMap<String, String>();
      uuidmap.put(Uuid, params.get(Uuid));
      System.out.println("Call assetRepo.findByUuid()");
      HashMap<String, String> paramsMap = repository.simpleAssetInfoFindByUuid(uuidmap);
      HashMap<String, String> valueMap  = repository.simpleAssetValueFindByUuid(uuidmap);

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: found params: "  + params);
      System.out.println("Received from C++: found value:  "  + value);
      System.out.println("----------------------------------------------");

      // 3. Ensure the integrity.
      if (!params.get(DomainId).equals(paramsMap.get(DomainId)))
        throw new IllegalStateException("Mismatch domain id");

      if (!params.get(SimpleAssetName).equals(paramsMap.get(AssetName)))
        throw new IllegalStateException("Mismatch asset name");

      if (!params.get(ContractName).equals(paramsMap.get(ContractName)))
        throw new IllegalStateException("Mismatch smartcontract name");

      // value check is responsible for C++.
      ensureIntegirityOfSimpleAssetValue(value, valueMap);

      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }

  public static void testUpdateSimpleAsset(HashMap<String, String> params, HashMap<String, String> value) throws IllegalStateException {
    try {

      // 1. Update Asset.
      repository.simpleAssetUpdate(params, value);

      // 2. Find by the uuid.
      HashMap<String, String> valueMap = repository.simpleAssetValueFindByUuid(params);

      // 3. Ensure the integrity.
      ensureIntegirityOfSimpleAssetValue(value, valueMap);

      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }

  public static void testRemoveSimpleAsset(HashMap<String, String> params) throws IllegalStateException {
    try {
      repository.simpleAssetRemove(params);
      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }

  /*********************************************************************************************
   * Check from Java
   *********************************************************************************************/

  private static void javaIntegrityCheckAddSimpleAsset() {
    try {
      HashMap<String, String> info = new HashMap<String, String>();
      info.put(DomainId, "DOMAIN ID");
      info.put(SimpleAssetName, "Mizuki Sonoko");
      info.put(ContractName, "simpleAssetContractFunc");

      HashMap<String, String> value = new HashMap<String, String>();
      value.put("type", "string");
      value.put("value", "FooValue");

      testAddSimpleAsset(info, value);

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
    System.out.println("Hello in Java! Test SimpleAsset");
    javaIntegrityCheckAddSimpleAsset();
//    javaIntegrityCheckAddAsset();
//    System.out.println("Call C++ gTest is also required.");
  }

}
