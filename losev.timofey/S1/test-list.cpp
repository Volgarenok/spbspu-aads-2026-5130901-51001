#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include "list.h"

BOOST_AUTO_TEST_SUITE(list_tests)

BOOST_AUTO_TEST_CASE(empty_list_test)
{
  losev::List< int > list;
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
  losev::List< int > list;
  list.push_front(5);
  BOOST_TEST(list.empty() == false);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  losev::List< int > list;
  list.push_front(5);
  list.pop_front();
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(iterator_begin_end_test)
{
  losev::List< int > list;
  list.push_front(5);
  BOOST_TEST(list.begin() != list.end());
}

BOOST_AUTO_TEST_CASE(iterator_dereference_test)
{
  losev::List< int > list;
  list.push_front(5);
  BOOST_TEST(*(list.begin()) == 5);
}

BOOST_AUTO_TEST_CASE(iterator_increment_test)
{
  losev::List< int > list;
  list.push_front(5);
  list.push_front(10);
  auto it = list.begin();
  BOOST_TEST(*it == 10);
  ++it;
  BOOST_TEST(*it == 5);
}

BOOST_AUTO_TEST_CASE(const_iterator_test)
{
  losev::List< int > list;
  list.push_front(5);
  const losev::List< int >& const_list = list;
  BOOST_TEST(*(const_list.begin()) == 5);
}

BOOST_AUTO_TEST_CASE(copy_test)
{
  losev::List< int > original;
  original.push_front(5);
  losev::List< int > copy(original);
  BOOST_TEST(*(copy.begin()) == 5);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  losev::List< int > list;
  list.push_front(5);
  list.clear();
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_SUITE_END()
