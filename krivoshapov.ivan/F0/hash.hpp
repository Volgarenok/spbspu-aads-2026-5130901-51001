#ifndef KRIVOSHAPOV_HASH_HPP
#define KRIVOSHAPOV_HASH_HPP

#include <cstdint>
#include <string>

namespace krivoshapov
{
  std::uint32_t hashShingle(const std::string &text);
}

#endif
