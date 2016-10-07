#ifndef __CONNECTION__
#define __CONNECTION__

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>

namespace connection {
void initialize_peer(const std::unordered_map<std::string, std::string>& config);

bool sendAll(std::string message);
bool send(std::string to, std::string message);
bool receive(std::function<void(std::string from, std::string message)> callback);

};  // end connection

#endif
