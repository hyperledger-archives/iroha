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
#include <gtest/gtest.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include "../../../core/infra/config/peer_service_with_json.hpp"

const std::string IrohaHome = getenv("IROHA_HOME");

// Bad solution. Test data is hard coded.
// Failed to copy json file in build in CI.

TEST(ensure_sumeragi_json_format, normal_sumeragi_json) {
    ASSERT_TRUE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
    /*
    std::ifstream ifs(IrohaHome + "/build/test/infra/config/inputs/normal_sumeragi.json");
    ASSERT_FALSE(ifs.fail());

    std::istreambuf_iterator<char> it(ifs);
    const auto jsonString = std::string(it, std::istreambuf_iterator<char>());
    ASSERT_TRUE(config::PeerServiceConfig::getInstance().ensureConfigFormat(jsonString));
    */
}

TEST(ensure_sumeragi_json_format, bad_json) {    
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":{},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
/*    
    std::ifstream ifs(IrohaHome + "/build/test/infra/config/inputs/bad_json.json");
    ASSERT_FALSE(ifs.fail());

    std::istreambuf_iterator<char> it(ifs);
    const auto jsonString = std::string(it, std::istreambuf_iterator<char>());
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat(jsonString));
*/
}

TEST(ensure_sumeragi_json_format, bad_ip) {
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.987\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4.\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
/*
    std::vector<std::string> fnames = {
      IrohaHome + "/build/test/infra/config/inputs/bad_ip.json",
      IrohaHome + "/build/test/infra/config/inputs/bad_ip2.json",
    };

    for (const auto& path: fnames) {
        std::cout << "\n" << path << "\n";
        std::ifstream ifs(path);
        ASSERT_FALSE(ifs.fail());

        std::istreambuf_iterator<char> it(ifs);
        const auto jsonString = std::string(it, std::istreambuf_iterator<char>());
        ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat(jsonString));
    }
    */
}

TEST(ensure_sumeragi_json_format, missing_key) {
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"iP\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"PublicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"myname\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"rivateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
/*
    const auto filepaths = {
        IrohaHome + "/build/test/infra/config/inputs/no_group_ip.json",
        IrohaHome + "/build/test/infra/config/inputs/no_group_name.json",
        IrohaHome + "/build/test/infra/config/inputs/no_group_publicKey.json",
        IrohaHome + "/build/test/infra/config/inputs/no_me_ip.json",
        IrohaHome + "/build/test/infra/config/inputs/no_me_name.json",
        IrohaHome + "/build/test/infra/config/inputs/no_me_privateKey.json",
        IrohaHome + "/build/test/infra/config/inputs/no_me_publicKey.json",
    };

    for (const auto& path: filepaths) {
        std::cout << "\n" << path << "\n";
        std::ifstream ifs(path);
        ASSERT_FALSE(ifs.fail()) << "path: " << path;

        std::istreambuf_iterator<char> it(ifs);
        const auto jsonString = std::string(it, std::istreambuf_iterator<char>());
        ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat(jsonString));
    }
    */
}

TEST(ensure_sumeragi_json_format, useless_key) {
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\",\"aaaa\":\"hogehoge\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
    ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat("{\"me\":{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\",\"privateKey\":\"aGIuSZRhnGfFyeoKNm/NbTylnAvRfMu3KumOEfyT2HPf36jSF22m2JXWrdCmKiDoshVqjFtZPX3WXaNuo9L8WA==\"},\"group\":[{\"ip\":\"172.17.0.3\",\"name\":\"mizuki\",\"publicKey\":\"jDQTiJ1dnTSdGH+yuOaPPZIepUj1Xt3hYOvLQTME3V0=\"},{\"ip\":\"172.17.0.4\",\"name\":\"natori\",\"publicKey\":\"Q5PaQEBPQLALfzYmZyz9P4LmCNfgM5MdN1fOuesw3HY=\"},{\"ip\":\"172.17.0.5\",\"name\":\"kabohara\",\"publicKey\":\"f5MWZUZK9Ga8XywDia68pH1HLY/Ts0TWBHsxiFDR0ig=\"},{\"ip\":\"172.17.0.6\",\"name\":\"samari\",\"key123456\":\"98765\",\"publicKey\":\"Sht5opDIxbyK+oNuEnXUs5rLbrvVgb2GjSPfqIYGFdU=\"}]}"));
    /*
    const auto filepaths = {
        IrohaHome + "/build/test/infra/config/inputs/useless_me_key.json",
        IrohaHome + "/build/test/infra/config/inputs/useless_group_key.json",
    };

    for (const auto& path: filepaths) {
        std::cout << "\n" << path << "\n";
        std::ifstream ifs(path);
        ASSERT_FALSE(ifs.fail()) << "path: " << path;

        std::istreambuf_iterator<char> it(ifs);
        const auto jsonString = std::string(it, std::istreambuf_iterator<char>());
        ASSERT_FALSE(config::PeerServiceConfig::getInstance().ensureConfigFormat(jsonString));
    }
    */
}
