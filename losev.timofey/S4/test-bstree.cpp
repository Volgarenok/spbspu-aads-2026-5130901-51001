#define BOOST_TEST_MODULE BSTreeTests
#include <boost/test/included/unit_test.hpp>
#include "bstree.hpp"

BOOST_AUTO_TEST_SUITE(bstree_tests)

BOOST_AUTO_TEST_CASE(empty_tree_test)
{
  losev::BSTree<int, std::string> tree;
  BOOST_TEST(tree.empty() == true);
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(push_and_get_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(5, "five");
  BOOST_TEST(tree.empty() == false);
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(5) == "five");
}

BOOST_AUTO_TEST_CASE(push_multiple_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree.get(10) == "ten");
  BOOST_TEST(tree.get(5) == "five");
  BOOST_TEST(tree.get(15) == "fifteen");
}

BOOST_AUTO_TEST_CASE(overwrite_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(5, "cinq");
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(5) == "cinq");
}

BOOST_AUTO_TEST_CASE(drop_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  std::string val = tree.drop(5);
  BOOST_TEST(val == "five");
  BOOST_TEST(tree.size() == 2);
}

BOOST_AUTO_TEST_CASE(drop_root_test)
{
  losev::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  std::string val = tree.drop(10);
  BOOST_TEST(val == "ten");
  BOOST_TEST(tree.size() == 2);
}

BOOST_AUTO_TEST_CASE(height_test)
{
  losev::BSTree<int, std::string> tree;
  BOOST_TEST(tree.height() == 0);

  tree.push(10, "ten");
  BOOST_TEST(tree.height() == 1);

  tree.push(5, "five");
  BOOST_TEST(tree.height() == 2);

  tree.push(15, "fifteen");
  BOOST_TEST(tree.height() == 2);

  tree.push(3, "three");
  BOOST_TEST(tree.height() == 3);
}

BOOST_AUTO_TEST_SUITE_END()
