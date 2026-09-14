#ifndef XXHASH_HPP
#define XXHASH_HPP

#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/hash_append.hpp>
#include <cstdint>

namespace hachaturyanov {

  template< class Key >
  struct xxhash {
    std::uint64_t operator()(const Key &key) const noexcept
    {
      boost::hash2::xxhash_64 h;
      boost::hash2::hash_append(h, {}, key);
      return h.result();
    }
  };

}

#endif
