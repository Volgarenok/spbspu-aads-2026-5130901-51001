#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "../common/list.hpp"

BOOST_AUTO_TEST_SUITE(list_tests)

BOOST_AUTO_TEST_CASE(empty_list_test)
{
  smirnova::List< size_t > list;
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(push_front_test)
{
  smirnova::List< size_t > list;
  list.pushFront(42);
  BOOST_TEST(list.empty() == false);
  BOOST_TEST(list.begin().value() == 42);
}

BOOST_AUTO_TEST_CASE(push_back_test)
{
  smirnova::List< size_t > list;
  list.pushBack(7);
  BOOST_TEST(list.begin().value() == 7);
  BOOST_TEST(list.end().valid() == false);
}

BOOST_AUTO_TEST_CASE(front_back_test)
{
  smirnova::List< size_t > list;

  list.pushBack(10);
  list.pushBack(20);
  list.pushBack(30);

  BOOST_TEST(list.front() == 10);
  BOOST_TEST(list.back() == 30);
}

BOOST_AUTO_TEST_CASE(pop_front_test)
{
  smirnova::List< size_t > list;
  list.pushFront(1);
  list.popFront();
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(pop_back_test)
{
  smirnova::List< size_t > list;
  list.pushBack(2);
  list.popBack();
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(clear_test)
{
  smirnova::List< size_t > list;
  list.pushBack(1);
  list.pushBack(2);
  list.clear();
  BOOST_TEST(list.empty() == true);
}

BOOST_AUTO_TEST_CASE(iterator_test)
{
  smirnova::List< size_t > list;
  list.pushBack(5);
  list.pushBack(10);
  auto it = list.begin();
  BOOST_TEST(it.value() == 5);
  it.next();
  BOOST_TEST(it.value() == 10);
  it.next();
  BOOST_TEST(it.valid() == false);
}

BOOST_AUTO_TEST_CASE(iterator_end_value_throws)
{
  smirnova::List< size_t > list;

  smirnova::LIter< size_t > it = list.end();
  BOOST_CHECK_THROW(it.value(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(front_back_on_empty_throws)
{
  smirnova::List< size_t > list;

  BOOST_CHECK_THROW(list.front(), std::out_of_range);
  BOOST_CHECK_THROW(list.back(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(pop_front_on_empty_throws)
{
  smirnova::List< size_t > list;
  BOOST_CHECK_THROW(list.popFront(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(pop_back_on_empty_throws)
{
  smirnova::List< size_t > list;
  BOOST_CHECK_THROW(list.popBack(), std::out_of_range);
}

BOOST_AUTO_TEST_SUITE_END()

