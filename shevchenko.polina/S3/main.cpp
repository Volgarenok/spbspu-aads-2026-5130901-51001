#include "sha1.hpp"
#include <iostream>
#include <iomanip>

int main() {

  shevchenko::SHA1 sha;
  sha.update("abc");
  auto digest = sha.final();
  
  std::cout << "SHA-1(\"abc\") via class = ";
  for (unsigned char c : digest) {
    std::cout << std::hex << std::setw(2) << std::setfill('0')
    << (int)c;
  }
  std::cout << "\n";
  
  auto hash2 = shevchenko::sha1_hash("abc");
  std::cout << "SHA-1(\"abc\") via function = ";
  for (unsigned char c : hash2) {
    std::cout << std::hex << std::setw(2) << std::setfill('0')
    << (int)c;
  }
  std::cout << "\n";

  shevchenko::SHA1Hasher hasher;
  std::cout << "Hash of \"abc\" as size_t = " << hasher("abc") << "\n";
  
  return 0;
}
