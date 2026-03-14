#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"
#include <string>

using namespace shevchenko;

BOOST_AUTO_TEST_CASE(test_constructor_and_empty)
{
  List< int > lst;
  BOOST_TEST(lst.empty());
  BOOST_TEST(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  List< int > lst;
  lst.pushFront(1);
  lst.pushFront(2);
  lst.pushFront(3);

  BOOST_TEST(lst.size() == 3);
  BOOST_TEST(lst.front() == 3);
  BOOST_TEST(lst.back() == 1);
}

BOOST_AUTO_TEST_CASE(test_push_back)
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);

  BOOST_TEST(lst.size() == 3);
  BOOST_TEST(lst.front() == 1);
  BOOST_TEST(lst.back() == 3);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);

  lst.popFront();
  BOOST_TEST(lst.size() == 2);
  BOOST_TEST(lst.front() == 2);

  lst.popFront();
  lst.popFront();
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(test_pop_back)
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);

  lst.popBack();
  BOOST_TEST(lst.size() == 2);
  BOOST_TEST(lst.back() == 2);

  lst.popBack();
  lst.popBack();
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  List< int > original;
  original.pushBack(1);
  original.pushBack(2);

  List< int > copy(original);

  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(copy.front() == 1);
  BOOST_TEST(copy.back() == 2);

  original.pushBack(3);
  BOOST_TEST(original.size() == 3);
  BOOST_TEST(copy.size() == 2);
}

BOOST_AUTO_TEST_CASE(test_iterator)
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);

  int expected[] = {1, 2, 3};
  int i = 0;
  for (auto it = lst.cbegin(); it != lst.cend(); ++it)
  {
    BOOST_TEST(*it == expected[i]);
    ++i;
  }
  BOOST_TEST(i == 3);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.clear();

  BOOST_TEST(lst.empty());
  BOOST_TEST(lst.size() == 0);
}
