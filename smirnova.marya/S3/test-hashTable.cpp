#define BOOST_TEST_MODULE S3
#include <boost/test/included/unit_test.hpp>

#include "hashTable.hpp"
#include <string>

using smirnova::HashTable;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(DefaultConstruct)
{
  HashTable<std::string, int> ht;
  BOOST_TEST(ht.bucketCount() == 16);
}

BOOST_AUTO_TEST_CASE(AddAndHas)
{
  HashTable<std::string, int> ht;
  BOOST_CHECK(!ht.has("key1"));

  ht.add("key1", 42);
  BOOST_CHECK(ht.has("key1"));
}

BOOST_AUTO_TEST_CASE(AddAndGet)
{
  HashTable<std::string, int> ht;
  ht.add("key1", 100);

  BOOST_CHECK_EQUAL(ht.get("key1"), 100);
}

BOOST_AUTO_TEST_CASE(AddMultiple)
{
  HashTable<std::string, int> ht;
  ht.add("a", 1);
  ht.add("b", 2);
  ht.add("c", 3);

  BOOST_CHECK_EQUAL(ht.get("a"), 1);
  BOOST_CHECK_EQUAL(ht.get("b"), 2);
  BOOST_CHECK_EQUAL(ht.get("c"), 3);
}

BOOST_AUTO_TEST_CASE(UpdateValue)
{
  HashTable<std::string, int> ht;
  ht.add("key", 10);
  BOOST_CHECK_EQUAL(ht.get("key"), 10);

  ht.add("key", 20);
  BOOST_CHECK_EQUAL(ht.get("key"), 20);
}

BOOST_AUTO_TEST_CASE(Drop)
{
  HashTable<std::string, int> ht;
  ht.add("key1", 50);

  int dropped = ht.drop("key1");
  BOOST_CHECK_EQUAL(dropped, 50);
  BOOST_CHECK(!ht.has("key1"));
}

BOOST_AUTO_TEST_CASE(DropThrowsOnNotFound)
{
  HashTable<std::string, int> ht;

  BOOST_CHECK_THROW(ht.drop("nonexistent"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GetThrowsOnNotFound)
{
  HashTable<std::string, int> ht;

  BOOST_CHECK_THROW(ht.get("nonexistent"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(BucketCount)
{
  HashTable<std::string, int> ht;
  BOOST_CHECK_EQUAL(ht.bucketCount(), 16);
}

BOOST_AUTO_TEST_CASE(Rehash)
{
  HashTable<std::string, int> ht;
  ht.add("key1", 1);
  ht.add("key2", 2);

  ht.rehash(32);

  BOOST_CHECK_EQUAL(ht.bucketCount(), 32);
  BOOST_CHECK_EQUAL(ht.get("key1"), 1);
  BOOST_CHECK_EQUAL(ht.get("key2"), 2);
}

BOOST_AUTO_TEST_CASE(HasReturnsFalseForMissing)
{
  HashTable<std::string, int> ht;
  ht.add("exists", 1);

  BOOST_CHECK(ht.has("exists"));
  BOOST_CHECK(!ht.has("missing"));
}

BOOST_AUTO_TEST_SUITE_END()


