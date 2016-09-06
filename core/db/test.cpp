#include "repository.hpp"
#include <iostream>

int main(){
  auto repository = Repository<std::string>();
  std::cout <<"add:"<< repository.add("test","mizuki") << std::endl;
  std::cout <<"find:"<< *repository.find("test") << std::endl;
  std::cout <<"update:"<< repository.update("test", "mizukisonoko") << std::endl;
  std::cout <<"find:"<< *repository.find("test") << std::endl;
  std::cout <<"remove:"<< repository.remove("test") << std::endl;
  return  0;
}
