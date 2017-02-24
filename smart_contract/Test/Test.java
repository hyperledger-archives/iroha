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

package test;

import test.repository.DomainRepository;
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

  // Test invoke function
  public static void test1() {
      System.out.println("Hello in JAVA! test1()");
  }

  // Test invoke function(HashMap<String,String>)
  public static void test2(HashMap<String,String> params){
    System.out.println("Hello in JAVA! test2()");
    assert params.get("key1").equals("Mizuki");
    assert params.get("key2").equals("Sonoko");
  }

  // Test invoke function(HashMap<String,String>) (UTF-8)
  public static void test3(HashMap<String,String> params){
    System.out.println("Hello in JAVA! test3()");
    assert params.get("key1").equals("水樹");
    assert params.get("key2").equals("素子");
  }

  /******************************************************************************
   * Verify account
   ******************************************************************************/
  public static void test_add_account(HashMap<String, String> params) {
    try {
      // Print received params
      System.out.println("----------------------------------------------");
      System.out.println("Params pubKey:      " + params.get(PublicKeyTag));
      System.out.println("Params accountName: " + params.get(AccountNameTag));
      System.out.println("----------------------------------------------");

      System.out.println("Call accountRepo.add()");

      String[] assets = { "Hoge", "Foo", "Bar" };

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

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: found pubKey:      " + accountMap.get(PublicKeyTag));
      System.out.println("Received from C++: found accountName: " + accountMap.get(AccountNameTag));
      System.out.println("----------------------------------------------");

      // 3. Then, verify integrity.
      /*
      assert accountMap.get(PublicKeyTag).equals(params.get(PublicKeyTag));
      assert accountMap.get(AccountNameTag).equals(params.get(AccountNameTag));
      */
      if (! accountMap.get(PublicKeyTag).equals(params.get(PublicKeyTag)))
        throw new Exception("Mismatch public key");

      if (! accountMap.get(AccountNameTag).equals(params.get(AccountNameTag)))
        throw new Exception("Mismatch account name");

      System.out.println("Success assertions of integrity.");
      System.out.println("----------------------------------------------");

    } catch(Exception e) {
      System.err.println(e.getMessage() + " in " + e.getClass().getName());
    }
  }

  /******************************************************************************
   * Verify asset
   ******************************************************************************/
  public static void test_add_asset(HashMap<String,String> params, HashMap<String,HashMap<String,String>> assetValueParam) {
    try {
      // Print received params
      System.out.println("----------------------------------------------");
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

      // 3. Then, verify integrity.
      /*
      assert params.get(DomainIdTag).equals(assetMap.get(DomainIdTag))     : "DomainId doesn't match.";
      assert params.get(AssetNameTag).equals(assetMap.get(AssetNameTag))   : "AssetName doesn't match.";
      assert params.get(AssetValueTag).equals(assetMap.get(AssetValueTag)) : "AssetValue doesn't match.";
      */
      if (! params.get(DomainIdTag).equals(assetInfoMap.get(DomainIdTag)))
        throw new Exception("Mismatch domain id");

      if (! params.get(AssetNameTag).equals(assetInfoMap.get(AssetNameTag)))
        throw new Exception("Mismatch asset name");

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

      if (! params.get(SmartContractNameTag).equals(assetInfoMap.get(SmartContractNameTag)))
        throw new Exception("Mismatch smartcontract name");

      System.out.println("Success assertions of integrity.");
      System.out.println("----------------------------------------------");

    } catch(Exception e) {
      System.err.println(e.getMessage() + " in " + e.getClass().getName());
    }
  }

  public static void java_integrity_check() {
    try {
      HashMap<String, String> params = new HashMap<String, String>();
      params.put(PublicKeyTag, "This is Public key.");
      params.put(AccountNameTag, "Mizuki Sonoko");
      test_add_account(params);

      HashMap<String, String> params2 = new HashMap<String, String>();
      params2.put(DomainIdTag,   "A domain id");
      params2.put(AssetNameTag,  "Currency");
      params2.put(SmartContractNameTag, "sample_sc_func");
  //    params2.put(AssetValueTag, "123456");

      HashMap<String, HashMap<String, String>> assetValue = new HashMap<String, HashMap<String, String>>();

      HashMap<String, String> value;

      value = new HashMap<String, String>();
      value.put("type",  "int");
      value.put("value", "123456");
      assetValue.put("someIntProperty", value);

      value = new HashMap<String, String>();
      value.put("type", "string");
      value.put("value", "karin");
      assetValue.put("yourFavorite", value);

      value = new HashMap<String, String>();
      value.put("type", "boolean");
      value.put("value", "true");
      assetValue.put("isOn", value);

      value = new HashMap<String, String>();
      value.put("type", "double");
      value.put("value", String.valueOf(Double.parseDouble("3.1415926535897932384626433832795028841971")));
      assetValue.put("pi", value);

      test_add_asset(params2, assetValue);
    } catch(Exception e) {
      System.err.println(e.getMessage() + " in " + e.getClass().getName());      
    }
  }

  public static void main(String[] argv) {
    System.out.println("Hello in JAVA!");
    java_integrity_check();
    System.out.println("Call C++ gTest is also required.");
  }

}
