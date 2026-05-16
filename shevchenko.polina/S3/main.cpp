#include "sha1.hpp"
#include <iostream>

int main() {
  auto hash = shevchenko::sha1_hash("abc");
  std::cout << "SHA-1 hash size: " << hash.size() << "\n";
  return 0;
}
