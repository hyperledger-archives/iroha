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

import repository.DomainRepository;
import java.util.Map;
import java.util.HashMap;
import java.util.Iterator;

public class Test {

  // Move some clear position.
  public static final String PublicKeyTag   = "publicKey";
  public static final String AccountNameTag = "accountName";
  public static final String DomainIdTag    = "domainId";
  public static final String AssetNameTag   = "assetName";
  public static final String AssetValueTag  = "assetValue";
  public static final String SmartContractNameTag  = "smartContractName";

  static DomainRepository domainRepo = new DomainRepository();

  public static void printSuccess() {
    System.out.println("==============================================");
    System.out.println("Success");
    System.out.println("==============================================");    
  }

  public static void printFail(Exception e) {
    System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    System.err.println("Fail");
    System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
    System.err.println(e.getMessage() + " in " + e.getClass().getName());
  }

  // Test invoke function
  public static void test1() {
      System.out.println("Hello in JAVA! test1()");
  }

  // Test invoke function(HashMap<String,String>)
  public static void test2(HashMap<String,String> params) throws Exception {
    try {
      System.out.println("Hello in JAVA! test2()");
      if (! params.get("key1").equals("Mizuki"))
        throw new Exception("Failed 'key1' value");

      if (! params.get("key2").equals("Sonoko"))
        throw new Exception("Failed 'key2' value");

      printSuccess();
    } catch(Exception e) {
      printFail(e);
    }
  }

  // Test invoke function(HashMap<String,String>) (UTF-8)
  public static void test3(HashMap<String,String> params) throws Exception {
    try {
      System.out.println("Hello in JAVA! test3()");
      if (! params.get("key1").equals("水樹"))
        throw new Exception("Failed 'key1' value");

      if (! params.get("key2").equals("素子"))
        throw new Exception("Failed 'key2' value");

      printSuccess();
    } catch(Exception e) {
      printFail(e);
    }
  }

  /******************************************************************************
   * Verify account
   ******************************************************************************/
  public static void testAddAccount(HashMap<String, String> params, String[] assets) throws Exception {
    try {
      // Print received params
      System.out.println("----------------------------------------------");
      System.out.println("Params pubKey:      " + params.get(PublicKeyTag));
      System.out.println("Params accountName: " + params.get(AccountNameTag));
      for (int i = 0; i < assets.length; i++) {
        System.out.println("Params assets[" + i + "]: " + assets[i]);
      }
      System.out.println("----------------------------------------------");

      // 1. Add account.
      System.out.println("Call accountRepo.add()");

      String accountUuid = domainRepo.accountAdd(
        params.get(PublicKeyTag),
        params.get(AccountNameTag),
        assets
      );

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: accountUuid: " + accountUuid);
      System.out.println("----------------------------------------------");

      // 2. Find account data by uuid.
      System.out.println("Call accountRepo.findByUuid()");
      HashMap<String, String> accountMap = domainRepo.accountInfoFindByUuid(accountUuid);
      String[] assetsArray = domainRepo.accountValueFindByUuid(accountUuid);

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: found pubKey:      " + accountMap.get(PublicKeyTag));
      System.out.println("Received from C++: found accountName: " + accountMap.get(AccountNameTag));
      for (int i = 0; i < assets.length; i++) {
        System.out.println("Received from C++: found assets[" + i + "]:   " + assetsArray[i]);
      }
      System.out.println("----------------------------------------------");

      // 3. Ensure the integrity.
      /*
      assert accountMap.get(PublicKeyTag).equals(params.get(PublicKeyTag));
      assert accountMap.get(AccountNameTag).equals(params.get(AccountNameTag));
      */
      if (! accountMap.get(PublicKeyTag).equals(params.get(PublicKeyTag)))
        throw new Exception("Mismatch public key");

      if (! accountMap.get(AccountNameTag).equals(params.get(AccountNameTag)))
        throw new Exception("Mismatch account name");

      printSuccess();
    } catch(Exception e) {
      printFail(e);
    }
  }

  /******************************************************************************
   * Verify asset
   ******************************************************************************/

  private static void ensureIntegirityOfAsset(HashMap<String, String> params,
                                              HashMap<String, HashMap<String, String>> assetValueParam,
                                              HashMap<String, String> assetInfoMap,
                                              HashMap<String, HashMap<String, String>> assetValueMap) throws Exception {
    if (! params.get(DomainIdTag).equals(assetInfoMap.get(DomainIdTag)))
      throw new Exception("Mismatch domain id");

    if (! params.get(AssetNameTag).equals(assetInfoMap.get(AssetNameTag)))
      throw new Exception("Mismatch asset name");

    if (! params.get(SmartContractNameTag).equals(assetInfoMap.get(SmartContractNameTag)))
      throw new Exception("Mismatch smartcontract name");

    for(HashMap.Entry<String, HashMap<String, String>> e : assetValueParam.entrySet()) {
      if (! e.getValue().get("value").equals(assetValueMap.get(e.getKey()).get("value"))) {
        if (! e.getValue().get("type").equals("double")) {
          System.out.println(e.getValue().get("type") + " vs " + assetValueMap.get(e.getKey()).get("type"));
          System.out.println(e.getValue().get("value") + " vs " + assetValueMap.get(e.getKey()).get("value"));
          throw new Exception("Mismatch asset value");
        }
        final Double Eps = 1e-5;
        final Double Diff = Math.abs(
            Double.parseDouble( e.getValue().get("value") )
          - Double.parseDouble( assetValueMap.get(e.getKey()).get("value")) );
        if (Diff >= Eps) {
          throw new Exception("Double value difference is " + Diff + ", over EPS(" + Eps + ")");
        } else {
          System.out.println("Warning: double value difference: " + Diff);
        }
      }
    }
  }

  public static void testAddAsset(HashMap<String, String> params, HashMap<String, HashMap<String, String>> assetValueParam) throws Exception {
    try {
      // Print received params
      System.out.println("----------------------------------------------");
      System.out.println("Params: " + params);
      System.out.println("Params domainId:   " + params.get(DomainIdTag));
      System.out.println("Params assetName:  " + params.get(AssetNameTag));
      System.out.println("Params AssetValue: " + assetValueParam);
      System.out.println("Params SCName:     " + params.get(SmartContractNameTag));
      System.out.println("----------------------------------------------");
      
      // 1. Add asset.
      System.out.println("Call assetRepo.add()");

      String assetUuid = domainRepo.assetAdd(
        params.get(DomainIdTag),
        params.get(AssetNameTag),
        assetValueParam,
        params.get(SmartContractNameTag)
      );
      
      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: assetUuid: " + assetUuid);
      System.out.println("----------------------------------------------");

      // 2. Find asset data by uuid.
      System.out.println("Call assetRepo.findByUuid()");
      HashMap<String, String> assetInfoMap = domainRepo.assetInfoFindByUuid(assetUuid);
      HashMap<String, HashMap<String, String>> assetValueMap = domainRepo.assetValueFindByUuid(assetUuid);

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: found domainId:   " + assetInfoMap.get(DomainIdTag));
      System.out.println("Received from C++: found assetName:  " + assetInfoMap.get(AssetNameTag));
      System.out.println("Received from C++: found assetValue: " + assetValueMap);
      System.out.println("Received from C++: found SCName:     " + assetInfoMap.get(SmartContractNameTag));
      System.out.println("----------------------------------------------");

      // 3. Ensure the integrity.
      ensureIntegirityOfAsset(params, assetValueParam, assetInfoMap, assetValueMap);

      printSuccess();
    } catch(Exception e) {
      printFail(e);
    }
  }

  public static void testUpdateAsset(HashMap<String, String> params, HashMap<String, HashMap<String, String>> assetValueParam) throws Exception {
    try {

      // 1. Update Asset.
      domainRepo.assetUpdate(params.get("uuid"), assetValueParam);

      // 2. Find by the uuid.
      HashMap<String, String> assetInfoMap = domainRepo.assetInfoFindByUuid(params.get("uuid"));
      HashMap<String, HashMap<String, String>> assetValueMap = domainRepo.assetValueFindByUuid(params.get("uuid"));

      // 3. Ensure the integrity.
      ensureIntegirityOfAsset(params, assetValueParam, assetInfoMap, assetValueMap);

      printSuccess();
    } catch(Exception e) {
      printFail(e);
    }
  }

  public static void testRemoveAsset(HashMap<String, String> params) throws Exception {
    try {

      // 1. Remove Asset.
      domainRepo.assetRemove(params.get("uuid"));

      // 2. Find by the uuid.
      HashMap<String, HashMap<String, String>> assetValueMap = domainRepo.assetValueFindByUuid(params.get("uuid"));

      // 3. Ensure removed asset.
      if (domainRepo.assetExists(params.get("uuid")))
        throw new Exception("Failed to removing asset");

      printSuccess();
    } catch(Exception e) {
      printFail(e);
    }
  }

  /***************************************************************************************************
   * From java main function
   ***************************************************************************************************/

  // Test 'add' only. All command tests are invoked by gTest.
  public static void javaIntegrityCheckAddAccount() {
    try {
      HashMap<String, String> params = new HashMap<String, String>();
      params.put(PublicKeyTag, "This is Public key.");
      params.put(AccountNameTag, "Mizuki Sonoko");
      String[] assets = { "Hoge", "Foo", "Bar" };

      testAddAccount(params, assets);

      System.out.println("==============================================");
      System.out.println("Success (from Java main)");
      System.out.println("==============================================");

    } catch(Exception e) {
      System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
      System.err.println("Fail (from Java main)");
      System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
      System.err.println(e.getMessage() + " in " + e.getClass().getName());
    }
  }

  // Test 'add' only. All command tests are invoked by gTest.
  public static void javaIntegrityCheckAddAsset() {
    try {
      HashMap<String, String> params = new HashMap<String, String>();
      params.put(DomainIdTag,   "A domain id");
      params.put(AssetNameTag,  "Currency");
      params.put(SmartContractNameTag, "sample_sc_func");
  //    params.put(AssetValueTag, "123456");

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

      testAddAsset(params, assetValue);

      System.out.println("==============================================");
      System.out.println("Success (from Java main)");
      System.out.println("==============================================");

    } catch(Exception e) {
      System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
      System.err.println("Fail (from Java main)");
      System.err.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
      System.err.println(e.getMessage() + " in " + e.getClass().getName());      
    }
  }

  public static void main(String[] argv) {
    System.out.println("Hello in JAVA!");
    javaIntegrityCheckAddAccount();
    javaIntegrityCheckAddAsset();
    System.out.println("Call C++ gTest is also required.");
  }

}
