#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>

#include "list.hpp"

using namespace hachaturyanov;

BOOST_AUTO_TEST_CASE(test_empty_list)
{
  List< int > list;
  BOOST_TEST(list.isEmpty());
}

BOOST_AUTO_TEST_CASE(test_add_front)
{
  List< int > list;
  list.addFront(1);
  BOOST_CHECK(!list.isEmpty());
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(*list.begin() == 1);

  list.addFront(2);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(*list.begin() == 2);
}

BOOST_AUTO_TEST_CASE(test_add_end)
{
  List< int > list;
  list.addEnd(1);
  BOOST_CHECK(!list.isEmpty());
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(*list.begin() == 1);

  list.addEnd(2);
  BOOST_TEST(list.size() == 2);
  BOOST_TEST(*list.begin() == 1);
  auto it = list.begin();
  --it;
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_CASE(test_insert_before)
{
  List< int > list;
  auto it = list.addEnd(1);
  list.insertBefore(it, 2);
  --it;
  BOOST_TEST(*it == 2);
  BOOST_CHECK(list.size() == 2);
  BOOST_TEST(*list.begin() == 2);
  BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(test_insert_after)
{
  List< int > list;
  auto it = list.addFront(1);
  list.insertAfter(it, 2);
  BOOST_CHECK(list.size() == 2);
  BOOST_TEST(*list.begin() == 1);
  BOOST_TEST(*list.end() == 2);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  List< int > list;
  list.addEnd(1);
  list.addEnd(2);
  list.popFront();
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(*list.begin() == 2);
}

