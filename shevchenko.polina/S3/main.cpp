#include "sha1_c.h"
#include <iostream>
#include <iomanip>
#include <cstring>

int main() {
  // "abc" из RFC (a9993e364706816aba3e25717850c26c9cd0d89d)
  SHA1Context ctx;
  SHA1Reset(&ctx);
  
  const char* test = "abc";
  SHA1Input(&ctx, (const uint8_t*)test, strlen(test));
  
  uint8_t digest[20];
  SHA1Result(&ctx, digest);
  
  std::cout << "SHA-1(\"abc\") = ";
  for (int i = 0; i < 20; i++) {
    std::cout << std::hex << std::setw(2) << std::setfill('0')
    << (int)digest[i];
  }
  std::cout << std::endl;
  
  return 0;
}
