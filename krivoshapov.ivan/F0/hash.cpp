#include "hash.hpp"

#include <cstddef>

std::uint32_t krivoshapov::hashShingle(const std::string &text)
{
  const std::uint32_t offsetBasis = 2166136261u;
  const std::uint32_t prime = 16777619u;
  std::uint32_t hash = offsetBasis;
  for (std::size_t i = 0; i < text.size(); ++i)
  {
    const unsigned char byte = static_cast<unsigned char>(text[i]);
    hash = hash ^ static_cast<std::uint32_t>(byte);
    hash = hash * prime;
  }
  return hash;
}
