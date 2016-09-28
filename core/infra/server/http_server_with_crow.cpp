

#define CROW_ENABLE_SSL


#include <crow.h>
#include <json.hpp>

#include <iostream>
#include <string>

#include "../../server/http_server.hpp"

  
namespace http {
  
  using nlohmann::json;

  namespace response{
    nlohmann::json error(std::string message){
      return {
        {"status", 400},
        {"message", message}
      };
    }

    nlohmann::json simple_mock(std::string message){
      return {
        {"status", 200},
        {"message", message}
      };
    }

    namespace mock{
      json account_register(std::string name){
        if(name == "mizuki"){
          return {
            {"status", 400},
            {"message", "duplicate user!"}
          };
        }else{
          return {
            {"status", 200},
            {"message", "successful"},
            {"uuid", "bd56fbc1c356368b0d9e9311c5b787e1db0cabd697dad274dcc1b0da94ccb96c04a73ef13be6e7606e48d43d518ad302bf8509818d907c6cbf00b61b984e36b9"}
          };
        }
      }
    
      json account(std::string uuid){
        if(uuid == "bd56fbc1c356368b0d9e9311c5b787e1db0cabd697dad274dcc1b0da94ccb96c04a73ef13be6e7606e48d43d518ad302bf8509818d907c6cbf00b61b984e36b9"){
          return {
            {"status", 200},
            {"screen_name", "mizuki"},
            {"message", "successful"}
          };
        }else{
          return {
            {"status", 400},
            {"message", "User not found!"}
          };
        }
      };
      
      json transaction(){
        return {
          {"status", 200},
          {"message", "transaction success"}
        };
      }
      
      json transaction_history(){
        return {
          {"uuid", "bd56fbc1c356368b0d9e9311c5b787e1db0cabd697dad274dcc1b0da94ccb96c04a73ef13be6e7606e48d43d518ad302bf8509818d907c6cbf00b61b984e36b9"},
          {"timestamp", 1474522744},
          {"history", 
            {
              {"asset-uuid", "bd56fbc1c356368b0d9e9311c5b787e1db0cabd697dad274dcc1b0da94ccb96c04a73ef13be6e7606e48d43d518ad302bf8509818d907c6cbf00b61b984e36b9"},
              {"params", 
                {
                  {"src", "bd56fbc1c356368b0d9e9311c5b787e1db0cabd697dad274dcc1b0da94ccb96c04a73ef13be6e7606e48d43d518ad302bf8509818d907c6cbf00b61b984e36b9"},
                  {"dst", "bd56fbc1c356368b0d9e9311c5b787e1db0cabd697dad274dcc1b0da94ccb96c04a73ef13be6e7606e48d43d518ad302bf8509818d907c6cbf00b61b984e36b9"},
                  {"value", "123"}
                }
              }
            }
          }
        };
      } 

      json gift_issue(){
        return {
          {"nonce", "697f2d856172cb8309d6b8b97dac4de344b549d4dee61edfb4962d8698b7fa803f4f93ff24393586e28b5b957ac3d1d369420ce53332712f997bd336d09ab02a"},
          {"timestamp", 1474523494}
        };
      }
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
        auto data = json::parse(req.body);
        if(data == nullptr){
          return crow::response(
            response::error("No json").dump()
          );
        }

        if(!data["publicKey"].is_null() &&
            !data["screen_name"].is_null() &&
              !data["timestamp"].is_null()){
            try{
              std::string pubKey = data["publicKey"];
              std::string name = data["screen_name"];
              unsigned int timestamp = data["timestamp"];
              return crow::response(
                response::mock::account_register(name).dump()
              );
            }catch(...){
              return crow::response(
                response::simple_mock("Illegal value type").dump()
              ); 
            }
        }
        // WIP
        return crow::response(
          response::simple_mock("Not enough value").dump()
        );
    });

    // Info
    CROW_ROUTE(app, "/account")
      .methods(crow::HTTPMethod::GET)
      ([](const crow::request& req) {
        if(req.url_params.get("uuid") != nullptr) {
          // WIP
          return crow::response(
            response::mock::account(req.url_params.get("uuid")).dump()
          );
        }else{
          return crow::response(
            response::error("You must set 'uuid' in url params").dump()
          );
        }
    });

    // **************
    //  Transaction
    // **************
    // Asset operation
    CROW_ROUTE(app, "/transaction")
      .methods(crow::HTTPMethod::POST)
      ([](const crow::request& req) {
        auto data = json::parse(req.body);
        // WIP
        if(!data["asset"].is_null()){
          auto asset = data["asset"];
          if(!asset["asset-uuid"].is_null() &&
              !asset["params"].is_null()){
            // WIP  
            auto param = asset["params"];
            return crow::response(
              response::mock::transaction().dump()
            );
          }
        }
        return crow::response(
          response::error("You must set 'uuid' in url params").dump()
        );
    });

    // Transaction history
    CROW_ROUTE(app, "/transaction/hostory")
      ([](const crow::request& req) {
        if(req.url_params.get("uuid") != nullptr &&
          req.url_params.get("asset-uuid") != nullptr){
          // WIP
          return crow::response(
            response::mock::transaction_history().dump()
          );
        }else{
          return crow::response(
            response::error("You must set 'uuid' and 'asset-uuid' in url params").dump()
          );
        }
    });

    app.port(8080).run(); // config 
  }
};  // namespace http
