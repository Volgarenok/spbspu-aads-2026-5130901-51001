#ifndef LOSEV_SIP_HASH_HPP
#define LOSEV_SIP_HASH_HPP

#include <cstdint>
#include <string>
#include <utility>

namespace losev {

inline uint64_t hashString(const std::string&)
{
  return 0;
}

inline uint64_t hashPair(const std::pair<std::string, std::string>&)
{
  return 0;
}

}

#endif
