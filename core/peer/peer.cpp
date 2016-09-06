#include <string>
#include <iostream>
#include <unistd.h>

#include <yaml-cpp/yaml.h>
#include "connection.hpp"
#include "../consensus/pbft-sieve.hpp"

bool loadYamlIsLeader(){
  try{
    YAML::Node config = YAML::LoadFile("config.yml");
    return config["peer"]["leader"].as<bool>();
  }catch(YAML::Exception& e) {
      std::cerr << e.what() << std::endl;
      exit(1);
  }
  return false;
}

int main(){
  std::cout<<"Process ID is "<< getpid() << std::endl;
  Connection::initialize_peer();
  int forkHttp = fork();
  if(forkHttp == 0){
    std::cout << "Created process ID is " << getpid() << std::endl;
    std::cout << "Parent process ID of created process is " <<getppid()<< std::endl;
    // Web Front
    exit(0);
  }else if(forkHttp == -1){
    std::cerr <<" fork failed. "<< std::endl;
    exit(1);
  }

  std::cout << "(Second) Process ID is " << getpid() << std::endl;
  pbft_sieve::initialize_pbft_sieve(11, 2, 2);
  pbft_sieve::loop();

  int status;
  int child_pid;
  bool isFinish = false;
  printf("parent process\n");
  while(isFinish){
    child_pid = waitpid(-1,&status,WNOHANG);
    if(child_pid > 0){
      isFinish = true;
      std::cout << "PID "<< child_pid <<" done" << std::endl;
    }else{
      std::cout << "No child exited" << std::endl;
    }
    sleep(1);
  }
  return 0;
}
