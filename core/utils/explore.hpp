//
// Created by Taisei Igarashi on 2017/04/22.
//

#ifndef IROHA_EXPLORE_H
#define IROHA_EXPLORE_H

#include <utils/logger.hpp>
#include <string>
namespace explore {
    namespace sumeragi{
        void printJudge(int numValidSignatures, int numValidationPeer, int faulty) {
            std::stringstream resLine[5];
            for (int i = 0; i < numValidationPeer; i++) {
                if (i < numValidSignatures) {
                    resLine[0] << "\033[1m\033[92m+-ー-+\033[0m";
                    resLine[1] << "\033[1m\033[92m| 　 |\033[0m";
                    resLine[2] << "\033[1m\033[92m|-承-|\033[0m";
                    resLine[3] << "\033[1m\033[92m| 　 |\033[0m";
                    resLine[4] << "\033[1m\033[92m+-＝-+\033[0m";
                } else {
                    resLine[0] << "\033[91m+-ー-+\033[0m";
                    resLine[1] << "\033[91m| 　 |\033[0m";
                    resLine[2] << "\033[91m| 否 |\033[0m";
                    resLine[3] << "\033[91m| 　 |\033[0m";
                    resLine[4] << "\033[91m+-＝-+\033[0m";
                }
            }
            for (int i = 0; i < 5; ++i) logger::explore("sumeragi") << resLine[i].str();

            std::string line;
            for (int i = 0; i < numValidationPeer; i++) line += "==＝==";
            logger::explore("sumeragi") << line;

            logger::explore("sumeragi")
                    << "numValidSignatures:" << numValidSignatures << " faulty:" << faulty;
        }

        void printAgree() {
            logger::explore("sumeragi") << "\033[1m\033[92m+==ーー==+\033[0m";
            logger::explore("sumeragi") << "\033[1m\033[92m|+-ーー-+|\033[0m";
            logger::explore("sumeragi") << "\033[1m\033[92m|| 承認 ||\033[0m";
            logger::explore("sumeragi") << "\033[1m\033[92m|+-ーー-+|\033[0m";
            logger::explore("sumeragi") << "\033[1m\033[92m+==ーー==+\033[0m";
        }

        void printReject() {
            logger::explore("sumeragi") << "\033[91m+==ーー==+\033[0m";
            logger::explore("sumeragi") << "\033[91m|+-ーー-+|\033[0m";
            logger::explore("sumeragi") << "\033[91m|| 否認 ||\033[0m";
            logger::explore("sumeragi") << "\033[91m|+-ーー-+|\033[0m";
            logger::explore("sumeragi") << "\033[91m+==ーー==+\033[0m";
        }
    }
};

#endif //IROHA_EXPLORE_H
