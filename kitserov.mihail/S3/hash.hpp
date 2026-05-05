#include <vector>
#include "common/list.hpp"
namespace kitserov
{
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable
  {
    
  private:
    size_t size_;
    size_t capacity_;
    std::vector< Value > slots;
    
    size_t probe(const Key& k, size_t i);
  };
}

template< class Key, class Value, class Hash, class Equal >
size_t kitserov::HashTable< Key, Value, Hash, Equal >::probe(const Key& k, size_t i)
{
  return Hash(k) + i * i;
}