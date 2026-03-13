#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include "list.h"

BOOST_AUTO_TEST_SUITE(list_tests)

BOOST_AUTO_TEST_CASE(empty_list_test)
{
  losev::List<int> list;
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
  losev::List<int> list;
  list.push_front(5);
  BOOST_TEST(list.empty() == false);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  losev::List<int> list;
  list.push_front(5);
  list.pop_front();
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(iterator_begin_end_test)
{
  losev::List<int> list;
  list.push_front(5);
  BOOST_TEST(list.begin() != list.end());
}

BOOST_AUTO_TEST_SUITE_END()
