#ifndef XXHASH_HPP
#define XXHASH_HPP

#include <cstdint>
#include <cstring>
#include <string>
#include <utility>

namespace loseva {

using EdgeKey = std::pair< std::string, std::string >;

class XxHash32 {
public:
  static constexpr std::uint32_t PRIME1 = 2654435761U;
  static constexpr std::uint32_t PRIME2 = 2246822519U;
  static constexpr std::uint32_t PRIME3 = 3266489917U;
  static constexpr std::uint32_t PRIME4 =  668265263U;
  static constexpr std::uint32_t PRIME5 =  374761393U;

  std::size_t operator()(const void * data, std::size_t len,
    std::uint32_t seed = 0) const
  {
    const auto * p = static_cast< const std::uint8_t * >(data);
    const auto * end = p + len;
    std::uint32_t h32 = 0;

    if (len >= 16) {
      std::uint32_t v1 = seed + PRIME1 + PRIME2;
      std::uint32_t v2 = seed + PRIME2;
      std::uint32_t v3 = seed;
      std::uint32_t v4 = seed - PRIME1;

      const auto * limit = end - 16;
      do {
        v1 = round(v1, readU32(p));
        p += 4;
        v2 = round(v2, readU32(p));
        p += 4;
        v3 = round(v3, readU32(p));
        p += 4;
        v4 = round(v4, readU32(p));
        p += 4;
      } while (p <= limit);

      h32 = rotl(v1, 1) + rotl(v2, 7)
        + rotl(v3, 12) + rotl(v4, 18);
    } else {
      h32 = seed + PRIME5;
    }

    h32 += static_cast< std::uint32_t >(len);

    while (p + 4 <= end) {
      h32 += readU32(p) * PRIME3;
      h32 = rotl(h32, 17) * PRIME4;
      p += 4;
    }

    while (p < end) {
      h32 += (*p) * PRIME5;
      h32 = rotl(h32, 11) * PRIME1;
      ++p;
    }

    h32 ^= h32 >> 15;
    h32 *= PRIME2;
    h32 ^= h32 >> 13;
    h32 *= PRIME3;
    h32 ^= h32 >> 16;

    return static_cast< std::size_t >(h32);
  }

private:
  static std::uint32_t round(std::uint32_t acc, std::uint32_t input)
  {
    acc += input * PRIME2;
    acc = rotl(acc, 13);
    acc *= PRIME1;
    return acc;
  }

  static std::uint32_t rotl(std::uint32_t x, int r)
  {
    return (x << r) | (x >> (32 - r));
  }

  static std::uint32_t readU32(const std::uint8_t * p)
  {
    std::uint32_t v = 0;
    std::memcpy(&v, p, 4);
    return v;
  }
};

struct StringXxHash {
  std::size_t operator()(const std::string & s) const
  {
    XxHash32 h;
    return h(s.data(), s.size());
  }

  std::size_t hashBytes(const unsigned char * data, std::size_t len) const
  {
    XxHash32 h;
    return h(data, len);
  }
};

struct PairStringXxHash {
  std::size_t operator()(const EdgeKey & p) const
  {
    XxHash32 h;
    const std::size_t h1 = h(p.first.data(), p.first.size());
    const std::size_t h2 = h(p.second.data(), p.second.size());
    return h1 ^ (h2 + 0x9e3779b9u + (h1 << 6) + (h1 >> 2));
  }
};

struct PairStringEqual {
  bool operator()(const EdgeKey & a, const EdgeKey & b) const
  {
    return a.first == b.first && a.second == b.second;
  }
};

}

#endif
