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

package instances.sample_rating;

import java.util.HashMap;

import repository.Repository;
import static repository.KeyConstants.*;

/*
- WeightingRate.java
    - 要件：
        - 初期条件: R = 1, alpha = 1.25, C = 10
        - invokeされるメソッド
            - linearIncrease()
                - R' = alpha * R + C の計算式で更新される
            - hit()
                - alpha++
            - miss()
                1. counter <- max(0, counter - 1)
                2. alpha <- max(1.25, alpha * 0.95)
    - 以下を10回繰り返す
        - 10回 hit(), miss() を実行
        - linearIncrease() を実行
        - Rを表示
*/
public class WeightedRate {

    static Repository repository = new Repository();

    public static void registerAccount(HashMap<String, String> params) {
        final Double initialRate = Double.parseDouble(params("rate").get());
        final int    constRate   = Integer.parseInt(params("constant").get());
    }

    public static void linearIncrease() {

    }

    public static void hit() {

    }

    public static void miss() {

    }

}