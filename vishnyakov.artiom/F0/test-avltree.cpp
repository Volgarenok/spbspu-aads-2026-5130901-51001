#include <boost/test/unit_test.hpp>
#include "avltree.hpp"

namespace vishnyakov
{
  BOOST_AUTO_TEST_SUITE(AVLTreeTests)

  BOOST_AUTO_TEST_CASE(ConstructorDefault)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    BOOST_TEST(tree.empty());
    BOOST_TEST(tree.size() == 0);
  }

  BOOST_AUTO_TEST_CASE(PushAndHas)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(1, "one");
    tree.push(2, "two");
    tree.push(3, "three");

    BOOST_TEST(tree.size() == 3);
    BOOST_TEST(!tree.empty());
    BOOST_TEST(tree.has(1));
    BOOST_TEST(tree.has(2));
    BOOST_TEST(tree.has(3));
    BOOST_TEST(!tree.has(4));
  }

  BOOST_AUTO_TEST_CASE(PushDuplicate)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(1, "one");
    tree.push(1, "one_again");

    BOOST_TEST(tree.size() == 1);
    BOOST_TEST(tree.has(1));
  }

  BOOST_AUTO_TEST_CASE(CopyConstructor)
  {
    AVLTree< int, std::string, std::less< int > > tree1;
    tree1.push(1, "one");
    tree1.push(2, "two");

    AVLTree< int, std::string, std::less< int > > tree2(tree1);
    BOOST_TEST(tree2.size() == 2);
    BOOST_TEST(tree2.has(1));
    BOOST_TEST(tree2.has(2));
  }

  BOOST_AUTO_TEST_CASE(MoveConstructor)
  {
    AVLTree< int, std::string, std::less< int > > tree1;
    tree1.push(1, "one");
    tree1.push(2, "two");

    AVLTree< int, std::string, std::less< int > > tree2(std::move(tree1));
    BOOST_TEST(tree2.size() == 2);
    BOOST_TEST(tree2.has(1));
    BOOST_TEST(tree2.has(2));
    BOOST_TEST(tree1.empty());
  }

  BOOST_AUTO_TEST_CASE(CopyAssignment)
  {
    AVLTree< int, std::string, std::less< int > > tree1;
    tree1.push(1, "one");
    tree1.push(2, "two");

    AVLTree< int, std::string, std::less< int > > tree2;
    tree2 = tree1;
    BOOST_TEST(tree2.size() == 2);
    BOOST_TEST(tree2.has(1));
    BOOST_TEST(tree2.has(2));
  }

  BOOST_AUTO_TEST_CASE(MoveAssignment)
  {
    AVLTree< int, std::string, std::less< int > > tree1;
    tree1.push(1, "one");
    tree1.push(2, "two");

    AVLTree< int, std::string, std::less< int > > tree2;
    tree2 = std::move(tree1);
    BOOST_TEST(tree2.size() == 2);
    BOOST_TEST(tree2.has(1));
    BOOST_TEST(tree2.has(2));
    BOOST_TEST(tree1.empty());
  }

  BOOST_AUTO_TEST_CASE(Clear)
  {
    AVLTree< int, std::string, std::less< int > > tree;
    tree.push(1, "one");
    tree.push(2, "two");
    tree.push(3, "three");

    BOOST_TEST(tree.size() == 3);
    tree.clear();
    BOOST_TEST(tree.empty());
    BOOST_TEST(tree.size() == 0);
    BOOST_TEST(!tree.has(1));
  }

  BOOST_AUTO_TEST_SUITE_END()
}

