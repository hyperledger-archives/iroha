
#include <crow.h>
#include <string>

#include "http_server.hpp"

namespace http {
  
  namespace response{
    crow::json::wvalue error(std::string message){
      crow::json::wvalue res;
      res["status"] = 400;
      res["message"] = message;
      return res;
    }
  };
  void server() {
    crow::SimpleApp app;

    // **************
    // Member service
    // **************

    // Register
    CROW_ROUTE(app, "/account/register")
      .methods(crow::HTTPMethod::POST)
      ([](const crow::request& req) {
        auto data = crow::json::load(req.body);
        // WIP
    });

    // Info
    CROW_ROUTE(app, "/account")
      .methods(crow::HTTPMethod::GET)
      ([](const crow::request& req) {
        if(req.url_params.get("uuid") != nullptr) {
         // WIP 
        }else{
          return response::error("You must set 'uuid' in url params");
        }
    });


    // **************
    //  Transaction
    // **************
    // Asset operation
    CROW_ROUTE(app, "/transaction")
      .methods(crow::HTTPMethod::POST)
      ([](const crow::request& req) {
        auto data = crow::json::load(req.body);
        // WIP
        return "Root";
    });

    // Transaction history
    CROW_ROUTE(app, "/transaction/hostory")
      .methods(crow::HTTPMethod::GET)
      ([](const crow::request& req) {
        if(req.url_params.get("uuid") != nullptr &&
          req.url_params.get("asset-uuid") != nullptr){
          // WIP
        }else{
          return response::error("You must set 'uuid' and 'asset-uuid' in url params");
    });


    // **************
    //      Gift
    // **************
    // Nonce generator
    CROW_ROUTE(app, "/gift/issue")
      .methods(crow::HTTPMethod::POST)
      ([](const crow::request& req) {
      // WIP
      return "Root";
    });

    // Nonce receiver
    CROW_ROUTE(app, "/gift/receive")
      .methods(crow::HTTPMethod::POST)
      ([](const crow::request& req) {
        auto data = crow::json::load(req.body);
        // WIP
        return "Root";
    });

    app.port(1337).multithreaded().run();
  }
};  // namespace http
