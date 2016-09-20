
#include <thread>

#include "../core/server/http_server.hpp"


void server(){
  http::server();
}

int main(){
  std::thread http_th( server );

  // WIP
  while(1){}

  http_th.detach();
  return 0;
}

