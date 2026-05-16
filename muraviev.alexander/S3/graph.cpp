#include "graph.hpp"

#include "sip_hash.hpp"

size_t muraviev::StringHash::operator()(const std::string& value) const
{
  const unsigned char* data = reinterpret_cast< const unsigned char* >(value.data());
  return static_cast< size_t >(sipHash24(data, value.size()));
}

bool muraviev::StringEqual::operator()(const std::string& lhs,
    const std::string& rhs) const
{
  return lhs == rhs;
}

size_t muraviev::EdgeKeyHash::operator()(const EdgeKey& value) const
{
  std::string bytes = value.from;
  bytes += '\0';
  bytes += value.to;
  const unsigned char* data = reinterpret_cast< const unsigned char* >(bytes.data());
  return static_cast< size_t >(sipHash24(data, bytes.size()));
}

bool muraviev::EdgeKeyEqual::operator()(const EdgeKey& lhs, const EdgeKey& rhs) const
{
  return lhs.from == rhs.from && lhs.to == rhs.to;
}
