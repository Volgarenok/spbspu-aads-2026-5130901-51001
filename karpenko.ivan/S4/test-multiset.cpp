#include <boost/test/unit_test.hpp>
#include "../common/multiset.hpp"

BOOST_AUTO_TEST_SUITE(MultisetTests)

BOOST_AUTO_TEST_CASE(MultisetInsertAndCount)
{
  karpenko::Multiset< int > ms;
  ms.insert(1);
  ms.insert(1);
  ms.insert(2);
  ms.insert(3);
  ms.insert(3);
  ms.insert(3);

  BOOST_CHECK_EQUAL(ms.size(), 6);
  BOOST_CHECK_EQUAL(ms.count(1), 2);
  BOOST_CHECK_EQUAL(ms.count(2), 1);
  BOOST_CHECK_EQUAL(ms.count(3), 3);
  BOOST_CHECK_EQUAL(ms.count(4), 0);
  BOOST_CHECK(ms.contains(1));
  BOOST_CHECK(ms.contains(2));
  BOOST_CHECK(ms.contains(3));
  BOOST_CHECK(!ms.contains(4));
}

BOOST_AUTO_TEST_CASE(MultisetEraseOne)
{
  karpenko::Multiset< int > ms;
  ms.insert(1);
  ms.insert(1);
  ms.insert(2);

  BOOST_CHECK_EQUAL(ms.count(1), 2);
  BOOST_CHECK(ms.erase_one(1));
  BOOST_CHECK_EQUAL(ms.count(1), 1);
  BOOST_CHECK_EQUAL(ms.size(), 2);

  BOOST_CHECK(ms.erase_one(1));
  BOOST_CHECK_EQUAL(ms.count(1), 0);
  BOOST_CHECK_EQUAL(ms.size(), 1);
  BOOST_CHECK(!ms.contains(1));

  BOOST_CHECK(!ms.erase_one(1));
  BOOST_CHECK_EQUAL(ms.size(), 1);
}

BOOST_AUTO_TEST_CASE(MultisetEraseAll)
{
  karpenko::Multiset< int > ms;
  ms.insert(1);
  ms.insert(1);
  ms.insert(1);
  ms.insert(2);

  size_t removed = ms.erase_all(1);
  BOOST_CHECK_EQUAL(removed, 3);
  BOOST_CHECK_EQUAL(ms.count(1), 0);
  BOOST_CHECK_EQUAL(ms.size(), 1);
  BOOST_CHECK(ms.contains(2));

  removed = ms.erase_all(3);
  BOOST_CHECK_EQUAL(removed, 0);
  BOOST_CHECK_EQUAL(ms.size(), 1);
}

BOOST_AUTO_TEST_CASE(MultisetIterator)
{
  karpenko::Multiset< int > ms;
  ms.insert(3);
  ms.insert(1);
  ms.insert(1);
  ms.insert(2);

  auto it = ms.begin();
  BOOST_CHECK_EQUAL(it->first, 1);
  BOOST_CHECK_EQUAL(it->second, 2);
  ++it;
  BOOST_CHECK_EQUAL(it->first, 2);
  BOOST_CHECK_EQUAL(it->second, 1);
  ++it;
  BOOST_CHECK_EQUAL(it->first, 3);
  BOOST_CHECK_EQUAL(it->second, 1);
  ++it;
  BOOST_CHECK(it == ms.end());
}

BOOST_AUTO_TEST_CASE(MultisetHeight)
{
  karpenko::Multiset< int > ms;
  for (int i = 1; i <= 100; ++i)
  {
    ms.insert(i);
  }
  BOOST_CHECK_LE(ms.height(), 20);
}

BOOST_AUTO_TEST_SUITE_END()
