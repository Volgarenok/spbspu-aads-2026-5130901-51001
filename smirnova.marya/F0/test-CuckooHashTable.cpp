#include <boost/test/unit_test.hpp>
#include "CuckooHashTable.hpp"

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(insert_and_get)
{
  smirnova::CuckooHashTable< std::string, int > map;
  map.insert("A", 10);
  int* val = map.get("A");
  BOOST_REQUIRE(val != nullptr);
  BOOST_CHECK_EQUAL(*val, 10);
}

BOOST_AUTO_TEST_CASE(contains_test)
{
  smirnova::CuckooHashTable< std::string, int > map;

  map.insert("A", 1);

  BOOST_CHECK(map.contains("A"));
  BOOST_CHECK(!map.contains("B"));
}

BOOST_AUTO_TEST_CASE(erase_test)
{
  smirnova::CuckooHashTable< std::string, int > map;
  map.insert("A", 1);
  BOOST_CHECK(map.erase("A"));
  BOOST_CHECK(!map.contains("A"));
}

BOOST_AUTO_TEST_CASE(load_factor_basic)
{
  smirnova::CuckooHashTable< std::string, int > map;
  for (int i = 0; i < 10; ++i)
  {
    map.insert("K" + std::to_string(i), i);
  }

  BOOST_CHECK(map.loadFactor() > 0.0);
}

BOOST_AUTO_TEST_CASE(iteration_works)
{
  smirnova::CuckooHashTable< std::string, int > map;
  map.insert("A", 1);
  map.insert("B", 2);
  map.insert("C", 3);
  int count = 0;
  for (auto it = map.begin(); it != map.end(); ++it)
  {
    ++count;
  }
  BOOST_CHECK(count >= 3);
}

BOOST_AUTO_TEST_SUITE_END()

