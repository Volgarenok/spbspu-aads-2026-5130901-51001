#ifndef CUCKOO_HASH_FUNC_HPP
#define CUCKOO_HASH_FUNC_HPP

#include <cstddef>
#include <functional>

namespace smirnova
{
  template<class Key>
  struct DefaultHasher
  {
    size_t h1(const Key& key) const noexcept
    {
      return std::hash< Key >{}(key);
    }

    size_t h2(const Key& key) const noexcept
    {
      size_t h = std::hash< Key >{}(key);
      return std::hash< size_t >{}(h);
    }
  };
}

#endif

