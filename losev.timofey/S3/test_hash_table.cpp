#define BOOST_TEST_MODULE S3Tests
#include <boost/test/included/unit_test.hpp>

#include "sip_hash.hpp"
#include "hash_table.hpp"

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

BOOST_AUTO_TEST_SUITE(hash_table_tests)

BOOST_AUTO_TEST_CASE(add_and_has)
{
  losev::HashTable<std::string, int> table(4);
  table.add("key", 42);
  BOOST_CHECK(table.has("key"));
}

BOOST_AUTO_TEST_CASE(get_returns_value)
{
  losev::HashTable<std::string, int> table(4);
  table.add("key", 42);
  BOOST_CHECK_EQUAL(table.get("key"), 42);
}

BOOST_AUTO_TEST_CASE(drop_removes_element)
{
  losev::HashTable<std::string, int> table(4);
  table.add("key", 100);
  int v = table.drop("key");
  BOOST_CHECK_EQUAL(v, 100);
  BOOST_CHECK(!table.has("key"));
}

BOOST_AUTO_TEST_CASE(drop_nonexistent_throws)
{
  losev::HashTable<std::string, int> table(4);
  BOOST_CHECK_THROW(table.drop("missing"), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()
