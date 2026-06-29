#include <boost/test/unit_test.hpp>
#include "set.hpp"

BOOST_AUTO_TEST_SUITE(SetTests)

BOOST_AUTO_TEST_CASE(SetInsertAndContains)
{
  karpenko::Set< int > set;
  set.insert(5);
  set.insert(3);
  set.insert(7);

  BOOST_CHECK(set.contains(5));
  BOOST_CHECK(set.contains(3));
  BOOST_CHECK(set.contains(7));
  BOOST_CHECK(!set.contains(1));
  BOOST_CHECK_EQUAL(set.size(), 3);

  set.insert(3); // дубликат
  BOOST_CHECK_EQUAL(set.size(), 3);
}

BOOST_AUTO_TEST_CASE(SetOrder)
{
  karpenko::Set< int > set;
  set.insert(5);
  set.insert(1);
  set.insert(4);
  set.insert(2);
  set.insert(3);

  auto it = set.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 3);
  ++it;
  BOOST_CHECK_EQUAL(*it, 4);
  ++it;
  BOOST_CHECK_EQUAL(*it, 5);
  ++it;
  BOOST_CHECK(it == set.end());
}

BOOST_AUTO_TEST_CASE(SetErase)
{
  karpenko::Set< std::string > set;
  set.insert("apple");
  set.insert("banana");
  set.insert("cherry");

  BOOST_CHECK_EQUAL(set.size(), 3);

  bool removed = set.erase("banana");
  BOOST_CHECK(removed);
  BOOST_CHECK_EQUAL(set.size(), 2);
  BOOST_CHECK(!set.contains("banana"));
  BOOST_CHECK(set.contains("apple"));
  BOOST_CHECK(set.contains("cherry"));

  removed = set.erase("grape");
  BOOST_CHECK(!removed);
  BOOST_CHECK_EQUAL(set.size(), 2);
}

BOOST_AUTO_TEST_CASE(SetClear)
{
  karpenko::Set< int > set;
  set.insert(1);
  set.insert(2);
  set.insert(3);
  BOOST_CHECK_EQUAL(set.size(), 3);

  set.clear();
  BOOST_CHECK(set.empty());
  BOOST_CHECK_EQUAL(set.size(), 0);
  BOOST_CHECK(!set.contains(1));
  BOOST_CHECK(set.begin() == set.end());
}

BOOST_AUTO_TEST_CASE(SetHeight)
{
  karpenko::Set< int > set;
  for (int i = 1; i <= 100; ++i)
  {
    set.insert(i);
  }
  BOOST_CHECK_LE(set.height(), 20);
}

BOOST_AUTO_TEST_SUITE_END()
