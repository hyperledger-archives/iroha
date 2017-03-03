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

public class TestDomain {

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

  public static void testAddDomain(HashMap<String, String> params) throws IllegalStateException {
    try {
      // Print received params
      System.out.println("----------------------------------------------");
      System.out.println("Params ownerPubKey: " + params.get(OwnerPublicKey));
      System.out.println("Params domainName: " + params.get(DomainName));
      System.out.println("----------------------------------------------");

      // 1. Add domain.s
      System.out.println("Call repository.domainAdd()");

      String uuid = repository.domainAdd(
        params
      );

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: uuid: " + uuid);
      System.out.println("----------------------------------------------");

      // 2. Find account data by uuid.
      System.out.println("Call repoisitory.domainFindByUuid()");
      HashMap<String, String> uuidmap = new HashMap<String, String>();
      uuidmap.put(Uuid, uuid);
      HashMap<String, String> domainMap = repository.domainFindByUuid(uuidmap);

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: found ownerPubKey: " + domainMap.get(OwnerPublicKey));
      System.out.println("Received from C++: found domainName:  " + domainMap.get(DomainName));
      System.out.println("----------------------------------------------");

      // 3. Ensure the integrity.
      /*
      assert accountMap.get(PublicKey).equals(params.get(PublicKey));
      assert accountMap.get(AccountName).equals(params.get(AccountName));
      */
      if (!domainMap.get(OwnerPublicKey).equals(params.get(OwnerPublicKey)))
        throw new IllegalStateException("Mismatch ownerPublicKey");

      if (!domainMap.get(DomainName).equals(params.get(DomainName)))
        throw new IllegalStateException("Mismatch domain name");

      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }

  public static void testUpdateDomain(HashMap<String, String> params) throws IllegalStateException {
    try {
      // Print received params
      System.out.println("----------------------------------------------");
      System.out.println("Params domainName: " + params.get(DomainName));
      System.out.println("----------------------------------------------");

      // 1. Update.
      System.out.println("Call repository.domainAdd()");

      if (!repository.domainUpdate(
        params
      )) throw new IllegalStateException("Cannot update domain");

      // 2. Find data by uuid.
      System.out.println("Call repository.domainFindByUuid()");
      HashMap<String, String> uuidmap = new HashMap<String, String>();
      uuidmap.put(Uuid, params.get(Uuid));
      HashMap<String, String> domainMap = repository.domainFindByUuid(uuidmap);

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: found ownerPubKey: " + domainMap.get(OwnerPublicKey));
      System.out.println("Received from C++: found domainName:  " + domainMap.get(DomainName));
      System.out.println("----------------------------------------------");

      // 3. Ensure the integrity.
      
      if (!domainMap.get(DomainName).equals(params.get(DomainName)))
        throw new IllegalStateException("Mismatch domain name");

      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }

  public static void testRemoveDomain(HashMap<String, String> params) throws IllegalStateException {
    try {
      // Print received params
      System.out.println("----------------------------------------------");
      System.out.println("Params uuid: " + params.get(Uuid));
      System.out.println("----------------------------------------------");

      HashMap<String, String> uuidmap = new HashMap<String, String>();
      uuidmap.put(Uuid, params.get(Uuid));
      if (!repository.domainRemove(uuidmap))
        throw new IllegalStateException("Cannot remove domain");
      
      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }
  /***************************************************************************************************
   * From java main function
   ***************************************************************************************************/

  // Test 'add' only. All command tests are invoked by gTest.
  private static void javaIntegrityCheckAddDomain() {
    try {
      HashMap<String, String> domainParam = new HashMap<String, String>();
      domainParam.put(OwnerPublicKey, "This is owner public key.");
      domainParam.put(DomainName, "this is domain name");

      testAddDomain(domainParam);

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
    javaIntegrityCheckAddDomain();
    System.out.println("Call C++ gTest is also required.");
  }

}
