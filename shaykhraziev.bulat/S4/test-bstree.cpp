#include <boost/test/unit_test.hpp>

#include <bstree.hpp>

#include <stdexcept>
#include <string>

namespace
{
  struct IntLess
  {
    bool operator()(int lhs, int rhs) const
    {
      return lhs < rhs;
    }
  };

  using Tree = shaykhraziev::BSTree< int, std::string, IntLess >;
}

BOOST_AUTO_TEST_CASE(bstree_default_constructor_creates_empty_tree)
{
  Tree tree;

  BOOST_CHECK(tree.empty());
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(bstree_push_one_element)
{
  Tree tree;

  BOOST_CHECK(tree.push(2, "two"));

  BOOST_CHECK(!tree.empty());
  BOOST_TEST(tree.size() == 1);
  BOOST_CHECK(tree.has(2));
  BOOST_TEST(tree.get(2) == "two");
}

BOOST_AUTO_TEST_CASE(bstree_push_many_elements_and_find_them)
{
  Tree tree;

  tree.push(4, "four");
  tree.push(2, "two");
  tree.push(6, "six");
  tree.push(1, "one");
  tree.push(3, "three");

  BOOST_TEST(tree.size() == 5);
  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(tree.has(2));
  BOOST_CHECK(tree.has(3));
  BOOST_CHECK(tree.has(4));
  BOOST_CHECK(tree.has(6));
  BOOST_CHECK(!tree.has(5));
}

BOOST_AUTO_TEST_CASE(bstree_get_returns_mutable_reference)
{
  Tree tree;
  tree.push(1, "one");

  tree.get(1) = "changed";

  BOOST_TEST(tree.get(1) == "changed");
}

BOOST_AUTO_TEST_CASE(bstree_const_get_returns_value)
{
  Tree tree;
  tree.push(1, "one");
  const Tree& constTree = tree;

  BOOST_TEST(constTree.get(1) == "one");
  BOOST_CHECK_THROW(constTree.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(bstree_push_existing_key_replaces_value)
{
  Tree tree;

  BOOST_CHECK(tree.push(1, "one"));
  BOOST_CHECK(!tree.push(1, "uno"));

  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(1) == "uno");
}

BOOST_AUTO_TEST_CASE(bstree_drop_leaf)
{
  Tree tree;
  tree.push(2, "two");
  tree.push(1, "one");
  tree.push(3, "three");

  BOOST_CHECK(tree.drop(1));

  BOOST_TEST(tree.size() == 2);
  BOOST_CHECK(!tree.has(1));
  BOOST_CHECK(tree.has(2));
  BOOST_CHECK(tree.has(3));
}

BOOST_AUTO_TEST_CASE(bstree_drop_node_with_one_child)
{
  Tree tree;
  tree.push(4, "four");
  tree.push(2, "two");
  tree.push(1, "one");

  BOOST_CHECK(tree.drop(2));

  BOOST_TEST(tree.size() == 2);
  BOOST_CHECK(!tree.has(2));
  BOOST_TEST(tree.get(1) == "one");
  BOOST_TEST(tree.get(4) == "four");
}

BOOST_AUTO_TEST_CASE(bstree_drop_node_with_two_children)
{
  Tree tree;
  tree.push(4, "four");
  tree.push(2, "two");
  tree.push(6, "six");
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(5, "five");
  tree.push(7, "seven");

  BOOST_CHECK(tree.drop(4));

  BOOST_TEST(tree.size() == 6);
  BOOST_CHECK(!tree.has(4));
  BOOST_TEST(tree.get(1) == "one");
  BOOST_TEST(tree.get(2) == "two");
  BOOST_TEST(tree.get(3) == "three");
  BOOST_TEST(tree.get(5) == "five");
  BOOST_TEST(tree.get(6) == "six");
  BOOST_TEST(tree.get(7) == "seven");
}

BOOST_AUTO_TEST_CASE(bstree_drop_root_until_empty)
{
  Tree tree;
  tree.push(2, "two");
  tree.push(1, "one");

  BOOST_CHECK(tree.drop(2));
  BOOST_CHECK(tree.drop(1));

  BOOST_CHECK(tree.empty());
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(bstree_drop_missing_key_returns_false)
{
  Tree tree;
  tree.push(1, "one");

  BOOST_CHECK(!tree.drop(2));

  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(1) == "one");
}
