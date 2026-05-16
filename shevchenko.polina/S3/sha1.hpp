#ifndef SHA1_HPP
#define SHA1_HPP

#include "sha1_c.h"
#include <string>
#include <vector>
#include <cstdint>

namespace shevchenko {

class SHA1 {
public:
  SHA1() {
    SHA1Reset(&ctx_);
  }
  
  void update(const std::string& data) {
    update(reinterpret_cast<const uint8_t*>(data.c_str()), data.size());
  }
  
  void update(const uint8_t* data, size_t len) {
    SHA1Input(&ctx_, data, static_cast<unsigned int>(len));
  }
  
  std::vector<uint8_t> final() {
    std::vector<uint8_t> digest(20);
    SHA1Result(&ctx_, digest.data());
    return digest;
  }

  void final(uint8_t digest[20]) {
    SHA1Result(&ctx_, digest);
  }
  
private:
  SHA1Context ctx_;
};

inline std::vector<uint8_t> sha1_hash(const std::string& input) {
  SHA1 sha;
  sha.update(input);
  return sha.final();
}

class SHA1Hasher {
public:
  size_t operator()(const std::string& key) const {
    auto hash_bytes = sha1_hash(key);

    size_t result = 0;
    for (size_t i = 0; i < sizeof(size_t) && i < hash_bytes.size(); ++i) {
      result = (result << 8) | hash_bytes[i];
    }
    return result;
  }
};

}

#endif
