
#include <thread>
#include <signal.h>
#include <unistd.h>
#include <atomic>

#include "../core/server/http_server.hpp"

std::atomic_bool running(true); 

void server(){
  http::server();
}

void sigIntHandler(int param){
  running = false;
  //std::cout <<"Halt"<< std::endl;
}

int main() {
  signal(SIGINT, sigIntHandler);

  //std::cout<<"Process ID is "<< getpid() << std::endl;
  //connection::initialize_peer();

  std::thread http_th( server );

 // std::cout << "(Second) Process ID is " << getpid() << std::endl;
 // Sumeragi::initializeSumeragi(11, 2, 2);
 //Sumeragi::loop();

  while(running){}

  http_th.detach();
  return 0;
}

