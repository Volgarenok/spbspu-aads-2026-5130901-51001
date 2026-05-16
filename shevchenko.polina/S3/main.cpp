#include "sha1.hpp"
#include "hash_table.hpp"
#include <iostream>

int main() {

  shevchenko::HashTable<std::string, int, shevchenko::SHA1Hasher> table;
  
  std::cout << table.bucket_count() << " buckets" << "\n";
  std::cout << "size: " << table.size() << "\n";
  std::cout << (table.empty() ? "yes" : "no") << "\n";
  
  return 0;
}
