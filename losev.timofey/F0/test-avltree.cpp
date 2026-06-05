#define BOOST_TEST_MODULE AVLTreeTests
#include <boost/test/included/unit_test.hpp>
#include <string>
#include <stdexcept>

#include "avltree.hpp"

BOOST_AUTO_TEST_SUITE(avltree_tests)

BOOST_AUTO_TEST_CASE(empty_tree_test)
{
  losev::AVLTree<int, std::string> tree;
  BOOST_TEST(tree.empty() == true);
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(push_and_get_test)
{
  losev::AVLTree<int, std::string> tree;
  tree.push(5, "five");
  BOOST_TEST(tree.empty() == false);
  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(5) == "five");
}

BOOST_AUTO_TEST_CASE(avl_balance_after_sequential_inserts)
{
  losev::AVLTree<int, std::string> tree;
  for (int i = 1; i <= 10; ++i)
  {
    tree.push(i, std::to_string(i));
  }
  // AVL должен сохранить высоту ~ log2(10) = 4
  size_t h = tree.height();
  BOOST_TEST(h <= 5);
  BOOST_TEST(h >= 3);
  // Все элементы должны быть доступны
  for (int i = 1; i <= 10; ++i)
  {
    BOOST_TEST(tree.get(i) == std::to_string(i));
  }
}

BOOST_AUTO_TEST_CASE(avl_balance_after_descending_inserts)
{
  losev::AVLTree<int, std::string> tree;

  // Вставка в порядке убывания
  for (int i = 10; i >= 1; --i)
  {
    tree.push(i, std::to_string(i));
  }

  size_t h = tree.height();
  BOOST_TEST(h <= 5);

  for (int i = 1; i <= 10; ++i)
  {
    BOOST_TEST(tree.get(i) == std::to_string(i));
  }
}

BOOST_AUTO_TEST_CASE(delete_rebalances_tree)
{
  losev::AVLTree<int, std::string> tree;

  for (int i = 1; i <= 10; ++i)
  {
    tree.push(i, std::to_string(i));
  }

  tree.drop(5);
  tree.drop(3);
  tree.drop(7);

  BOOST_TEST(tree.size() == 7);
  BOOST_TEST(tree.height() <= 4);

  for (int i = 1; i <= 10; ++i)
  {
    if (i != 5 && i != 3 && i != 7)
    {
      BOOST_TEST(tree.get(i) == std::to_string(i));
    }
  }

  BOOST_CHECK_THROW(tree.get(5), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(update_existing_key_test)
{
  losev::AVLTree<int, std::string> tree;

  tree.push(5, "five");
  tree.push(5, "cinq");

  BOOST_TEST(tree.size() == 1);
  BOOST_TEST(tree.get(5) == "cinq");
}

BOOST_AUTO_TEST_CASE(iterator_test)
{
  losev::AVLTree<int, std::string> tree;

  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(1, "one");
  tree.push(9, "nine");

  int expected[] = {1, 3, 5, 7, 9};
  int index = 0;

  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    auto pair = *it;
    BOOST_TEST(pair.first == expected[index]);
    ++index;
  }

  BOOST_TEST(index == 5);
}

BOOST_AUTO_TEST_CASE(const_iterator_test)
{
  losev::AVLTree<int, std::string> tree;

  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");

  const auto& constTree = tree;
  int expected[] = {3, 5, 7};
  int index = 0;

  for (auto it = constTree.begin(); it != constTree.end(); ++it)
  {
    auto pair = *it;
    BOOST_TEST(pair.first == expected[index]);
    ++index;
  }
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  losev::AVLTree<int, std::string> tree1;

  tree1.push(5, "five");
  tree1.push(3, "three");
  tree1.push(7, "seven");

  losev::AVLTree<int, std::string> tree2(tree1);

  BOOST_TEST(tree2.size() == 3);
  BOOST_TEST(tree2.get(5) == "five");
  BOOST_TEST(tree2.get(3) == "three");
  BOOST_TEST(tree2.get(7) == "seven");
}

BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  losev::AVLTree<int, std::string> tree1;

  tree1.push(5, "five");
  tree1.push(3, "three");

  losev::AVLTree<int, std::string> tree2(std::move(tree1));

  BOOST_TEST(tree2.size() == 2);
  BOOST_TEST(tree2.get(5) == "five");
  BOOST_TEST(tree2.get(3) == "three");
  BOOST_TEST(tree1.empty() == true);
}

BOOST_AUTO_TEST_SUITE_END()
