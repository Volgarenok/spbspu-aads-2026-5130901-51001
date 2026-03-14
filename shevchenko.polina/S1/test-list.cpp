#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"
#include <string>

using namespace shevchenko;

BOOST_AUTO_TEST_CASE(test_constructors)
{
  List< int > lst1;
  BOOST_TEST(lst1.empty());
  BOOST_TEST(lst1.size() == 0);

  lst1.pushBack(1);
  lst1.pushBack(2);
  lst1.pushBack(3);

  List< int > lst2(lst1);
  BOOST_TEST(lst2.size() == 3);
  BOOST_TEST(lst2.front() == 1);
  BOOST_TEST(lst2.back() == 3);

  List< int > lst3(std::move(lst2));
  BOOST_TEST(lst2.empty());
  BOOST_TEST(lst3.size() == 3);
}

BOOST_AUTO_TEST_CASE(test_push)
{
  List< int > lst;

  lst.pushFront(1);
  BOOST_TEST(lst.front() == 1);
  BOOST_TEST(lst.back() == 1);

  lst.pushFront(2);
  BOOST_TEST(lst.front() == 2);
  BOOST_TEST(lst.back() == 1);

  lst.pushBack(3);
  BOOST_TEST(lst.back() == 3);

  auto it = lst.cbegin();
  BOOST_TEST(*it++ == 2);
  BOOST_TEST(*it++ == 1);
  BOOST_TEST(*it++ == 3);
  BOOST_TEST(it == lst.cend());
}

BOOST_AUTO_TEST_CASE(test_insert)
{
  List< int > lst;
  lst.pushBack(1);
  lst.pushBack(3);

  auto it = lst.begin();
  lst.insertAfter(it, 2);

  it = lst.end();
  lst.insertBefore(it, 4);

  int expected[] = {1, 2, 3, 4};
  int i = 0;
  for (auto cit = lst.cbegin(); cit != lst.cend(); ++cit, ++i)
  {
    BOOST_TEST(*cit == expected[i]);
  }
  BOOST_TEST(i == 4);
}

BOOST_AUTO_TEST_CASE(test_remove)
{
  List< int > lst;
  for (int i = 1; i <= 5; ++i) lst.pushBack(i);

  lst.popFront();
  BOOST_TEST(lst.front() == 2);

  lst.popBack();
  BOOST_TEST(lst.back() == 4);

  auto it = lst.begin();
  ++it;
  lst.erase(it);

  int expected[] = {2, 4};
  int i = 0;
  for (auto cit = lst.cbegin(); cit != lst.cend(); ++cit, ++i)
  {
    BOOST_TEST(*cit == expected[i]);
  }

  lst.clear();
  BOOST_TEST(lst.empty());
}

BOOST_AUTO_TEST_CASE(test_iterators)
{
  List< int > lst;
  for (int i = 1; i <= 3; ++i) lst.pushBack(i);

  int forward[] = {1, 2, 3};
  int i = 0;
  for (auto it = lst.begin(); it != lst.end(); ++it, ++i)
  {
    BOOST_TEST(*it == forward[i]);
  }

  int backward[] = {3, 2, 1};
  i = 0;
  auto it = lst.end();
  while (it != lst.begin())
  {
    --it;
    BOOST_TEST(*it == backward[i]);
    ++i;
  }

  const List< int >& clst = lst;
  i = 0;
  for (auto cit = clst.cbegin(); cit != clst.cend(); ++cit, ++i)
  {
    BOOST_TEST(*cit == forward[i]);
  }
}

BOOST_AUTO_TEST_CASE(test_assignments)
{
  List< int > a;
  a.pushBack(1);
  a.pushBack(2);

  List< int > b;
  b = a;
  BOOST_TEST(b.size() == 2);
  BOOST_TEST(b.front() == 1);

  List< int > c;
  c = std::move(b);
  BOOST_TEST(b.empty());
  BOOST_TEST(c.size() == 2);

  List< int > d;
  d.pushBack(3);
  d.pushBack(4);
  c.swap(d);
  BOOST_TEST(c.front() == 3);
  BOOST_TEST(d.front() == 1);
}

BOOST_AUTO_TEST_CASE(test_types)
{
  List< std::string > strList;
  strList.pushBack("hello");
  strList.pushBack("world");
  BOOST_TEST(strList.front() == "hello");
  BOOST_TEST(strList.back() == "world");

  List< std::pair< int, std::string > > pairList;
  pairList.pushBack({1, "one"});
  pairList.pushBack({2, "two"});
  auto it = pairList.begin();
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == "one");
  ++it;
  BOOST_TEST(it->first == 2);
  BOOST_TEST(it->second == "two");
}
