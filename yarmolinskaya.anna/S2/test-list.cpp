#define BOOST_TEST_MODULE S2

#include <boost/test/included/unit_test.hpp>
#include "common/list.hpp"

using namespace yarmolinskaya;

BOOST_AUTO_TEST_CASE(empty_test)
{
  List< int > list;
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(push_back_test)
{
  List< int > list;

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

BOOST_AUTO_TEST_CASE(push_front_test)
{
  List< int > list;

  list.push_front(3);
  list.push_front(2);
  list.push_front(1);

  BOOST_TEST(list.front() == 1);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  List< int > list;

  list.push_back(1);
  list.push_back(2);

  list.pop_front();

  BOOST_TEST(list.front() == 2);
}
