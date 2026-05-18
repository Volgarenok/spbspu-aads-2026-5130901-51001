#include <boost/test/unit_test.hpp>

#include "hashTable.hpp"
#include <string>

using smirnova::HashTable;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(DefaultConstruct)
{
  HashTable<std::string, int> ht;

  BOOST_CHECK_EQUAL(ht.bucketCount(), 16);
  BOOST_CHECK_EQUAL(ht.size(), 0);
}

BOOST_AUTO_TEST_CASE(AddAndGet)
{
  HashTable<std::string, int> ht;

  BOOST_CHECK(!ht.has("key1"));

  ht.add("key1", 42);

  BOOST_CHECK(ht.has("key1"));
  BOOST_CHECK_EQUAL(ht.get("key1"), 42);
  BOOST_CHECK_EQUAL(ht.size(), 1);
}

BOOST_AUTO_TEST_CASE(UpdateValue)
{
  HashTable<std::string, int> ht;

  ht.add("key1", 10);
  BOOST_CHECK_EQUAL(ht.get("key1"), 10);

  ht.add("key1", 20);

  BOOST_CHECK_EQUAL(ht.get("key1"), 20);
  BOOST_CHECK_EQUAL(ht.size(), 1);
}

BOOST_AUTO_TEST_CASE(AddMultiple)
{
  HashTable<std::string, int> ht;

  ht.add("key1", 1);
  ht.add("key2", 2);
  ht.add("key3", 3);

  BOOST_CHECK_EQUAL(ht.get("key1"), 1);
  BOOST_CHECK_EQUAL(ht.get("key2"), 2);
  BOOST_CHECK_EQUAL(ht.get("key3"), 3);

  BOOST_CHECK_EQUAL(ht.size(), 3);
}

BOOST_AUTO_TEST_CASE(Drop)
{
  HashTable<std::string, int> ht;

  ht.add("key1", 50);
  ht.add("key2", 60);
  ht.add("key3", 70);

  BOOST_CHECK_EQUAL(ht.drop("key1"), 50);

  BOOST_CHECK(!ht.has("key1"));
  BOOST_CHECK(ht.has("key2"));
  BOOST_CHECK(ht.has("key3"));

  BOOST_CHECK_EQUAL(ht.size(), 2);
}

BOOST_AUTO_TEST_CASE(HasReturnsFalseForMissing)
{
  HashTable<std::string, int> ht;

  ht.add("key1", 1);
  ht.add("key2", 2);
  ht.add("key3", 3);

  BOOST_CHECK(ht.has("key1"));
  BOOST_CHECK(ht.has("key2"));
  BOOST_CHECK(ht.has("key3"));

  BOOST_CHECK(!ht.has("missing"));
}

BOOST_AUTO_TEST_CASE(Rehash)
{
  HashTable<std::string, int> ht;

  ht.add("key1", 1);
  ht.add("key2", 2);
  ht.add("key3", 3);

  ht.rehash(32);

  BOOST_CHECK_EQUAL(ht.bucketCount(), 32);

  BOOST_CHECK(ht.has("key1"));
  BOOST_CHECK(ht.has("key2"));
  BOOST_CHECK(ht.has("key3"));

  BOOST_CHECK_EQUAL(ht.get("key1"), 1);
  BOOST_CHECK_EQUAL(ht.get("key2"), 2);
  BOOST_CHECK_EQUAL(ht.get("key3"), 3);

  BOOST_CHECK_EQUAL(ht.size(), 3);
}

BOOST_AUTO_TEST_SUITE_END()

