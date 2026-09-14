#include <boost/test/unit_test.hpp>

#include "bstree.hpp"

using namespace hachaturyanov;
using Tree = BSTree< int, std::string, std::less< int > >;

BOOST_AUTO_TEST_CASE(test_bstree)
{
  Tree t;

  BOOST_TEST(t.isEmpty());
}

BOOST_AUTO_TEST_CASE(test_push_get)
{
  Tree t;

  t.push(1, "one");
  t.push(2, "two");
  t.push(3, "three");

  BOOST_TEST(!t.isEmpty());
  BOOST_TEST(t.get(1) == "one");
  BOOST_TEST(t.get(2) == "two");
  BOOST_TEST(t.get(3) == "three");
}

BOOST_AUTO_TEST_CASE(test_push_double)
{
  Tree t;

  t.push(1, "one");
  BOOST_TEST(t.get(1) == "one");

  t.push(1, "two");
  BOOST_TEST(t.get(1) == "two");
}

BOOST_AUTO_TEST_CASE(test_get_missing)
{
  Tree t;

  BOOST_CHECK_THROW(t.get(2), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_drop_leaf)
{
  Tree t;

  t.push(1, "one");
  t.push(2, "two");
  t.push(3, "three");

  t.drop(3);

  BOOST_CHECK_THROW(t.get(3), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_drop_node_with_one_child)
{
  Tree t;

  t.push(1, "one");
  t.push(2, "two");
  t.push(3, "three");

  t.drop(2);

  BOOST_CHECK_THROW(t.get(2), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_drop_node_with_two_children)
{
  Tree t;

  t.push(1, "one");
  t.push(2, "two");
  t.push(3, "three");

  t.drop(1);

  BOOST_CHECK_THROW(t.get(1), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_drop_missing)
{
  Tree t;

  BOOST_CHECK_THROW(t.drop(2), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_drop_root)
{
  Tree t;

  t.push(1, "one");
  t.push(2, "two");
  t.push(3, "three");

  t.drop(1);

  BOOST_CHECK_THROW(t.get(1), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_empty_begin_end)
{
  Tree t;

  BOOST_CHECK(t.begin() == t.end());
  BOOST_CHECK(t.cbegin() == t.cend());
}

BOOST_AUTO_TEST_CASE(test_iterator)
{
  Tree t;

  t.push(1, "one");
  t.push(2, "two");
  t.push(3, "three");

  auto it = t.begin();
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == "one");

  ++it;
  BOOST_TEST(it->first == 2);
  BOOST_TEST(it->second == "two");

  ++it;
  BOOST_TEST(it->first == 3);
  BOOST_TEST(it->second == "three");

  ++it;
  BOOST_CHECK(it == t.end());
}

BOOST_AUTO_TEST_CASE(test_const_iterator)
{
  Tree t;

  t.push(1, "one");
  t.push(2, "two");
  t.push(3, "three");

  const Tree &ct = t;

  auto it = ct.begin();
  BOOST_TEST(it->first == 1);
  BOOST_TEST(it->second == "one");

  ++it;
  BOOST_TEST(it->first == 2);
  BOOST_TEST(it->second == "two");

  ++it;
  BOOST_TEST(it->first == 3);
  BOOST_TEST(it->second == "three");

  ++it;
  BOOST_CHECK(it == ct.end());
}

BOOST_AUTO_TEST_CASE(test_iterator_const_iterator)
{
  Tree t;

  t.push(1, "one");
  t.push(2, "two");
  t.push(3, "three");

  auto it = t.begin();
  BSTConstIterator< int, std::string > cit(it);

  BOOST_TEST(cit->first == 1);
  BOOST_TEST(cit->second == "one");

  ++cit;
  BOOST_TEST(cit->first == 2);
  BOOST_TEST(cit->second == "two");

  ++cit;
  BOOST_TEST(cit->first == 3);
  BOOST_TEST(cit->second == "three");

  ++cit;
  BOOST_CHECK(cit == t.end());
}

BOOST_AUTO_TEST_CASE(test_height)
{
  Tree t;

  BOOST_TEST(t.height() == 0);

  t.push(1, "one");
  BOOST_TEST(t.height() == 1);

  t.push(2, "two");
  BOOST_TEST(t.height() == 2);

  t.push(3, "three");
  BOOST_TEST(t.height() == 3);

}

BOOST_AUTO_TEST_CASE(test_rotate_left)
{
  Tree t;

  t.push(1, "one");
  t.push(2, "two");

  auto it = t.begin();
  ++it;
  auto newRoot = t.rotateLeft(it);

  auto cur = t.begin();
  BOOST_TEST(cur->first == 1);
  ++cur;
  BOOST_TEST(cur->first == 2);
  ++cur;
  BOOST_CHECK(cur == t.end());

  BOOST_TEST(newRoot->first == 2);

  BOOST_TEST(t.height() == 2);
}

BOOST_AUTO_TEST_CASE(test_rotate_right)
{
    Tree t;
    t.push(5, "five");
    t.push(3, "three");

    auto it = t.begin();
    auto newRoot = t.rotateRight(it);

    BOOST_CHECK(newRoot->first == 3);

    auto cur = t.begin();
    BOOST_CHECK(cur->first == 3);
    ++cur;
    BOOST_CHECK(cur->first == 5);
    ++cur;
    BOOST_CHECK(cur == t.end());

    BOOST_CHECK(t.height() == 2);
}

BOOST_AUTO_TEST_CASE(test_rotate_large_left)
{
  Tree t;

  t.push(1, "one");
  t.push(3, "three");
  t.push(2, "two");

  auto it = t.begin();
  ++it;
  BOOST_TEST(t.height() == 3);

  auto newRoot = t.rotateLargeLeft(it);

  BOOST_TEST(newRoot->first == 2);

  auto cur = t.begin();
  BOOST_TEST(cur->first == 1);
  ++cur;
  BOOST_TEST(cur->first == 2);
  ++cur;
  BOOST_TEST(cur->first == 3);
  ++cur;
  BOOST_CHECK(cur == t.end());

  BOOST_TEST(t.height() == 2);
}

BOOST_AUTO_TEST_CASE(test_rotate_large_right)
{
  Tree t;

  t.push(3, "three");
  t.push(1, "one");
  t.push(2, "two");

  auto it = t.begin();
  ++it;
  BOOST_TEST(t.height() == 3);

  auto newRoot = t.rotateLargeRight(it);

  BOOST_TEST(newRoot->first == 2);

  auto cur = t.begin();
  BOOST_TEST(cur->first == 1);
  ++cur;
  BOOST_TEST(cur->first == 2);
  ++cur;
  BOOST_TEST(cur->first == 3);
  ++cur;
  BOOST_CHECK(cur == t.end());

  BOOST_TEST(t.height() == 2);
}
