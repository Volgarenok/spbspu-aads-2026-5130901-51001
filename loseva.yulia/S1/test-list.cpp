#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(list_tests)

BOOST_AUTO_TEST_CASE(empty_list_test)
{
  loseva::List<size_t> list;
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
  loseva::List<size_t> list;
  list.push_front(5);
  BOOST_TEST(list.empty() == false);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  loseva::List<size_t> list;
  list.push_front(5);
  list.pop_front();
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_SUITE_END()
