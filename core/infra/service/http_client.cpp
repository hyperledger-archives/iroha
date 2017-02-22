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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/param.h>
#include <unistd.h>

#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#include <service/http_client.hpp>

#include <util/logger.hpp>

namespace http_client {
    using nlohmann::json;

    const int BUFFER_LENGTH = 2048;

    int GET(std::string dest, int port, std::string path) {

        struct hostent *hostent;
        struct sockaddr_in server;

        int fd;
        char buffer[BUFFER_LENGTH];

        hostent = gethostbyname(dest.c_str()); /* lookup IP */
        if (hostent == nullptr) {
            logger::error("HttpClient") <<  "Cannot resolve [" << dest << "]";
            return 1;
        }
        bzero(&server, sizeof(server));
        server.sin_family = AF_INET;
        bcopy(hostent->h_addr, &server.sin_addr, hostent->h_length);
        server.sin_port = htons(port);

        if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            logger::error("HttpClient") <<  "Socker error";
            return 1;
        }
        if(connect(fd, (struct sockaddr *) &server, sizeof(server)) == -1) {
            logger::error("HttpClient") <<  "Connection failed";
            return 1;
        }

        std::string message = "GET " + path + " HTTP/1.0\r\n";
        write(fd, message.c_str(), message.size());

        std::string response = "";
        while(read(fd, buffer, BUFFER_LENGTH) > 0) {
            response += buffer;
        }

        logger::debug("HttpClient") <<  "response [\n" << response << "\n]";

        close(fd);
        return 0;
    }

    int POST(std::string dest, int port, std::string path) {
        struct hostent *hostent;
        struct sockaddr_in server;

        int fd;
        char buffer[BUFFER_LENGTH];

        hostent = gethostbyname(dest.c_str()); /* lookup IP */
        if (hostent == nullptr) {
            logger::error("HttpClient") <<  "Cannot resolve [" << dest << "]";
            return 1;
        }
        bzero(&server, sizeof(server));
        server.sin_family = AF_INET;
        bcopy(hostent->h_addr, &server.sin_addr, hostent->h_length);
        server.sin_port = htons(port);

        if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            logger::error("HttpClient") <<  "Socker error";
            return 1;
        }
        if(connect(fd, (struct sockaddr *) &server, sizeof(server)) == -1) {
            logger::error("HttpClient") <<  "Connection failed";
            return 1;
        }

        // -- SSL --
        SSL_load_error_strings();
        SSL_library_init();
        auto context = SSL_CTX_new(SSLv23_client_method());
        auto ssl = SSL_new(context);

        if (SSL_set_fd(ssl, fd) == 0) {
            ERR_print_errors_fp(stderr);
            return 1;
        }
        if (SSL_connect(ssl) != 1) {
            ERR_print_errors_fp(stderr);
            return 1;
        }

        // WIP
        // ----------
        std::string message = "POST " + path + " HTTP/1.1\n"
          + "Host: " + dest + "\n"
          + "User-Agent: iroha\n"
          + "Accept: */*\n"
          + "Content-Type: application/x-www-form-urlencoded\n"
          + "\n";
        logger::info("HttpClient") <<"dest! [\n" <<  dest <<":" << port << path << "\n ]";
        logger::info("HttpClient") <<"send! [\n" <<  message <<  "]";
        SSL_write(ssl, message.c_str(), message.size());

        logger::info("HttpClient") <<"Sended!";

        std::string response = "";

        int read_len;
        do {
            read_len = SSL_read(ssl, buffer, BUFFER_LENGTH);
            response += buffer;
            logger::info("HttpClient") <<"recv " << buffer;
            logger::info("HttpClient") <<"size " << read_len;
        } while(read_len > 0);

        logger::info("HttpClient") <<  "response [\n" << response << "\n]";

        // -- SSL --
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(context);
        ERR_free_strings();
        // ---------
        close(fd);
        return 0;

    }

    int POST(std::string dest, int port, std::string path, json data) {

        struct hostent *hostent;
        struct sockaddr_in server;

        int fd;
        char buffer[BUFFER_LENGTH];

        hostent = gethostbyname(dest.c_str()); /* lookup IP */
        if (hostent == nullptr) {
            logger::error("HttpClient") <<  "Cannot resolve [" << dest << "]";
            return 1;
        }
        bzero(&server, sizeof(server));
        server.sin_family = AF_INET;
        bcopy(hostent->h_addr, &server.sin_addr, hostent->h_length);
        server.sin_port = htons(port);

        if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            logger::error("HttpClient") <<  "Socker error";
            return 1;
        }
        if(connect(fd, (struct sockaddr *) &server, sizeof(server)) == -1) {
            logger::error("HttpClient") <<  "Connection failed";
            return 1;
        }

        // -- SSL --
        SSL_load_error_strings();
        SSL_library_init();
        auto context = SSL_CTX_new(SSLv23_client_method());
        auto ssl = SSL_new(context);

        if (SSL_set_fd(ssl, fd) == 0) {
            ERR_print_errors_fp(stderr);
            return 1;
        }
        if (SSL_connect(ssl) != 1) {
            ERR_print_errors_fp(stderr);
            return 1;
        }

        // WIP
        // ----------
        std::string message = "POST " + path + " HTTP/1.1\n"
        + "Host: " + dest + "\n"
        + "User-Agent: iroha\n"
        + "Accept: */*\n"
        + "Content-Length: " + std::to_string(data.dump().size()) + "\n"
        + "Content-Type: application/x-www-form-urlencoded\n"
        + "\n";
        logger::info("HttpClient") <<"dest! [\n" <<  dest <<":" << port << path << "\n ]";
        logger::info("HttpClient") <<"send! [\n" <<  message <<  "]";
        SSL_write(ssl, message.c_str(), message.size());

        SSL_write(ssl, data.dump().c_str(), data.dump().size());

        logger::info("HttpClient") <<"Sended!";

        std::string response = "";

        int read_len;
        do {
            read_len = SSL_read(ssl, buffer, BUFFER_LENGTH);
            response += buffer;
            logger::info("HttpClient") <<"recv " << buffer;
            logger::info("HttpClient") <<"size " << read_len;
        } while(read_len > 0);

        logger::info("HttpClient") <<  "response [\n" << response << "\n]";

        // -- SSL --
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(context);
        ERR_free_strings();
        // ---------
        close(fd);
        return 0;


    }
}