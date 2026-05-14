#ifndef LOSEV_SIP_HASH_HPP
#define LOSEV_SIP_HASH_HPP

#include <cstdint>
#include <string>
#include <utility>

namespace losev {

namespace detail
{

inline uint64_t rotl(uint64_t x, int b)
{
  return (x << b) | (x >> (64 - b));
}

inline void sipRound(uint64_t& v0, uint64_t& v1, uint64_t& v2, uint64_t& v3)
{
  v0 += v1;
  v1 = rotl(v1, 13);
  v1 ^= v0;
  v0 = rotl(v0, 32);
  v2 += v3;
  v3 = rotl(v3, 16);
  v3 ^= v2;
  v0 += v3;
  v3 = rotl(v3, 21);
  v3 ^= v0;
  v2 += v1;
  v1 = rotl(v1, 17);
  v1 ^= v2;
  v2 = rotl(v2, 32);
}

}  // namespace detail

inline uint64_t sipHash24(const uint8_t* data, size_t len,
                          uint64_t k0 = 0x0706050403020100ull,
                          uint64_t k1 = 0x0f0e0d0c0b0a0908ull)
{
  uint64_t v0 = k0 ^ 0x736f6d6570736575ull;
  uint64_t v1 = k1 ^ 0x646f72616e646f6dull;
  uint64_t v2 = k0 ^ 0x6c7967656e657261ull;
  uint64_t v3 = k1 ^ 0x7465646279746573ull;
  uint64_t m;
  size_t i;
  const uint8_t* end = data + len - (len % 8);

  for (i = 0; data + i < end; i += 8) {
    m = 0;
    for (int j = 0; j < 8; ++j) {
      m |= static_cast<uint64_t>(data[i + j]) << (8 * j);
    }
    v3 ^= m;
    detail::sipRound(v0, v1, v2, v3);
    detail::sipRound(v0, v1, v2, v3);
    v0 ^= m;
  }

  m = static_cast<uint64_t>(len) << 56;
  for (size_t j = 0; j < len % 8; ++j) {
    m |= static_cast<uint64_t>(data[i + j]) << (8 * j);
  }

  v3 ^= m;
  detail::sipRound(v0, v1, v2, v3);
  detail::sipRound(v0, v1, v2, v3);
  v0 ^= m;
  v2 ^= 0xff;
  detail::sipRound(v0, v1, v2, v3);
  detail::sipRound(v0, v1, v2, v3);
  detail::sipRound(v0, v1, v2, v3);
  detail::sipRound(v0, v1, v2, v3);

  return v0 ^ v1 ^ v2 ^ v3;
}

inline uint64_t hashString(const std::string& str)
{
  return sipHash24(reinterpret_cast<const uint8_t*>(str.data()), str.size());
}

inline uint64_t hashPair(const std::pair<std::string, std::string>& p)
{
  uint64_t h1 = hashString(p.first);
  uint64_t h2 = hashString(p.second);
  return h1 ^ (h2 + 0x9e3779b97f4a7c15ull);
}

template<typename Key>
struct SipHash;

template<>
struct SipHash<std::string>
{
  uint64_t operator()(const std::string& key) const
  {
    return hashString(key);
  }
};

template<>
struct SipHash<std::pair<std::string, std::string>>
{
  uint64_t operator()(const std::pair<std::string, std::string>& key) const
  {
    return hashPair(key);
  }
};

}

#endif
