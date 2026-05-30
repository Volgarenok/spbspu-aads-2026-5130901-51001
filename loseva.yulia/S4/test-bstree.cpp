#define BOOST_TEST_MODULE BSTreeTest
#include <boost/test/included/unit_test.hpp>
#include "bstree.hpp"
#include "commands.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(BSTreeSuite)

BOOST_AUTO_TEST_CASE(PushAndGetTest) {
  loseva::BSTree<int, std::string> tree;
  tree.push(1, "one");
  tree.push(2, "two");

  BOOST_CHECK_EQUAL(tree.has(1), true);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.has(3), false);
  BOOST_CHECK_THROW(tree.get(3), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(DropTest) {
  loseva::BSTree<int, std::string> tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");

  BOOST_CHECK_EQUAL(tree.drop(5), "five");
  BOOST_CHECK_EQUAL(tree.has(5), false);
  BOOST_CHECK_EQUAL(tree.has(10), true);
}

BOOST_AUTO_TEST_CASE(RotationsTest) {
  loseva::BSTree<int, std::string> tree;
  tree.push(10, "root");
  tree.push(5, "left");
  tree.push(15, "right");

  auto it = tree.cbegin();
  ++it;

  auto newRoot = tree.rotateLeft(it);
  BOOST_CHECK_EQUAL(newRoot->first, 15);
}

BOOST_AUTO_TEST_CASE(UnionDsTest) {
  loseva::Dataset d1, d2, res;
  d1.push(1, "a");
  d2.push(2, "b");
  loseva::unionDs(res, d1, d2);
  BOOST_CHECK_EQUAL(res.has(1), true);
  BOOST_CHECK_EQUAL(res.has(2), true);
}

BOOST_AUTO_TEST_SUITE_END()
