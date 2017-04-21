#include <iostream>
#include "expected/expected.hpp"

Expected<std::string> definitelyEven(int number) {
  if (number % 2 == 0) {
    return std::string("OK. Number is even.");
  }
  return makeUnexpected(exception::IrohaException("Number is not even."));
}

int main() {
  while (1) {
    int x; std::cin >> x;
    auto ret = definitelyEven(x);
    if (ret) {
      std::cout << *ret << std::endl;
    } else {
      std::cerr << "Error: " << ret.message() << std::endl;
    }
  }
}
