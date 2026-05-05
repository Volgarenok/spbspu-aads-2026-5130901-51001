#define BOOST_TEST_MODULE HashTests
#include <boost/test/unit_test.hpp>
#include "hash.hpp"
#include <utility>


using namespace kitserov;
BOOST_AUTO_TEST_CASE(hash_start_empty)
{
  HashTable< int, int, siphash_64, std::equal_to< int > > r;
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
}


