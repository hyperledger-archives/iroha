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
import repository.AccountRepository;

public class Test {

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

  public static void test4(HashMap<String,String> params){
    System.out.println("Hello in JAVA! test4() ");
    AccountRepository accountRepo = new AccountRepository();
    System.out.println("Java: AccountRepository add: " + params.get("key"));
    accountRepo.add(params.get("key"), "MizukiSonoko");
  }

  public static void main(String[] argv) {
    System.out.println("Hello in JAVA! check repositories");
    System.out.println("AssetRepository");
    AccountRepository account = new AccountRepository();
    account.add("MPTt3ULszCLGQqAqRgHj2gQHVnxn/DuNlRXR/iLMAn4=", "MyAsset");
  }

}
