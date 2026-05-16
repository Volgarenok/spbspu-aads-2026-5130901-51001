#define BOOST_TEST_MODULE S3
#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"
#include "siphash.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(IteratorTests)

BOOST_AUTO_TEST_CASE(IteratorBeginEndEmpty)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  BOOST_CHECK(table.begin() == table.end());
}

BOOST_AUTO_TEST_CASE(IteratorIterateOverElements)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  int count = 0;

  for (vishnyakov::HTIter< int, std::string, vishnyakov::SipHash, std::equal_to< int > > it = table.begin();
       it != table.end();
       ++it)
  {
    ++count;
  }

  BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_CASE(IteratorConstIterateOverElements)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  const auto& const_table = table;
  int count = 0;

  for (vishnyakov::HTCIter< int, std::string, vishnyakov::SipHash, std::equal_to< int > > it = const_table.begin();
       it != const_table.end();
       ++it)
  {
    ++count;
  }

  BOOST_CHECK_EQUAL(count, 3);
}

BOOST_AUTO_TEST_CASE(IteratorDereference)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(42, "answer");

  vishnyakov::HTIter< int, std::string, vishnyakov::SipHash, std::equal_to< int > > it = table.begin();
  BOOST_CHECK_EQUAL(it->first, 42);
  BOOST_CHECK_EQUAL(it->second, "answer");
  BOOST_CHECK_EQUAL((*it).second, "answer");
}

BOOST_AUTO_TEST_CASE(ConstIteratorDereference)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(42, "answer");

  vishnyakov::HTCIter< int, std::string, vishnyakov::SipHash, std::equal_to< int > > it = table.cbegin();
  BOOST_CHECK_EQUAL(it->first, 42);
  BOOST_CHECK_EQUAL(it->second, "answer");
  BOOST_CHECK_EQUAL((*it).second, "answer");
}

BOOST_AUTO_TEST_CASE(IteratorPrefixIncrement)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.add(2, "two");

  vishnyakov::HTIter< int, std::string, vishnyakov::SipHash, std::equal_to< int > > it = table.begin();
  vishnyakov::HTIter< int, std::string, vishnyakov::SipHash, std::equal_to< int > >& ref = ++it;
  BOOST_CHECK(it == ref);
}

BOOST_AUTO_TEST_CASE(IteratorPostfixIncrement)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");

  vishnyakov::HTIter< int, std::string, vishnyakov::SipHash, std::equal_to< int > > it = table.begin();
  vishnyakov::HTIter< int, std::string, vishnyakov::SipHash, std::equal_to< int > > old = it++;
  BOOST_CHECK(old != it);
  BOOST_CHECK(it == table.end());
}

BOOST_AUTO_TEST_CASE(IteratorConvertToConstIterator)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");

  vishnyakov::HTIter< int, std::string, vishnyakov::SipHash, std::equal_to< int > > it = table.begin();
  vishnyakov::HTCIter< int, std::string, vishnyakov::SipHash, std::equal_to< int > > cit = it;

  BOOST_CHECK_EQUAL(cit->first, 1);
  BOOST_CHECK_EQUAL(cit->second, "one");
}

BOOST_AUTO_TEST_CASE(RangeBasedForLoop)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  int sum = 0;

  for (const auto& pair : table)
  {
    sum += pair.first;
  }

  BOOST_CHECK_EQUAL(sum, 6);
}

BOOST_AUTO_TEST_CASE(ConstRangeBasedForLoop)
{
  vishnyakov::HashTable< int, std::string, vishnyakov::SipHash, std::equal_to< int > > table;
  table.add(1, "one");
  table.add(2, "two");
  table.add(3, "three");

  const auto& const_table = table;
  int sum = 0;

  for (const auto& pair : const_table)
  {
    sum += pair.first;
  }

  BOOST_CHECK_EQUAL(sum, 6);
}

BOOST_AUTO_TEST_SUITE_END()

