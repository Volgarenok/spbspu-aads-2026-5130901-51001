#include <stdexcept>
#include <string>

#include <boost/test/unit_test.hpp>

#include "../common/robin_hood_hash.hpp"

BOOST_AUTO_TEST_SUITE(RobinHoodHashTableTests)

BOOST_AUTO_TEST_CASE(ConstructorDefault)
{
  karpenko::RobinHoodHashTable< std::string, std::string > ht;
  BOOST_TEST(ht.empty());
  BOOST_TEST(ht.size() == 0);
}

BOOST_AUTO_TEST_CASE(InsertAndContains)
{
  karpenko::RobinHoodHashTable< std::string, std::string > ht;
  ht.insert("1", "one");
  ht.insert("2", "two");
  ht.insert("3", "three");
  BOOST_TEST(ht.size() == 3);
  BOOST_TEST(!ht.empty());
  BOOST_TEST(ht.contains("1"));
  BOOST_TEST(ht.contains("2"));
  BOOST_TEST(ht.contains("3"));
  BOOST_TEST(!ht.contains("4"));
}

BOOST_AUTO_TEST_CASE(InsertAndUpdate)
{
  karpenko::RobinHoodHashTable< std::string, std::string > ht;
  ht.insert("1", "first");
  BOOST_TEST(ht.at("1") == "first");
  ht.insert("1", "updated");
  BOOST_TEST(ht.size() == 1);
  BOOST_TEST(ht.at("1") == "updated");
}

BOOST_AUTO_TEST_CASE(AtThrowsForMissingKey)
{
  karpenko::RobinHoodHashTable< std::string, std::string > ht;
  ht.insert("1", "one");
  BOOST_CHECK_THROW(ht.at("2"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Erase)
{
  karpenko::RobinHoodHashTable< std::string, std::string > ht;
  ht.insert("1", "one");
  ht.insert("2", "two");
  ht.insert("3", "three");
  BOOST_TEST(ht.erase("2"));
  BOOST_TEST(ht.size() == 2);
  BOOST_TEST(!ht.contains("2"));
  BOOST_TEST(ht.contains("1"));
  BOOST_TEST(ht.contains("3"));
}

BOOST_AUTO_TEST_CASE(EraseNonExistent)
{
  karpenko::RobinHoodHashTable< std::string, std::string > ht;
  ht.insert("1", "one");
  BOOST_TEST(!ht.erase("2"));
  BOOST_TEST(ht.size() == 1);
}

BOOST_AUTO_TEST_CASE(SizeAndEmpty)
{
  karpenko::RobinHoodHashTable< std::string, int > ht;
  BOOST_TEST(ht.empty());
  ht.insert("10", 100);
  BOOST_TEST(!ht.empty());
  BOOST_TEST(ht.size() == 1);
  ht.erase("10");
  BOOST_TEST(ht.empty());
}

BOOST_AUTO_TEST_CASE(IteratorBasic)
{
  karpenko::RobinHoodHashTable< std::string, int > ht;
  for (int i = 0; i < 10; ++i)
  {
    ht.insert(std::to_string(i), i * i);
  }
  int sum = 0;
  for (auto it = ht.begin(); it != ht.end(); ++it)
  {
    sum += (*it).second;
  }
  BOOST_TEST(sum == 285);
}

BOOST_AUTO_TEST_CASE(IteratorConst)
{
  karpenko::RobinHoodHashTable< std::string, int > ht;
  ht.insert("1", 10);
  ht.insert("2", 20);
  const auto& const_ht = ht;
  int sum = 0;
  for (auto it = const_ht.begin(); it != const_ht.end(); ++it)
  {
    sum += (*it).second;
  }
  BOOST_TEST(sum == 30);
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  karpenko::RobinHoodHashTable< std::string, int > original;
  original.insert("1", 10);
  original.insert("2", 20);
  karpenko::RobinHoodHashTable< std::string, int > copy = original;
  BOOST_TEST(copy.size() == 2);
  BOOST_TEST(copy.contains("1"));
  BOOST_TEST(copy.at("2") == 20);
  original.erase("1");
  BOOST_TEST(original.size() == 1);
  BOOST_TEST(copy.size() == 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  karpenko::RobinHoodHashTable< std::string, int > original;
  original.insert("1", 100);
  original.insert("2", 200);
  karpenko::RobinHoodHashTable< std::string, int > moved = std::move(original);
  BOOST_TEST(moved.size() == 2);
  BOOST_TEST(moved.at("1") == 100);
  BOOST_TEST(original.empty());
  BOOST_TEST(original.size() == 0);
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  karpenko::RobinHoodHashTable< std::string, int > a, b;
  a.insert("1", 10);
  a.insert("2", 20);
  b = a;
  BOOST_TEST(b.size() == 2);
  BOOST_TEST(b.contains("1"));
  BOOST_TEST(b.at("2") == 20);
  a.erase("1");
  BOOST_TEST(b.contains("1"));
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  karpenko::RobinHoodHashTable< std::string, int > a, b;
  a.insert("1", 1);
  a.insert("2", 2);
  b = std::move(a);
  BOOST_TEST(b.size() == 2);
  BOOST_TEST(b.contains("1"));
  BOOST_TEST(a.empty());
}

BOOST_AUTO_TEST_CASE(RehashAndGrowth)
{
  karpenko::RobinHoodHashTable< std::string, int > ht(4);
  for (int i = 0; i < 100; ++i)
  {
    ht.insert(std::to_string(i), i);
  }
  BOOST_TEST(ht.size() == 100);
  for (int i = 0; i < 100; ++i)
  {
    BOOST_TEST(ht.contains(std::to_string(i)));
  }
}

BOOST_AUTO_TEST_CASE(Clear)
{
  karpenko::RobinHoodHashTable< std::string, int > ht;
  ht.insert("1", 10);
  ht.insert("2", 20);
  ht.insert("3", 30);
  ht = karpenko::RobinHoodHashTable< std::string, int >();
  BOOST_TEST(ht.empty());
  BOOST_TEST(ht.size() == 0);
  BOOST_TEST(!ht.contains("1"));
}

BOOST_AUTO_TEST_CASE(EraseThenInsert)
{
  karpenko::RobinHoodHashTable< std::string, int > ht;
  ht.insert("1", 100);
  ht.erase("1");
  BOOST_TEST(!ht.contains("1"));
  ht.insert("1", 200);
  BOOST_TEST(ht.contains("1"));
  BOOST_TEST(ht.at("1") == 200);
}

BOOST_AUTO_TEST_SUITE_END()
