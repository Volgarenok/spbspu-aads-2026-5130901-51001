#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"
#include "sha1.hpp"

using namespace shevchenko;

BOOST_AUTO_TEST_CASE(table_empty_on_start)
{
  HashTable<std::string, int, SHA1Hasher> t;
  BOOST_CHECK(t.empty());
  BOOST_CHECK_EQUAL(t.size(), 0);
}

BOOST_AUTO_TEST_CASE(table_insert_one)
{
  HashTable<std::string, int, SHA1Hasher> t;
  t.insert("key", 5);
  BOOST_CHECK(t.contains("key"));
  BOOST_CHECK_EQUAL(t.at("key"), 5);
}

BOOST_AUTO_TEST_CASE(table_insert_same_key_updates)
{
  HashTable<std::string, int, SHA1Hasher> t;
  t.insert("key", 1);
  t.insert("key", 2);
  BOOST_CHECK_EQUAL(t.at("key"), 2);
  BOOST_CHECK_EQUAL(t.size(), 1);
}

BOOST_AUTO_TEST_CASE(table_erase_removes)
{
  HashTable<std::string, int, SHA1Hasher> t;
  t.insert("a", 1);
  t.insert("b", 2);
  t.erase("a");
  BOOST_CHECK(!t.contains("a"));
  BOOST_CHECK(t.contains("b"));
  BOOST_CHECK_EQUAL(t.size(), 1);
}

BOOST_AUTO_TEST_CASE(table_erase_missing_throws)
{
  HashTable<std::string, int, SHA1Hasher> t;
  t.insert("a", 1);
  BOOST_CHECK_THROW(t.erase("b"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(table_clear_works)
{
  HashTable<std::string, int, SHA1Hasher> t;
  t.insert("a", 1);
  t.insert("b", 2);
  t.clear();
  BOOST_CHECK(t.empty());
  BOOST_CHECK_EQUAL(t.size(), 0);
}

BOOST_AUTO_TEST_CASE(table_rehash_keeps_data)
{
  HashTable<std::string, int, SHA1Hasher> t(2, 2);
  t.insert("a", 1);
  t.insert("b", 2);
  t.rehash(10);
  BOOST_CHECK(t.contains("a"));
  BOOST_CHECK(t.contains("b"));
  BOOST_CHECK_EQUAL(t.bucket_count(), 10);
}

BOOST_AUTO_TEST_CASE(table_iterator_loops)
{
  HashTable<std::string, int, SHA1Hasher> t;
  t.insert("x", 1);
  t.insert("y", 2);
  int sum = 0;
  for (auto it = t.begin(); it != t.end(); ++it) {
    sum += (*it).second;
  }
  BOOST_CHECK_EQUAL(sum, 3);
}
