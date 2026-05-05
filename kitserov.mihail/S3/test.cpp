#define BOOST_TEST_MODULE HashTests
#include <boost/test/unit_test.hpp>
#include "hash.hpp"
#include <utility>


using namespace kitserov;
using hs = HashTable< int, int, siphash_64, std::equal_to< int > >
BOOST_AUTO_TEST_CASE(hash_start_empty)
{
  hs r;
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
}
BOOST_AUTO_TEST_CASE(check_initial_size_and_cap)
{
  hs r(5);
  BOOST_CHECK(!r.isEmpty() && !yar.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
  BOOST_CHECK_EQUAL(r.capacity(), 5);
}
