#define BOOST_TEST_MODULE Listtests
#include <boost/test/included/unit_test.hpp>

#include "list.hpp"

BOOST_AUTO_TEST_CASE(emptyList)
{
  yarmolinskaya::List< int > list;
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(pushBackTest)
{
  yarmolinskaya::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(pushFrontTest)
{
  yarmolinskaya::List< int > list;
  list.push_front(3);
  list.push_front(2);
  list.push_front(1);
  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(popFrontTest)
{
  yarmolinskaya::List< int > list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  list.pop_front();
  BOOST_TEST(list.front() == 20);
}

BOOST_AUTO_TEST_CASE(popBackTest)
{
  yarmolinskaya::List< int > list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  list.pop_back();
  BOOST_TEST(list.back() == 20);
}

BOOST_AUTO_TEST_CASE(clearTest)
{
  yarmolinskaya::List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.clear();
  BOOST_TEST(list.empty());
}
