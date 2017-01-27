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

import java.util.HashMap;
import repository.AccountRepository;
import repository.AssetRepository;

public class Test {

  // Move some clear position.
  public static final String PublicKeyTag   = "publicKey";
  public static final String AccountNameTag = "accountName";
  public static final String AssetNameTag   = "assetName";
  public static final String AssetValueTag  = "assetValue";

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

    // 1. Add account.
    AccountRepository accountRepo = new AccountRepository();

    System.out.println("Call accountRepo.add()");

    String accountDBKeyHash = accountRepo.add(
      params.get(PublicKeyTag),
      params.get(AccountNameTag)
    );

    System.out.println("----------------------------------------------");
    System.out.println("Received from C++: accountDBKeyHash: " + accountDBKeyHash);
    System.out.println("----------------------------------------------");

    // 2. Find account data by uuid.
    System.out.println("Call accountRepo.findByUuid()");
    HashMap<String, String> accountMap = accountRepo.findByUuid(
      accountDBKeyHash
    );

    System.out.println("----------------------------------------------");
    System.out.println("Received from C++: found pubKey:      " + accountMap.get(PublicKeyTag));
    System.out.println("Received from C++: found accountName: " + accountMap.get(AccountNameTag));
    System.out.println("----------------------------------------------");

    // 3. Then, verify integrity.
    assert accountMap.get(PublicKeyTag).equals(params.get(PublicKeyTag));
    assert accountMap.get(AccountNameTag).equals(params.get(AccountNameTag));

    System.out.println("Success assetions of integrity.");
    System.out.println("----------------------------------------------");
  }

  /******************************************************************************
   * Verify asset
   ******************************************************************************/
  public static void test_add_asset(HashMap<String,String> params) {
    // Print received params
    System.out.println("----------------------------------------------");
    System.out.println("Params pubKey:     " + params.get(PublicKeyTag));
    System.out.println("Params assetName:  " + params.get(AssetNameTag));
    System.out.println("Params assetValue: " + params.get(AssetValueTag));
    System.out.println("----------------------------------------------");

    AssetRepository assetRepo = new AssetRepository();

    // 1. Add asset.
    System.out.println("Call assetRepo.add()");

    String assetDBKeyHash = assetRepo.add(
      params.get(PublicKeyTag),
      params.get(AssetNameTag),
      params.get(AssetValueTag)
    );
    
    System.out.println("----------------------------------------------");
    System.out.println("Received from C++: assetDBKeyHash: " + assetDBKeyHash);
    System.out.println("----------------------------------------------");

    // 2. Find asset data by uuid.
    System.out.println("Call assetRepo.findByUuid()");
    HashMap<String, String> assetMap = assetRepo.findByUuid(
      assetDBKeyHash
    );

    System.out.println("----------------------------------------------");
    System.out.println("Received from C++: found pubKey:     " + assetMap.get(PublicKeyTag));
    System.out.println("Received from C++: found assetName:  " + assetMap.get(AssetNameTag));
    System.out.println("Received from C++: found assetValue: " + assetMap.get(AssetValueTag));
    System.out.println("----------------------------------------------");

    // 3. Then, verify integrity.
    assert assetMap.get(PublicKeyTag).equals(params.get(PublicKeyTag));
    assert assetMap.get(AssetNameTag).equals(params.get(AssetNameTag));
    assert assetMap.get(AssetValueTag).equals(params.get(AssetValueTag));

    System.out.println("Success assetions of integrity.");
    System.out.println("----------------------------------------------");
  }

  public static void main(String[] argv) {
    System.out.println("Hello in JAVA!");
  }

}
