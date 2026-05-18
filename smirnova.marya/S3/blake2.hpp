#ifndef BLAKE2HASH_HPP
#define BLAKE2HASH_HPP

#include <cstddef>
#include <boost/container_hash/hash.hpp>

namespace smirnova
{
  template < class T >
  struct Blake2Hasher
  {
    size_t operator()(const T& value) const
    {
      return boost::hash< T >{}(value);
    }
  };

  template < class T >
  struct PairHasher
  {
    size_t operator()(const std::pair< T, T >& p) const
    {
      size_t h = boost::hash< T >{}(p.first);
      boost::hash_combine(h, p.second);
      return h;
    }
  };

}

#endif


