#define BOOST_TEST_MODULE S3Tests
#include <boost/test/included/unit_test.hpp>

#include "sip_hash.hpp"

BOOST_AUTO_TEST_SUITE(sip_hash_tests)

BOOST_AUTO_TEST_CASE(same_string_same_hash)
{
  uint64_t h1 = losev::hashString("hello");
  uint64_t h2 = losev::hashString("hello");
  BOOST_CHECK_EQUAL(h1, h2);
}

BOOST_AUTO_TEST_CASE(different_strings_different_hashes)
{
  uint64_t h1 = losev::hashString("hello");
  uint64_t h2 = losev::hashString("world");
  BOOST_CHECK_NE(h1, h2);
}

BOOST_AUTO_TEST_SUITE_END()
