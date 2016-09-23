#ifndef __CONNECTION__
#define __CONNECTION__

#include <string>
#include <memory>
#include <functional>

namespace connection {

  struct Config {
    std::string  address;
    std::string     port;
    std::string     name;

    int publishChannel;
    int publishStreamId;
    int subscribeChannel;
    int subscribeStreamId;
  };

  void initialize_peer(std::unique_ptr<Config> config);

  bool sendAll(std::string message);
  bool send(std::string to,std::string message);
  bool receive(std::function<void(std::string from,std::string message)> callback);

};

#endif
