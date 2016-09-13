#include <crow.h>

#include "HTTPServer.hpp"

namespace HTTP {
  void server() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Root";
    });

    app.port(1337).multithreaded().run();
  }
}  // namespace http
