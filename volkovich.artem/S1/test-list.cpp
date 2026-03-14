#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <string>
#include <utility>
#include "list.hpp"

using namespace volkovich;

BOOST_AUTO_TEST_CASE(test_empty_list)
{
  List<int> a;
  BOOST_TEST(a.isEmpty());
}

BOOST_AUTO_TEST_CASE(test_push_front)
{
  List<int> a;
  a.pushFront(1);
  a.pushFront(2);
  a.pushFront(3);

  BOOST_TEST(!a.isEmpty());
  BOOST_TEST(a.size() == 3u);

  int expected[] = {3, 2, 1};
  size_t i = 0;
  for (LIter<int> it = a.begin(); it != LIter<int>(nullptr); ++it, ++i)
  {
    BOOST_TEST(*it == expected[i]);
  }
  BOOST_TEST(i == 3u);
}

BOOST_AUTO_TEST_CASE(test_push_back)
{
  List<int> a;
  a.pushBack(1);
  a.pushBack(2);
  a.pushBack(3);

  BOOST_TEST(!a.isEmpty());
  BOOST_TEST(a.size() == 3u);

  int expected[] = {1, 2, 3};
  size_t i = 0;
  for (LIter<int> it = a.begin(); it != LIter<int>(nullptr); ++it, ++i)
  {
    BOOST_TEST(*it == expected[i]);
  }
  BOOST_TEST(i == 3u);
}

BOOST_AUTO_TEST_CASE(test_insert_after)
{
  List<int> a;
  a.pushBack(1);
  a.pushBack(3);

  LIter<int> it = a.begin();
  a.insertAfter(it, 2);

  int expected[] = {1, 2, 3};
  size_t i = 0;
  for (LIter<int> iter = a.begin(); iter != LIter<int>(nullptr); ++iter, ++i)
  {
    BOOST_TEST(*iter == expected[i]);
  }
  BOOST_TEST(i == 3u);
}

BOOST_AUTO_TEST_CASE(test_fake_begin)
{
  List<int> a;
  LIter<int> it = a.fakeBegin();
  it = a.insertAfter(it, 1);
  it = a.insertAfter(it, 2);

  BOOST_TEST(!a.isEmpty());
  BOOST_TEST(a.size() == 2u);

  int expected[] = {1, 2};
  size_t i = 0;
  for (LIter<int> iter = a.begin(); iter != LIter<int>(nullptr); ++iter, ++i)
  {
    BOOST_TEST(*iter == expected[i]);
  }
  BOOST_TEST(i == 2u);
}

BOOST_AUTO_TEST_CASE(test_begin)
{
  List<int> a;
  BOOST_TEST(a.begin().item == nullptr);

  a.pushBack(42);
  BOOST_TEST(a.begin().item != nullptr);
  BOOST_TEST(*a.begin() == 42);
}

BOOST_AUTO_TEST_CASE(test_pop_back)
{
  List<int> a;
  a.pushBack(1);
  a.pushBack(2);
  a.pushBack(3);

  a.popBack();
  BOOST_TEST(a.size() == 2u);

  int expected1[] = {1, 2};
  size_t i = 0;
  for (LIter<int> it = a.begin(); it != LIter<int>(nullptr); ++it, ++i)
  {
    BOOST_TEST(*it == expected1[i]);
  }
  BOOST_TEST(i == 2u);

  a.popBack();
  a.popBack();
  BOOST_TEST(a.isEmpty());
  BOOST_TEST(a.size() == 0u);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  List<int> a;
  a.pushBack(1);
  a.pushBack(2);
  a.pushBack(3);

  a.popFront();
  BOOST_TEST(a.size() == 2u);

  int expected1[] = {2, 3};
  size_t i = 0;
  for (LIter<int> it = a.begin(); it != LIter<int>(nullptr); ++it, ++i)
  {
    BOOST_TEST(*it == expected1[i]);
  }
  BOOST_TEST(i == 2u);

  a.popFront();
  a.popFront();
  BOOST_TEST(a.isEmpty());
  BOOST_TEST(a.size() == 0u);
}
