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

import java.util.HashMap;
import test.repository.DomainRepository;

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
    if (! accountMap.get(PublicKeyTag).equals(params.get(PublicKeyTag)))      return;
    if (! accountMap.get(AccountNameTag).equals(params.get(AccountNameTag)))  return;

    System.out.println("Success assertions of integrity.");
    System.out.println("----------------------------------------------");
  }

  /******************************************************************************
   * Verify asset
   ******************************************************************************/
  public static void test_add_asset(HashMap<String,String> params, HashMap<String,HashMap<String,String>> assetValue) {
    // Print received params
    System.out.println("----------------------------------------------");
    System.out.println("Params domainId:   " + params.get(DomainIdTag));
    System.out.println("Params assetName:  " + params.get(AssetNameTag));
    System.out.println("AssetValue:        " + assetValue);
    System.out.println("Params SCName:     " + params.get(SmartContractNameTag));
    System.out.println("----------------------------------------------");
    
    // 1. Add asset.
    System.out.println("Call assetRepo.add()");

    String assetUuid = domainRepo.assetAdd(
      params.get(DomainIdTag),
      params.get(AssetNameTag),
      assetValue,
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
    if (! params.get(DomainIdTag).equals(assetInfoMap.get(DomainIdTag)))      return;
    if (! params.get(AssetNameTag).equals(assetInfoMap.get(AssetNameTag)))    return;
    if (! assetValue.equals(assetValueMap))  return;
    if (! params.get(SmartContractNameTag).equals(assetInfoMap.get(SmartContractNameTag)))  return;

    System.out.println("Success assertions of integrity.");
    System.out.println("----------------------------------------------");
  }

  public static void main(String[] argv) {
    System.out.println("Hello in JAVA!");
    HashMap<String, String> params = new HashMap<String, String>();
    params.put(PublicKeyTag, "This is Public key.");
    params.put(AccountNameTag, "Mizuki Sonoko");
    test_add_account(params);

    HashMap<String, String> params2 = new HashMap<String, String>();
    params2.put(DomainIdTag,   "A domain id");
    params2.put(AssetNameTag,  "Currency");
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
    value.put("isOn", "true");
    assetValue.put("stringProperty", value);

    value = new HashMap<String, String>();
    value.put("type", "double");
    value.put("value", "3.1415926535");
    assetValue.put("pi", value);

    test_add_asset(params2, assetValue);
  }

}
