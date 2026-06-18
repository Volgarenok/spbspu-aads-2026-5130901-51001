#define BOOST_TEST_MODULE S4_BSTree
#include <boost/test/included/unit_test.hpp>

#include "bstree.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace shevchenko
{

namespace
{
using Tree = BSTree< int, std::string, std::less< int > >;

std::vector< int > getKeys(const Tree& tree)
{
  std::vector< int > keys;
  for (auto it = tree.cbegin(); it != tree.cend(); ++it)
  {
    keys.push_back(it->first);
  }
  return keys;
}

Tree makeTestTree()
{
  Tree tree;
  tree.push(4, "four");
  tree.push(2, "two");
  tree.push(6, "six");
  tree.push(1, "one");
  tree.push(3, "three");
  tree.push(5, "five");
  tree.push(7, "seven");
  return tree;
}

Tree makeFirstDict()
{
  Tree dict;
  dict.push(1, "name");
  dict.push(2, "surname");
  return dict;
}

Tree makeSecondDict()
{
  Tree dict;
  dict.push(4, "mouse");
  dict.push(1, "name");
  dict.push(2, "keyboard");
  return dict;
}
}

BOOST_AUTO_TEST_CASE(new_tree_is_empty)
{
  Tree tree;
  BOOST_CHECK(tree.empty());
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(add_one_element)
{
  Tree tree;
  tree.push(2, "two");

  BOOST_CHECK(!tree.empty());
  BOOST_TEST(tree.size() == 1);
  BOOST_CHECK(tree.has(2));
  BOOST_TEST(tree.get(2) == "two");
}

BOOST_AUTO_TEST_CASE(add_several_elements)
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

BOOST_AUTO_TEST_CASE(add_existing_key_updates_value)
{
  Tree tree;
  tree.push(1, "one");
  tree.push(1, "uno");

  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(1) == "uno");
}

BOOST_AUTO_TEST_CASE(get_returns_correct_value)
{
  Tree tree;
  tree.push(1, "one");
  tree.push(2, "two");

  BOOST_TEST(tree.get(1) == "one");
  BOOST_TEST(tree.get(2) == "two");
}

BOOST_AUTO_TEST_CASE(get_throws_if_key_not_found)
{
  Tree tree;
  tree.push(1, "one");
  BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(const_get_works_on_const_tree)
{
  Tree tree;
  tree.push(1, "one");
  const Tree& constTree = tree;

  BOOST_TEST(constTree.get(1) == "one");
  BOOST_CHECK_THROW(constTree.get(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(has_returns_true_if_key_exists)
{
  Tree tree;
  tree.push(1, "one");
  BOOST_CHECK(tree.has(1));
  BOOST_CHECK(!tree.has(2));
}

BOOST_AUTO_TEST_CASE(clear_removes_all_elements)
{
  Tree tree = makeTestTree();
  tree.clear();

  BOOST_CHECK(tree.empty());
  BOOST_TEST(tree.size() == 0);
  BOOST_CHECK(!tree.has(1));
}

BOOST_AUTO_TEST_CASE(empty_tree_iterators_are_equal)
{
  Tree tree;
  BOOST_CHECK(tree.begin() == tree.end());
  BOOST_CHECK(tree.cbegin() == tree.cend());
}

BOOST_AUTO_TEST_CASE(iterator_goes_through_one_element)
{
  Tree tree;
  tree.push(2, "two");

  auto it = tree.begin();
  BOOST_REQUIRE(it != tree.end());
  BOOST_TEST(it->first == 2);
  BOOST_TEST(it->second == "two");
  ++it;
  BOOST_CHECK(it == tree.end());
}

BOOST_AUTO_TEST_CASE(iterator_visits_keys_in_sorted_order)
{
  Tree tree;
  tree.push(4, "four");
  tree.push(2, "two");
  tree.push(6, "six");
  tree.push(1, "one");
  tree.push(3, "three");

  std::vector< int > keys = getKeys(tree);
  std::vector< int > expected{1, 2, 3, 4, 6};
  BOOST_TEST(keys == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(const_iterator_visits_keys_in_sorted_order)
{
  Tree tree = makeTestTree();
  const Tree& constTree = tree;

  std::vector< int > keys;
  for (auto it = constTree.cbegin(); it != constTree.cend(); ++it)
  {
    keys.push_back(it->first);
  }

  std::vector< int > expected{1, 2, 3, 4, 5, 6, 7};
  BOOST_TEST(keys == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_CASE(copy_constructor_makes_deep_copy)
{
  Tree original = makeTestTree();
  Tree copy(original);

  BOOST_TEST(copy.size() == original.size());
  BOOST_TEST(copy.get(4) == "four");
  BOOST_TEST(copy.get(2) == "two");

  copy.push(8, "eight");
  BOOST_TEST(copy.size() == 8);
  BOOST_TEST(original.size() == 7);
  BOOST_CHECK(!original.has(8));
}

BOOST_AUTO_TEST_CASE(move_constructor_transfers_data)
{
  Tree original = makeTestTree();
  size_t originalSize = original.size();

  Tree moved(std::move(original));

  BOOST_TEST(moved.size() == originalSize);
  BOOST_TEST(moved.get(4) == "four");
  BOOST_CHECK(original.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment_makes_deep_copy)
{
  Tree original = makeTestTree();
  Tree copy;
  copy.push(100, "hundred");

  copy = original;

  BOOST_TEST(copy.size() == original.size());
  BOOST_TEST(copy.get(4) == "four");
  BOOST_CHECK(!copy.has(100));
}

BOOST_AUTO_TEST_CASE(move_assignment_transfers_data)
{
  Tree original = makeTestTree();
  Tree moved;
  moved.push(100, "hundred");

  moved = std::move(original);

  BOOST_TEST(moved.size() == 7);
  BOOST_TEST(moved.get(4) == "four");
  BOOST_CHECK(original.empty());
}

BOOST_AUTO_TEST_CASE(print_second_dictionary)
{
  Tree second = makeSecondDict();

  std::ostringstream output;
  output << "second";
  for (auto it = second.cbegin(); it != second.cend(); ++it)
  {
    output << " " << it->first << " " << it->second;
  }

  BOOST_TEST(output.str() == "second 1 name 2 keyboard 4 mouse");
}

BOOST_AUTO_TEST_CASE(complement_second_minus_first)
{
  Tree second = makeSecondDict();
  Tree first = makeFirstDict();

  Tree result;
  for (auto it = second.cbegin(); it != second.cend(); ++it)
  {
    try
    {
      first.get(it->first);
    }
    catch (const std::out_of_range&)
    {
      result.push(it->first, it->second);
    }
  }

  BOOST_TEST(result.size() == 1);
  BOOST_CHECK(result.has(4));
  BOOST_TEST(result.get(4) == "mouse");
}

BOOST_AUTO_TEST_CASE(intersect_first_and_second)
{
  Tree first = makeFirstDict();
  Tree second = makeSecondDict();

  Tree result;
  for (auto it = first.cbegin(); it != first.cend(); ++it)
  {
    try
    {
      second.get(it->first);
      result.push(it->first, it->second);
    }
    catch (const std::out_of_range&)
    {
    }
  }

  BOOST_TEST(result.size() == 2);
  BOOST_CHECK(result.has(1));
  BOOST_CHECK(result.has(2));
  BOOST_TEST(result.get(1) == "name");
  BOOST_TEST(result.get(2) == "surname");
}

BOOST_AUTO_TEST_CASE(union_first_and_second)
{
  Tree first = makeFirstDict();
  Tree second = makeSecondDict();

  Tree result;
  for (auto it = first.cbegin(); it != first.cend(); ++it)
  {
    result.push(it->first, it->second);
  }
  for (auto it = second.cbegin(); it != second.cend(); ++it)
  {
    try
    {
      result.get(it->first);
    }
    catch (const std::out_of_range&)
    {
      result.push(it->first, it->second);
    }
  }

  BOOST_TEST(result.size() == 3);
  BOOST_TEST(result.get(1) == "name");
  BOOST_TEST(result.get(2) == "surname");
  BOOST_TEST(result.get(4) == "mouse");
}

BOOST_AUTO_TEST_CASE(union_second_and_first_left_wins)
{
  Tree second = makeSecondDict();
  Tree first = makeFirstDict();

  Tree result;
  for (auto it = second.cbegin(); it != second.cend(); ++it)
  {
    result.push(it->first, it->second);
  }
  for (auto it = first.cbegin(); it != first.cend(); ++it)
  {
    try
    {
      result.get(it->first);
    }
    catch (const std::out_of_range&)
    {
      result.push(it->first, it->second);
    }
  }

  BOOST_TEST(result.size() == 3);
  BOOST_TEST(result.get(1) == "name");
  BOOST_TEST(result.get(2) == "keyboard");
  BOOST_TEST(result.get(4) == "mouse");
}

BOOST_AUTO_TEST_CASE(empty_dictionary_output)
{
  Tree empty;

  std::ostringstream output;
  if (empty.empty())
  {
    output << "<EMPTY>";
  }

  BOOST_TEST(output.str() == "<EMPTY>");
}

}
