#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include "bstree.hpp"

BOOST_AUTO_TEST_CASE(insert_test)
{
  yarmolinskaya::BSTree< int, int > tree;

  tree.insert(5, 10);
  tree.insert(2, 20);
  tree.insert(8, 30);

  BOOST_TEST(tree.contains(5));
  BOOST_TEST(tree.contains(2));
  BOOST_TEST(tree.contains(8));
}

BOOST_AUTO_TEST_CASE(height_test)
{
  yarmolinskaya::BSTree< int, int > tree;

  tree.insert(5, 1);
  tree.insert(3, 1);
  tree.insert(1, 1);

  BOOST_TEST(tree.height() == 3);
}

BOOST_AUTO_TEST_CASE(left_rotate_test)
{
  yarmolinskaya::BSTree< int, int > tree;

  tree.insert(10, 1);
  tree.insert(20, 2);

  auto it = tree.find(10);

  tree.rotateLeft(it);

  BOOST_TEST(tree.begin()->first == 10);
}

BOOST_AUTO_TEST_CASE(right_rotate_test)
{
  yarmolinskaya::BSTree< int, int > tree;

  tree.insert(10, 1);
  tree.insert(5, 2);

  auto it = tree.find(10);

  tree.rotateRight(it);

  BOOST_TEST(tree.begin()->first == 5);
}
