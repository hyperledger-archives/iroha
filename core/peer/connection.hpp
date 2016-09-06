#ifndef __CONNECTION__
#define __CONNECTION__

#include <string>
#include <functional>

namespace Connection{
  void initialize_peer();
  bool sendAll(std::string message);
  bool send(std::string to,std::string message);
  void receive(std::function<void(std::string from,std::string message)> callback);
};

#endif
