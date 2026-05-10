#define BOOST_TEST_MODULE HashTests
#include <boost/test/included/unit_test.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
#include "hash.hpp"
#include <utility>

template<typename T>
struct SipHash {
    size_t operator()(const T& key) const {
        boost::hash2::siphash_64 hash;  
        boost::hash2::hash_append(hash, {}, key);
        return hash.result();
    }
};

using namespace kitserov;
BOOST_AUTO_TEST_CASE(hash_start_empty)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r;
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
}
BOOST_AUTO_TEST_CASE(check_initial_size_and_cap)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
  BOOST_CHECK_EQUAL(r.capacity(), 5);
}
BOOST_AUTO_TEST_CASE(copy)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  HashTable< int, int, SipHash< int >, std::equal_to< int > > yar(3);
  r.copy(yar);
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
  BOOST_CHECK_EQUAL(r.capacity(), 3);
}
BOOST_AUTO_TEST_CASE(swap)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  HashTable< int, int, SipHash< int >, std::equal_to< int > > yar(3);
  r.swap(yar);
  BOOST_CHECK(r.isEmpty());
  BOOST_CHECK_EQUAL(r.size(), 0);
  BOOST_CHECK_EQUAL(r.capacity(), 3);
  BOOST_CHECK_EQUAL(yar.capacity(), 5);
}
BOOST_AUTO_TEST_CASE(find)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  BOOST_CHECK_EQUAL(r.find(4), nullptr);
  BOOST_CHECK(!r.contains(4));
}
BOOST_AUTO_TEST_CASE(add)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  r.add(4, 16);
  BOOST_CHECK_EQUAL(r.size(), 1);
  BOOST_CHECK_EQUAL(*(r.find(4)), 16);
  BOOST_CHECK(r.contains(4));
}
BOOST_AUTO_TEST_CASE(copy_equal)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  r.add(4, 16);
  HashTable< int, int, SipHash< int >, std::equal_to< int > > yar = r;
  BOOST_CHECK_EQUAL(yar.size(), 1);
  BOOST_CHECK_EQUAL(yar.capacity(), 5);
}
BOOST_AUTO_TEST_CASE(equal_with_rvalue)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  r.add(4, 16);
  HashTable< int, int, SipHash< int >, std::equal_to< int > > yar = std::move(r);
  BOOST_CHECK_EQUAL(yar.size(), 1);
  BOOST_CHECK_EQUAL(yar.capacity(), 5);
}
BOOST_AUTO_TEST_CASE(load_factor)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  r.add(4, 16);
  BOOST_CHECK_EQUAL(r.loadFactor(), (1 / 5));
}
BOOST_AUTO_TEST_CASE(square_staples)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  r.add(4, 16);
  BOOST_CHECK_EQUAL(r[4], 16);
}
BOOST_AUTO_TEST_CASE(read)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  r.add(4, 16);
  r.add(4, 17);
  BOOST_CHECK_EQUAL(r[4], 17);
}
BOOST_AUTO_TEST_CASE(erase)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  r.add(4, 16);
  r.add(5, 25);
  BOOST_CHECK_EQUAL(r.size(), 2);
  r.erase(4);
  BOOST_CHECK_EQUAL(r.find(4), nullptr);
}
BOOST_AUTO_TEST_CASE(rehash)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > r(5);
  r.add(4, 16);
  r.add(5, 25);
  r.add(6, 36);
  r.rehash(10);
  BOOST_CHECK_EQUAL(r.size(), 3);
  BOOST_CHECK_EQUAL(r.capacity(), 10);
  BOOST_CHECK_EQUAL(r.loadFactor(), (3/10));
  BOOST_CHECK(r.find(4));
  BOOST_CHECK(r.find(5));
  BOOST_CHECK(r.find(6));
}