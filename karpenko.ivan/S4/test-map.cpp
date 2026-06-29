#include <boost/test/unit_test.hpp>
#include "../common/map.hpp"
#include <string>
#include <stdexcept>

BOOST_AUTO_TEST_SUITE(MapTests)

BOOST_AUTO_TEST_CASE(MapInsertAndAccess)
{
  karpenko::Map< int, std::string > map;
  map.insert(2, "two");
  map.insert(1, "one");
  map.insert(3, "three");

  BOOST_CHECK_EQUAL(map.size(), 3);
  BOOST_CHECK_EQUAL(map.at(1), "one");
  BOOST_CHECK_EQUAL(map.at(2), "two");
  BOOST_CHECK_EQUAL(map.at(3), "three");
  BOOST_CHECK_THROW(map.at(4), std::out_of_range);

  map[4] = "four";
  BOOST_CHECK_EQUAL(map.size(), 4);
  BOOST_CHECK_EQUAL(map[4], "four");

  auto it = map.begin();
  BOOST_CHECK_EQUAL(it->first, 1);
  BOOST_CHECK_EQUAL(it->second, "one");
  ++it;
  BOOST_CHECK_EQUAL(it->first, 2);
  BOOST_CHECK_EQUAL(it->second, "two");
  ++it;
  BOOST_CHECK_EQUAL(it->first, 3);
  BOOST_CHECK_EQUAL(it->second, "three");
  ++it;
  BOOST_CHECK_EQUAL(it->first, 4);
  BOOST_CHECK_EQUAL(it->second, "four");
  ++it;
  BOOST_CHECK(it == map.end());

  bool erased = map.erase(2);
  BOOST_CHECK(erased);
  BOOST_CHECK_EQUAL(map.size(), 3);
  BOOST_CHECK(!map.contains(2));
  erased = map.erase(10);
  BOOST_CHECK(!erased);
}

BOOST_AUTO_TEST_CASE(MapConstIterator)
{
  karpenko::Map< int, std::string > map;
  map.insert(1, "one");
  map.insert(2, "two");
  const karpenko::Map< int, std::string >& constMap = map;

  auto cit = constMap.begin();
  BOOST_CHECK_EQUAL(cit->first, 1);
  BOOST_CHECK_EQUAL(cit->second, "one");
  ++cit;
  BOOST_CHECK_EQUAL(cit->first, 2);
  BOOST_CHECK_EQUAL(cit->second, "two");
  ++cit;
  BOOST_CHECK(cit == constMap.end());
}

BOOST_AUTO_TEST_CASE(MapHeight)
{
  karpenko::Map< int, int > map;
  for (int i = 1; i <= 50; ++i)
  {
    map.insert(i, i * i);
  }
  BOOST_CHECK_LE(map.height(), 20);
}

BOOST_AUTO_TEST_SUITE_END()
