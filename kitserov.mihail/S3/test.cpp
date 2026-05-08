#define BOOST_TEST_MODULE HashTests
#include <boost/test/included/unit_test.hpp>
#include <boost/hash2/hash.hpp>
#include <boost/hash2/siphash.hpp>
#include "hash.hpp"
#include <utility>


using namespace kitserov;
using MyHash = boost::hash2::hash< int, boost::hash2::siphash_64 >;
BOOST_AUTO_TEST_CASE(hash_start_empty)
{
  HashTable< int, int, MyHash, std::equal_to< int > > r;
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
}
BOOST_AUTO_TEST_CASE(check_initial_size_and_cap)
{
  HashTable< int, int, MyHash, std::equal_to< int > > r(5);
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
  BOOST_CHECK_EQUAL(r.capacity(), 5);
}
BOOST_AUTO_TEST_CASE(copy)
{
  HashTable< int, int, MyHash, std::equal_to< int > > r(5);
  HashTable< int, int, MyHash, std::equal_to< int > > yar(3);
  r.copy(yar);
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
  BOOST_CHECK_EQUAL(r.capacity(), 3);
}
BOOST_AUTO_TEST_CASE(swap)
{
  HashTable< int, int, MyHash, std::equal_to< int > > r(5);
  HashTable< int, int, MyHash, std::equal_to< int > > yar(3);
  r.swap(yar);
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
  BOOST_CHECK_EQUAL(r.capacity(), 3);
  BOOST_CHECK_EQUAL(yar.capacity(), 5);
}
BOOST_AUTO_TEST_CASE(find)
{
  HashTable< int, int, MyHash, std::equal_to< int > > r(5);
  BOOST_CHECK_EQUAL(r.find(4), nullptr);
}
BOOST_AUTO_TEST_CASE(add)
{
  HashTable< int, int, MyHash, std::equal_to< int > > r(5);
  r.add(4, 16);
  BOOST_CHECK_EQUAL(r.size(), 1);
  BOOST_CHECK_EQUAL(r.find(4), 16);
}
BOOST_AUTO_TEST_CASE(copy_equal)
{
  HashTable< int, int, MyHash, std::equal_to< int > > r(5);
  r.add(4, 16);
  HashTable< int, int, MyHash, std::equal_to< int > > yar = r;
  BOOST_CHECK_EQUAL(yar.size(), 1);
  BOOST_CHECK_EQUAL(yar.capacity(), 5);
}
BOOST_AUTO_TEST_CASE(equal_with_rvalue)
{
  HashTable< int, int, MyHash, std::equal_to< int > > r(5);
  r.add(4, 16);
  HashTable< int, int, MyHash, std::equal_to< int > > yar = std::move(r);
  BOOST_CHECK_EQUAL(yar.size(), 1);
  BOOST_CHECK_EQUAL(yar.capacity(), 5);
}
BOOST_AUTO_TEST_CASE(load_factor)
{
  HashTable< int, int, MyHash, std::equal_to< int > > r(5);
  r.add(4, 16);
  BOOST_CHECK_EQUAL(r.loadFactor, 0.2);
}