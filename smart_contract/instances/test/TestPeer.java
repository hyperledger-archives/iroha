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

public class TestPeer {

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

  public static void testAddPeer(HashMap<String, String> params, HashMap<String, String> trust) throws IllegalStateException {
    try {
      // Print received params
      System.out.println("----------------------------------------------");
      System.out.println("Params: " + params);
      System.out.println("Params trust value: " + trust);
      System.out.println("----------------------------------------------");

      // 1. Add account.
      System.out.println("Call peerRepo.add()");

      String uuid = repository.peerAdd(
        params,
        trust
      );

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: uuid: " + uuid);
      System.out.println("----------------------------------------------");

      // 2. Find account data by uuid.
      System.out.println("Call repository.peerFindByUuid()");
      HashMap<String, String> uuidmap = new HashMap<String, String>();
      uuidmap.put(Uuid, uuid);
      HashMap<String, String> peerInfoMap  = repository.peerInfoFindByUuid(uuidmap);
      HashMap<String, String> peerTrustMap = repository.peerTrustFindByUuid(uuidmap);

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: " + params);
      System.out.println("----------------------------------------------");

      // 3. Ensure the integrity.
      if (!peerInfoMap.get(PublicKey).equals(params.get(PublicKey)))
        throw new IllegalStateException("Mismatch public key");

      if (!peerInfoMap.get(PeerAddress).equals(params.get(PeerAddress)))
        throw new IllegalStateException("Mismatch peer address");

      if (!peerTrustMap.get(PeerTrustIsOk).equals(trust.get(PeerTrustIsOk)))
        throw new IllegalStateException("Mismatch peer trust isOk");

      if (!peerTrustMap.get(PeerTrustValue).equals(trust.get(PeerTrustValue)))
        throw new IllegalStateException("Mismatch peer trust value");

      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }

  public static void testUpdatePeer(HashMap<String, String> params, HashMap<String, String> trust) throws IllegalStateException {
    try {
      // Print received params
      System.out.println("----------------------------------------------");
      System.out.println("Params:       " + params);
      System.out.println("Params trust: " + trust);
      System.out.println("----------------------------------------------");

      // 1. Add account.
      System.out.println("Call repository.peerAdd()");

      if (!repository.peerUpdate(
        params,
        trust
      )) throw new IllegalStateException("Cannot update peer");

      // 2. Find account data by uuid.
      System.out.println("Call accountRepo.findByUuid()");
      HashMap<String, String> uuidmap = new HashMap<String, String>();
      uuidmap.put(Uuid, params.get(Uuid));
      HashMap<String, String> peerInfoMap = repository.peerInfoFindByUuid(uuidmap);
      HashMap<String, String> peerValueMap = repository.peerTrustFindByUuid(uuidmap);

      System.out.println("----------------------------------------------");
      System.out.println("Received from C++: " + peerInfoMap);
      System.out.println("Received from C++: " + peerValueMap);
      System.out.println("----------------------------------------------");

      // 3. Ensure the integrity.
      
      if (! peerInfoMap.get(PeerAddress).equals(params.get(PeerAddress)))
        throw new IllegalStateException("Mismatch peer address");

      if (! peerValueMap.get(PeerTrustValue).equals(trust.get(PeerTrustValue)))
        throw new IllegalStateException("Mismatch peer trust value");

      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }

  public static void testRemovePeer(HashMap<String, String> params) throws IllegalStateException {
    try {
      // Print received params
      System.out.println("----------------------------------------------");
      System.out.println("Params uuid: " + params);
      System.out.println("----------------------------------------------");

      HashMap<String, String> uuidmap = new HashMap<String, String>();
      uuidmap.put(Uuid, params.get(Uuid));
      if (!repository.peerRemove(uuidmap))
        throw new IllegalStateException("Cannot remove account");
      
      printSuccess();
    } catch(IllegalStateException e) {
      printFail(e);
    }
  }
  /***************************************************************************************************
   * From java main function
   ***************************************************************************************************/

  // Test 'add' only. All command tests are invoked by gTest.
  private static void javaIntegrityCheckAddAccount() {
    try {
      HashMap<String, String> params = new HashMap<String, String>();
      params.put(PublicKey,   "This is Public key.");
      params.put(PeerAddress, "this is Peer Addr");
      
      HashMap<String, String> trust = new HashMap<String, String>();
      trust.put(PeerTrustValue, "1.234567890987654321");
      trust.put(PeerTrustIsOk,  "true");

      testAddPeer(params, trust);

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
    javaIntegrityCheckAddAccount();
    System.out.println("Call C++ gTest is also required.");
  }

}
