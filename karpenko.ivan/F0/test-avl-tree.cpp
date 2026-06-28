#include <sstream>
#include <stdexcept>

#include <boost/test/unit_test.hpp>

#include "../common/avl_tree.hpp"
#include "../common/list.hpp"

template< typename Tree >
std::string keys(const Tree& tree)
{
  std::ostringstream oss;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    oss << it->first << " ";
  }
  return oss.str();
}

BOOST_AUTO_TEST_CASE(insert_and_find)
{
  karpenko::AVLTree< int, std::string > tree;
  tree.insert(2, "two");
  tree.insert(1, "one");
  tree.insert(3, "three");

  BOOST_TEST(tree.contains(1));
  BOOST_TEST(tree.contains(2));
  BOOST_TEST(tree.contains(3));
  BOOST_TEST(!tree.contains(4));

  BOOST_TEST(tree.at(1) == "one");
  BOOST_TEST(tree.at(2) == "two");
  BOOST_TEST(tree.at(3) == "three");
  BOOST_CHECK_THROW(tree.at(4), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(update_value)
{
  karpenko::AVLTree< int, std::string > tree;
  tree.insert(1, "first");
  tree.insert(1, "updated");
  BOOST_TEST(tree.at(1) == "updated");
  BOOST_TEST(tree.size() == 1);
}

BOOST_AUTO_TEST_CASE(erase)
{
  karpenko::AVLTree< int, std::string > tree;
  tree.insert(5, "five");
  tree.insert(3, "three");
  tree.insert(7, "seven");
  tree.insert(2, "two");
  tree.insert(4, "four");
  tree.insert(6, "six");
  tree.insert(8, "eight");

  BOOST_TEST(tree.size() == 7);
  tree.erase(5);
  BOOST_TEST(!tree.contains(5));
  BOOST_TEST(tree.size() == 6);

  tree.erase(2);
  tree.erase(4);
  tree.erase(6);
  tree.erase(8);
  tree.erase(3);
  tree.erase(7);
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(size_and_empty)
{
  karpenko::AVLTree< int, int > tree;
  BOOST_TEST(tree.empty());
  tree.insert(10, 100);
  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 1);
  tree.erase(10);
  BOOST_TEST(tree.empty());
}

BOOST_AUTO_TEST_CASE(const_iterator_inorder)
{
  karpenko::AVLTree< int, char > tree;
  tree.insert(2, 'b');
  tree.insert(1, 'a');
  tree.insert(4, 'd');
  tree.insert(3, 'c');
  tree.insert(5, 'e');

  std::string inorder;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    inorder.push_back(it->second);
  }
  BOOST_TEST(inorder == "abcde");
}

BOOST_AUTO_TEST_CASE(const_iterator_key_order)
{
  karpenko::AVLTree< int, int > tree;
  tree.insert(50, 500);
  tree.insert(30, 300);
  tree.insert(70, 700);
  tree.insert(20, 200);
  tree.insert(40, 400);
  tree.insert(60, 600);
  tree.insert(80, 800);

  karpenko::List< int > keys_in_order;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys_in_order.pushBack(it->first);
  }
  auto it = keys_in_order.begin();
  BOOST_TEST(*it == 20); ++it;
  BOOST_TEST(*it == 30); ++it;
  BOOST_TEST(*it == 40); ++it;
  BOOST_TEST(*it == 50); ++it;
  BOOST_TEST(*it == 60); ++it;
  BOOST_TEST(*it == 70); ++it;
  BOOST_TEST(*it == 80);
}

BOOST_AUTO_TEST_CASE(iterator_operations)
{
  karpenko::AVLTree< int, int > tree;
  tree.insert(2, 2);
  tree.insert(1, 1);
  tree.insert(3, 3);

  auto it = tree.begin();
  BOOST_TEST(it->first == 1);
  ++it;
  BOOST_TEST(it->first == 2);
  it++;
  BOOST_TEST(it->first == 3);
  ++it;
  BOOST_CHECK_EQUAL(it == tree.end(), true);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  karpenko::AVLTree< int, int > original;
  original.insert(1, 10);
  original.insert(2, 20);
  original.insert(3, 30);

  karpenko::AVLTree< int, int > copy = original;
  BOOST_TEST(copy.contains(1));
  BOOST_TEST(copy.contains(2));
  BOOST_TEST(copy.contains(3));
  BOOST_TEST(copy.at(2) == 20);
  BOOST_TEST(copy.size() == 3);

  original.erase(2);
  BOOST_TEST(original.size() == 2);
  BOOST_TEST(copy.size() == 3);
  BOOST_TEST(copy.contains(2));
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  karpenko::AVLTree< int, int > original;
  original.insert(1, 100);
  original.insert(2, 200);

  karpenko::AVLTree< int, int > moved = std::move(original);
  BOOST_TEST(moved.contains(1));
  BOOST_TEST(moved.contains(2));
  BOOST_TEST(moved.at(1) == 100);
  BOOST_TEST(original.empty());
  BOOST_TEST(original.size() == 0);
}

BOOST_AUTO_TEST_CASE(assignment_operator)
{
  karpenko::AVLTree< int, int > a, b;
  a.insert(1, 10);
  a.insert(2, 20);
  b = a;
  BOOST_TEST(b.contains(1));
  BOOST_TEST(b.contains(2));
  BOOST_TEST(b.at(1) == 10);
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  karpenko::AVLTree< int, int > a, b;
  a.insert(1, 1);
  a.insert(2, 2);
  b = std::move(a);
  BOOST_TEST(b.contains(1));
  BOOST_TEST(b.contains(2));
  BOOST_TEST(a.empty());
}

BOOST_AUTO_TEST_CASE(height_after_insertions)
{
  karpenko::AVLTree< int, int > tree;
  for (int i = 1; i <= 100; ++i)
  {
    tree.insert(i, i);
  }
  BOOST_TEST(tree.getHeight() <= 8);
  BOOST_TEST(tree.getHeight() >= 7);
}

BOOST_AUTO_TEST_CASE(complex_insert_erase_sequence)
{
  karpenko::AVLTree< int, int > tree;
  for (int i = 1; i <= 20; ++i)
  {
    tree.insert(i, i);
  }
  for (int i = 1; i <= 20; i += 2)
  {
    tree.erase(i);
  }
  for (int i = 2; i <= 20; i += 2)
  {
    BOOST_TEST(tree.contains(i));
  }
  BOOST_TEST(!tree.contains(1));
  BOOST_TEST(tree.size() == 10);
}
