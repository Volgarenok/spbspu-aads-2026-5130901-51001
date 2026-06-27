#include <stdexcept>
#include <initializer_list>
#include <string>
#include <utility>

#include <boost/test/unit_test.hpp>

#include "../common/bstree.hpp"

using Tree = smirnova::BSTree< int, std::string, smirnova::Less< int > >;

static Tree makeTree(std::initializer_list< std::pair < int, const char* > > items)
{
  Tree tree;

  for (const auto& item : items)
  {
    tree.push(item.first, item.second);
  }

  return tree;
}

Tree::iterator find(Tree& tree, int key)
{
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    if (it->first == key)
    {
      return it;
    }
  }

  return tree.end();
}

BOOST_AUTO_TEST_CASE(default_constructor_test)
{
  Tree tree;

  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.height() == 0);
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.cbegin() == tree.cend());
}

BOOST_AUTO_TEST_CASE(push_get_contains_test)
{
  Tree tree = makeTree({{5, "five"}, {2, "two"}, {7, "seven"}});

  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree.contains(5));
  BOOST_TEST(tree.contains(2));
  BOOST_TEST(tree.contains(7));
  BOOST_TEST(!tree.contains(10));
  BOOST_TEST(tree.get(5) == "five");
  BOOST_TEST(tree.get(2) == "two");
  BOOST_TEST(tree.get(7) == "seven");
}

BOOST_AUTO_TEST_CASE(push_updates_value_test)
{
  Tree tree;

  tree.push(1, "one");
  tree.push(1, "new");

  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(1) == "new");
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  Tree first = makeTree({{1, "one"}, {2, "two"}});

  Tree second(first);

  first.drop(1);

  BOOST_TEST(first.size() == 1);
  BOOST_TEST(second.size() == 2);
  BOOST_TEST(!first.contains(1));
  BOOST_TEST(second.contains(1));
  BOOST_TEST(second.get(1) == "one");
}

BOOST_AUTO_TEST_CASE(copy_assignment_test)
{
  Tree first = makeTree({{1, "one"}, {2, "two"}});

  Tree second;
  second.push(10, "ten");
  second = first;

  first.drop(1);

  BOOST_TEST(first.size() == 1);
  BOOST_TEST(second.size() == 2);
  BOOST_TEST(second.contains(1));
  BOOST_TEST(second.get(1) == "one");
}

BOOST_AUTO_TEST_CASE(height_test)
{
  Tree tree = makeTree({{10, "ten"}, {5, "five"}, {15, "fifteen"}});

  BOOST_TEST(tree.height() == 2u);
}

BOOST_AUTO_TEST_CASE(drop_leaf_test)
{
  Tree tree = makeTree({{5, "five"}, {2, "two"}, {7, "seven"}});

  BOOST_TEST(tree.drop(2) == "two");
  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(!tree.contains(2));
  BOOST_TEST(tree.contains(5));
  BOOST_TEST(tree.contains(7));
}

BOOST_AUTO_TEST_CASE(drop_node_with_one_child_test)
{
  Tree tree = makeTree({{5, "five"}, {2, "two"}, {1, "one"}});

  BOOST_TEST(tree.drop(2) == "two");
  BOOST_TEST(tree.size() == 2);
  BOOST_TEST(!tree.contains(2));
  BOOST_TEST(tree.contains(1));
  BOOST_TEST(tree.get(1) == "one");
}

BOOST_AUTO_TEST_CASE(drop_node_with_two_children_test)
{
  Tree tree = makeTree({{5, "five"}, {2, "two"}, {7, "seven"}, {6, "six"}, {8, "eight"}});

  BOOST_TEST(tree.drop(7) == "seven");
  BOOST_TEST(tree.size() == 4);
  BOOST_TEST(!tree.contains(7));
  BOOST_TEST(tree.contains(2));
  BOOST_TEST(tree.contains(5));
  BOOST_TEST(tree.contains(6));
  BOOST_TEST(tree.contains(8));
}

BOOST_AUTO_TEST_CASE(drop_missing_key_test)
{
  Tree tree;

  tree.push(1, "one");

  BOOST_CHECK_THROW(tree.drop(10), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(left_rotate_test)
{
  Tree tree = makeTree({{10, "ten"}, {5, "five"}, {15, "fifteen"}});

  auto rotated = tree.rotateLeft(find(tree, 10));

  BOOST_TEST(rotated->first == 15);
  BOOST_TEST(tree.height() == 3u);
  BOOST_TEST(tree.contains(5));
  BOOST_TEST(tree.contains(10));
  BOOST_TEST(tree.contains(15));
}

BOOST_AUTO_TEST_CASE(right_rotate_test)
{
  Tree tree = makeTree({{10, "ten"}, {5, "five"}, {15, "fifteen"}});

  auto rotated = tree.rotateRight(find(tree, 10));

  BOOST_TEST(rotated->first == 5);
  BOOST_TEST(tree.height() == 3u);
  BOOST_TEST(tree.contains(5));
  BOOST_TEST(tree.contains(10));
  BOOST_TEST(tree.contains(15));
}

BOOST_AUTO_TEST_CASE(large_left_rotate_test)
{
  Tree tree = makeTree({{10, "ten"}, {20, "twenty"}, {15, "fifteen"}});

  auto rotated = tree.rotateLargeLeft(find(tree, 10));

  BOOST_TEST(rotated->first == 15);
  BOOST_TEST(tree.height() == 2u);
  BOOST_TEST(tree.contains(10));
  BOOST_TEST(tree.contains(15));
  BOOST_TEST(tree.contains(20));
}

BOOST_AUTO_TEST_CASE(large_right_rotate_test)
{
  Tree tree = makeTree({{20, "twenty"}, {10, "ten"}, {15, "fifteen"}});

  auto rotated = tree.rotateLargeRight(find(tree, 20));

  BOOST_TEST(rotated->first == 15);
  BOOST_TEST(tree.height() == 2u);
  BOOST_TEST(tree.contains(10));
  BOOST_TEST(tree.contains(15));
  BOOST_TEST(tree.contains(20));
}

BOOST_AUTO_TEST_CASE(rotate_on_end_noop_test)
{
  Tree tree = makeTree({{10, "ten"}, {5, "five"}});

  auto res = tree.rotateLeft(tree.end());
  BOOST_CHECK(res == tree.end());

  BOOST_TEST(tree.contains(5));
  BOOST_TEST(tree.contains(10));
}

BOOST_AUTO_TEST_CASE(rotate_no_child_noop_test)
{
  Tree tree = makeTree({{10, "ten"}, {5, "five"}});

  auto res = tree.rotateLeft(find(tree, 5));
  BOOST_TEST(res->first == 5);

  BOOST_TEST(tree.contains(5));
  BOOST_TEST(tree.contains(10));
}

BOOST_AUTO_TEST_CASE(rotate_root_test)
{
  Tree tree = makeTree({{10, "ten"}, {5, "five"}, {15, "fifteen"}});

  auto res = tree.rotateLeft(find(tree, 10));

  BOOST_TEST(res->first == 15);
  BOOST_TEST(tree.size() == 3u);
  BOOST_TEST(tree.contains(5));
  BOOST_TEST(tree.contains(10));
  BOOST_TEST(tree.contains(15));
}

