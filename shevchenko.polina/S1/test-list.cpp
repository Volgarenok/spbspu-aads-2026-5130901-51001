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
