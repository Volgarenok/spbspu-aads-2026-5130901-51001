#include <boost/test/unit_test.hpp>
#include "multimap.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(MultimapTests)

BOOST_AUTO_TEST_CASE(MultimapInsertAndCount)
{
  karpenko::Multimap< int, std::string > mm;
  mm.insert(1, "one");
  mm.insert(1, "uno");
  mm.insert(2, "two");
  mm.insert(2, "dos");
  mm.insert(2, "zwei");

  BOOST_CHECK_EQUAL(mm.size(), 5);
  BOOST_CHECK_EQUAL(mm.count(1), 2);
  BOOST_CHECK_EQUAL(mm.count(2), 3);
  BOOST_CHECK_EQUAL(mm.count(3), 0);
  BOOST_CHECK(mm.contains(1));
  BOOST_CHECK(mm.contains(2));
  BOOST_CHECK(!mm.contains(3));

  const auto& list1 = mm.at(1);
  BOOST_CHECK_EQUAL(list1.size(), 2);
  auto it1 = list1.begin();
  BOOST_CHECK_EQUAL(*it1, "one");
  ++it1;
  BOOST_CHECK_EQUAL(*it1, "uno");

  const auto& list2 = mm.at(2);
  BOOST_CHECK_EQUAL(list2.size(), 3);
  auto it2 = list2.begin();
  BOOST_CHECK_EQUAL(*it2, "two");
  ++it2;
  BOOST_CHECK_EQUAL(*it2, "dos");
  ++it2;
  BOOST_CHECK_EQUAL(*it2, "zwei");
}

BOOST_AUTO_TEST_CASE(MultimapEraseOne)
{
  karpenko::Multimap< int, std::string > mm;
  mm.insert(1, "one");
  mm.insert(1, "uno");
  mm.insert(2, "two");

  BOOST_CHECK_EQUAL(mm.count(1), 2);
  BOOST_CHECK(mm.erase_one(1));
  BOOST_CHECK_EQUAL(mm.count(1), 1);
  BOOST_CHECK_EQUAL(mm.size(), 2);

  BOOST_CHECK(mm.erase_one(1));
  BOOST_CHECK_EQUAL(mm.count(1), 0);
  BOOST_CHECK_EQUAL(mm.size(), 1);
  BOOST_CHECK(!mm.contains(1));

  BOOST_CHECK(!mm.erase_one(1));
  BOOST_CHECK_EQUAL(mm.size(), 1);
}

BOOST_AUTO_TEST_CASE(MultimapEraseAll)
{
  karpenko::Multimap< int, std::string > mm;
  mm.insert(1, "one");
  mm.insert(1, "uno");
  mm.insert(2, "two");

  size_t removed = mm.erase_all(1);
  BOOST_CHECK_EQUAL(removed, 2);
  BOOST_CHECK_EQUAL(mm.count(1), 0);
  BOOST_CHECK_EQUAL(mm.size(), 1);
  BOOST_CHECK(mm.contains(2));

  removed = mm.erase_all(3);
  BOOST_CHECK_EQUAL(removed, 0);
  BOOST_CHECK_EQUAL(mm.size(), 1);
}

BOOST_AUTO_TEST_CASE(MultimapClear)
{
  karpenko::Multimap< int, std::string > mm;
  mm.insert(1, "one");
  mm.insert(2, "two");
  BOOST_CHECK_EQUAL(mm.size(), 2);

  mm.clear();
  BOOST_CHECK(mm.empty());
  BOOST_CHECK_EQUAL(mm.size(), 0);
  BOOST_CHECK(!mm.contains(1));
}

BOOST_AUTO_TEST_CASE(MultimapHeight)
{
  karpenko::Multimap< int, int > mm;
  for (int i = 1; i <= 50; ++i)
  {
    mm.insert(i, i);
  }
  BOOST_CHECK_LE(mm.height(), 20);
}

BOOST_AUTO_TEST_SUITE_END()
