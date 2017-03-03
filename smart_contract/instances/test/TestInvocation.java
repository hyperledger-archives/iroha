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

import java.util.HashMap;

public class TestInvocation {

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

  // Test invoke function
  public static void test1() {
    System.out.println("Hello in JAVA! test1()");
  }

  // Test invoke function(HashMap<String,String>)
  public static void test2(HashMap<String,String> params) throws IllegalStateException {
    try {
      System.out.println("Hello in JAVA! test2()");
      if (! params.get("key1").equals("Mizuki"))
        throw new IllegalStateException("Failed 'key1' value");

      if (! params.get("key2").equals("Sonoko"))
        throw new IllegalStateException("Failed 'key2' value");

      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }

  // Test invoke function(HashMap<String,String>) (UTF-8)
  public static void test3(HashMap<String, String> params) throws IllegalStateException {
    try {
      System.out.println("Hello in JAVA! test3()");
      if (! params.get("key1").equals("水樹"))
        throw new IllegalStateException("Failed 'key1' value");

      if (! params.get("key2").equals("素子"))
        throw new IllegalStateException("Failed 'key2' value");

      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }
  
}
