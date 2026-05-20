#include <stdexcept>

#include <boost/test/unit_test.hpp>

#include "../common/list.hpp"

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(ConstructorDefault)
{
  karpenko::List< int > lst;
  BOOST_TEST(lst.empty());
  BOOST_TEST(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushFront)
{
  karpenko::List< int > lst;
  lst.pushFront(1);
  lst.pushFront(2);
  lst.pushFront(3);
  BOOST_TEST(lst.size() == 3);
  BOOST_TEST(lst.front() == 3);
  BOOST_TEST(lst.back() == 1);
  auto it = lst.begin();
  BOOST_TEST(*it == 3);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(PushBack)
{
  karpenko::List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);
  BOOST_TEST(lst.size() == 3);
  BOOST_TEST(lst.front() == 1);
  BOOST_TEST(lst.back() == 3);
  auto it = lst.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(PopFront)
{
  karpenko::List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);
  lst.popFront();
  BOOST_TEST(lst.size() == 2);
  BOOST_TEST(lst.front() == 2);
  lst.popFront();
  BOOST_TEST(lst.size() == 1);
  BOOST_TEST(lst.front() == 3);
  lst.popFront();
  BOOST_TEST(lst.empty());
  BOOST_CHECK_THROW(lst.popFront(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(PopBack)
{
  karpenko::List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);
  lst.popBack();
  BOOST_TEST(lst.size() == 2);
  BOOST_TEST(lst.back() == 2);
  lst.popBack();
  BOOST_TEST(lst.size() == 1);
  BOOST_TEST(lst.back() == 1);
  lst.popBack();
  BOOST_TEST(lst.empty());
  BOOST_CHECK_THROW(lst.popBack(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(FrontBackOnEmpty)
{
  karpenko::List< int > lst;
  BOOST_CHECK_THROW(lst.front(), std::out_of_range);
  BOOST_CHECK_THROW(lst.back(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(InsertAfter)
{
  karpenko::List< int > lst;
  lst.pushBack(1);
  lst.pushBack(3);
  auto it = lst.begin();
  lst.insertAfter(it, 2);
  BOOST_TEST(lst.size() == 3);
  auto check = lst.begin();
  BOOST_TEST(*check == 1);
  ++check;
  BOOST_TEST(*check == 2);
  ++check;
  BOOST_TEST(*check == 3);
}

BOOST_AUTO_TEST_CASE(InsertAfterEnd)
{
  karpenko::List< int > lst;
  lst.pushBack(1);
  auto it = lst.begin();
  lst.insertAfter(it, 2);
  BOOST_TEST(lst.size() == 2);
  auto check = lst.begin();
  BOOST_TEST(*check == 1);
  ++check;
  BOOST_TEST(*check == 2);
}

BOOST_AUTO_TEST_CASE(EraseAfter)
{
  karpenko::List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);
  auto it = lst.begin();
  lst.eraseAfter(it);
  BOOST_TEST(lst.size() == 2);
  auto check = lst.begin();
  BOOST_TEST(*check == 1);
  ++check;
  BOOST_TEST(*check == 3);
}

BOOST_AUTO_TEST_CASE(EraseAfterInvalid)
{
  karpenko::List< int > lst;
  lst.pushBack(1);
  auto it = lst.begin();
  BOOST_CHECK_THROW(lst.eraseAfter(it), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  karpenko::List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);
  lst.clear();
  BOOST_TEST(lst.empty());
  BOOST_TEST(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(IteratorTraversal)
{
  karpenko::List< int > lst;
  lst.pushBack(1);
  lst.pushBack(2);
  lst.pushBack(3);
  int sum = 0;
  for (auto it = lst.begin(); it != lst.end(); ++it)
  {
    sum += *it;
  }
  BOOST_TEST(sum == 6);
}

BOOST_AUTO_TEST_CASE(ConstIterator)
{
  karpenko::List< int > lst;
  lst.pushBack(10);
  lst.pushBack(20);
  const auto& const_lst = lst;
  int sum = 0;
  for (auto it = const_lst.begin(); it != const_lst.end(); ++it)
  {
    sum += *it;
  }
  BOOST_TEST(sum == 30);
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  karpenko::List< int > original;
  original.pushBack(1);
  original.pushBack(2);
  karpenko::List< int > copy = original;
  BOOST_TEST(copy.size() == 2);
  auto it = copy.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  original.pushBack(3);
  BOOST_TEST(original.size() == 3);
  BOOST_TEST(copy.size() == 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  karpenko::List< int > original;
  original.pushBack(1);
  original.pushBack(2);
  karpenko::List< int > moved = std::move(original);
  BOOST_TEST(moved.size() == 2);
  auto it = moved.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  BOOST_TEST(original.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  karpenko::List< int > a, b;
  a.pushBack(1);
  a.pushBack(2);
  b = a;
  BOOST_TEST(b.size() == 2);
  auto it = b.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  a.pushBack(3);
  BOOST_TEST(a.size() == 3);
  BOOST_TEST(b.size() == 2);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  karpenko::List< int > a, b;
  a.pushBack(1);
  a.pushBack(2);
  b = std::move(a);
  BOOST_TEST(b.size() == 2);
  auto it = b.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  BOOST_TEST(a.empty());
}

BOOST_AUTO_TEST_CASE(Size)
{
  karpenko::List< int > lst;
  BOOST_TEST(lst.size() == 0);
  lst.pushBack(1);
  BOOST_TEST(lst.size() == 1);
  lst.pushBack(2);
  BOOST_TEST(lst.size() == 2);
  lst.popFront();
  BOOST_TEST(lst.size() == 1);
  lst.clear();
  BOOST_TEST(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(Swap)
{
  karpenko::List< int > a, b;
  a.pushBack(1);
  a.pushBack(2);
  b.pushBack(3);
  b.pushBack(4);
  swap(a, b);
  BOOST_TEST(a.size() == 2);
  BOOST_TEST(b.size() == 2);
  auto it = a.begin();
  BOOST_TEST(*it == 3);
  ++it;
  BOOST_TEST(*it == 4);
  it = b.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_SUITE_END()
