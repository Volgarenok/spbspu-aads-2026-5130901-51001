#include <stdexcept>
#include <boost/test/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(empty_list)
{
  loseva::List< int > lst;
  BOOST_CHECK(lst.empty());
  BOOST_CHECK_EQUAL(lst.size(), 0);
}

BOOST_AUTO_TEST_CASE(push_back_single)
{
  loseva::List< int > lst;
  lst.push_back(42);

  BOOST_CHECK(!lst.empty());
  BOOST_CHECK_EQUAL(lst.size(), 1);

  loseva::List< int >::iterator it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 42);
}

BOOST_AUTO_TEST_CASE(push_back_multiple)
{
  loseva::List< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);

  loseva::List< int >::iterator it = lst.begin();

  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(push_front)
{
  loseva::List< int > lst;
  lst.push_front(1);
  lst.push_front(2);

  loseva::List< int >::iterator it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
  loseva::List< int > lst;
  lst.push_back(1);
  lst.push_back(2);

  lst.pop_front();

  loseva::List< int >::iterator it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 2);
  BOOST_CHECK_EQUAL(lst.size(), 1);
}

BOOST_AUTO_TEST_CASE(pop_back)
{
  loseva::List< int > lst;
  lst.push_back(1);
  lst.push_back(2);

  lst.pop_back();

  loseva::List< int >::iterator it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  BOOST_CHECK_EQUAL(lst.size(), 1);
}

BOOST_AUTO_TEST_CASE(insert_middle)
{
  loseva::List< int > lst;
  lst.push_back(1);
  lst.push_back(3);

  loseva::List< int >::iterator it = lst.begin();
  ++it;

  lst.insert(it, 2);

  it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(erase_middle)
{
  loseva::List< int > lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);

  loseva::List< int >::iterator it = lst.begin();
  ++it;

  lst.erase(it);

  it = lst.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  loseva::List< int > lst1;
  lst1.push_back(1);
  lst1.push_back(2);

  loseva::List< int > lst2(lst1);

  BOOST_CHECK_EQUAL(lst2.size(), 2);

  loseva::List< int >::iterator it = lst2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);

  BOOST_CHECK_EQUAL(lst1.size(), 2);
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  loseva::List< int > lst1;
  lst1.push_back(5);
  lst1.push_back(6);

  loseva::List< int > lst2;
  lst2.push_back(7);

  lst2 = lst1;

  loseva::List< int >::iterator it = lst2.begin();
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK_EQUAL(*it, 6);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  loseva::List< int > lst1;
  lst1.push_back(10);
  lst1.push_back(20);

  loseva::List< int > lst2(std::move(lst1));

  BOOST_CHECK(lst1.empty());

  loseva::List< int >::iterator it = lst2.begin();
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  loseva::List< int > lst1;
  lst1.push_back(10);
  lst1.push_back(20);

  loseva::List< int > lst2;
  lst2.push_back(30);

  lst2 = std::move(lst1);

  BOOST_CHECK(lst1.empty());

  loseva::List< int >::iterator it = lst2.begin();
  BOOST_CHECK_EQUAL(*it, 10);
  ++it;
  BOOST_CHECK_EQUAL(*it, 20);
}

BOOST_AUTO_TEST_CASE(clear)
{
  loseva::List< int > lst;
  lst.push_back(1);
  lst.push_back(2);

  lst.clear();

  BOOST_CHECK(lst.empty());
}

BOOST_AUTO_TEST_CASE(bitwise_or_logic)
{
  int res = 0;

  res |= 1;
  res |= 2;
  res |= 4;

  BOOST_CHECK_EQUAL(res, 7);
}

BOOST_AUTO_TEST_SUITE_END()
