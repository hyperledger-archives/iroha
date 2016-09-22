

#define CROW_ENABLE_SSL

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

    crow::json::wvalue simple_mock(std::string message){
      crow::json::wvalue res;
      res["status"] = 200;
      res["message"] = message;
      return res;
    }

    namespace mock{
      crow::json::wvalue account_register(std::string name){
        crow::json::wvalue res;
        if(name == "mizuki"){
          res["status"] = 400;
          res["message"] = "duplicate user!";
        }else{
          res["status"] = 200;
          res["message"] = "successful";
          res["uuid"] = "bd56fbc1c356368b0d9e9311c5b787e1db0cabd697dad274dcc1b0da94ccb96c04a73ef13be6e7606e48d43d518ad302bf8509818d907c6cbf00b61b984e36b9";
        }
        return res;
      }
    
      crow::json::wvalue account(std::string uuid){
        crow::json::wvalue res;
        if(uuid == "bd56fbc1c356368b0d9e9311c5b787e1db0cabd697dad274dcc1b0da94ccb96c04a73ef13be6e7606e48d43d518ad302bf8509818d907c6cbf00b61b984e36b9"){
          res["status"] = 200;
          res["screen_name"] = "mizuki";
          res["message"] = "successful";
        }else{
          res["status"] = 400;
          res["message"] = "User not found!";
        }
        return res;
      };
      
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
        if(!data){
          return response::error("No json");
        }
        if(data.has("publicKey") &&
            data.has("screen_name") &&
              data.has("timestamp")){
            
            std::string pubKey = data["publicKey"].s();
            std::string name = data["publicKey"].s();
            unsigned int timestamp = data["publicKey"].u();
            return response::mock::account_register(name);
        }
        // WIP
        return response::simple_mock("Not enough value");
    });

    // Info
    CROW_ROUTE(app, "/account")
      .methods(crow::HTTPMethod::GET)
      ([](const crow::request& req) {
        if(req.url_params.get("uuid") != nullptr) {
          return response::mock::account(req.url_params.get("uuid"));
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
        return response::simple_mock("OK");
    });

    // Transaction history
    CROW_ROUTE(app, "/transaction/hostory")
      ([](const crow::request& req) {
        if(req.url_params.get("uuid") != nullptr &&
          req.url_params.get("asset-uuid") != nullptr){
          // WIP
          return response::simple_mock("OK");
        }else{
          return response::error("You must set 'uuid' and 'asset-uuid' in url params"
);
        }
    });


    // **************
    //      Gift
    // **************
    // Nonce generator
    CROW_ROUTE(app, "/gift/issue")
      .methods(crow::HTTPMethod::POST)
      ([](const crow::request& req) {
      // WP
      return response::simple_mock("OK");
    });

    // Nonce receiver
    CROW_ROUTE(app, "/gift/receive")
      .methods(crow::HTTPMethod::POST)
      ([](const crow::request& req) {
        auto data = crow::json::load(req.body);
        // WIP
        return response::simple_mock("OK");
    });

    app.port(443).ssl_file("/var/key/.crt", "/var/key/server.key").run();
  }
};  // namespace http
