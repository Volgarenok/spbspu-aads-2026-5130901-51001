#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "bstree.hpp"

namespace
{
  struct TestFailure: public std::runtime_error
  {
    explicit TestFailure(const std::string& message):
      std::runtime_error(message)
    {
    }
  };

  void require(bool condition, const std::string& message)
  {
    if (!condition)
    {
      throw TestFailure(message);
    }
  }

  void testEmptyTree()
  {
    const alekseev::BSTree< int, std::string > tree;
    require(tree.empty(), "new tree is empty");
    require(tree.size() == 0, "new tree size is zero");
    require(tree.begin() == tree.end(), "empty begin equals end");
    require(tree.height() == 0, "empty height is zero");
  }

  template< class Tree >
  std::string keys(const Tree& tree)
  {
    std::string result;
    for (typename Tree::const_iterator it = tree.cbegin(); it != tree.cend(); ++it)
    {
      result += static_cast< char >('0' + it->first);
    }
    return result;
  }

  void testInsertSearchAndIterators()
  {
    alekseev::BSTree< int, std::string > tree;
    tree.push(2, "two");
    require(!tree.empty(), "insert makes tree nonempty");
    require(tree.size() == 1, "single insert size");
    require(tree.get(2) == "two", "get inserted key");
    tree.push(1, "one");
    tree.push(3, "three");
    require(keys(tree) == "123", "in order traversal");
    tree.push(2, "changed");
    require(tree.size() == 3, "replace keeps size");
    require(tree.get(2) == "changed", "replace existing key");
    require(tree.contains(1) && !tree.contains(4), "contains works");

    bool thrown = false;
    try
    {
      tree.get(4);
    }
    catch (const std::out_of_range&)
    {
      thrown = true;
    }
    require(thrown, "get missing throws");

    alekseev::BSTree< int, std::string >::iterator it = tree.begin();
    require(it->first == 1, "begin is minimum");
    require((++it)->first == 2, "prefix increment");
    require((it++)->first == 2, "postfix increment");
    require(it->first == 3, "postfix changes iterator");
    it = tree.end();
    --it;
    require(it->first == 3, "decrement end returns maximum");

    const alekseev::BSTree< int, std::string >& constTree = tree;
    alekseev::BSTree< int, std::string >::const_iterator cit = constTree.cbegin();
    require(cit->first == 1 && cit->second == "one", "const iterator reads data");
  }

  void testCopyMoveAndClear()
  {
    alekseev::BSTree< int, std::string > tree;
    tree.push(2, "two");
    tree.push(1, "one");
    tree.push(3, "three");

    alekseev::BSTree< int, std::string > copy(tree);
    copy.push(4, "four");
    require(keys(tree) == "123", "copy constructor independent source");
    require(keys(copy) == "1234", "copy constructor has copied data");

    alekseev::BSTree< int, std::string > assigned;
    assigned = tree;
    assigned.drop(1);
    require(keys(tree) == "123", "copy assignment independent source");
    require(keys(assigned) == "23", "copy assignment copied data");

    alekseev::BSTree< int, std::string > moved(std::move(copy));
    require(copy.empty() && copy.begin() == copy.end(), "move constructor empties source");
    require(keys(moved) == "1234", "move constructor keeps data");

    alekseev::BSTree< int, std::string > moveAssigned;
    moveAssigned = std::move(moved);
    require(moved.empty() && moved.begin() == moved.end(), "move assignment empties source");
    require(keys(moveAssigned) == "1234", "move assignment keeps data");

    moveAssigned.clear();
    require(moveAssigned.empty() && moveAssigned.size() == 0, "clear removes nodes");

    alekseev::BSTree< int, std::string > left;
    left.push(1, "one");
    alekseev::BSTree< int, std::string > right;
    right.push(2, "two");
    right.push(3, "three");
    left.swap(right);
    require(keys(left) == "23", "swap moves right tree into left");
    require(keys(right) == "1", "swap moves left tree into right");
  }

  void testDropAndHeight()
  {
    alekseev::BSTree< int, std::string > tree;
    tree.push(4, "four");
    tree.push(2, "two");
    tree.push(6, "six");
    tree.push(1, "one");
    tree.push(3, "three");
    tree.push(5, "five");
    tree.push(7, "seven");
    require(tree.height() == 3, "balanced height");
    require(tree.drop(1) == "one", "drop leaf returns value");
    require(keys(tree) == "234567", "drop leaf keeps order");
    tree.push(1, "one");
    tree.drop(2);
    require(keys(tree) == "134567", "drop node with two children keeps order");
    tree.drop(6);
    require(keys(tree) == "13457", "drop another two child node keeps order");
    tree.drop(7);
    require(keys(tree) == "1345", "drop leaf after deletion");
    tree.drop(5);
    require(keys(tree) == "134", "drop one child setup");
    tree.drop(4);
    require(keys(tree) == "13", "drop real root through fake root");

    alekseev::BSTree< int, std::string > one;
    one.push(10, "ten");
    require(one.height() == 1, "one node height");
    one.push(9, "nine");
    one.push(8, "eight");
    require(one.height(one.begin()) == 1, "subtree height for leaf");
    require(one.height(one.end()) == 0, "end subtree height");
  }

  void testRotations()
  {
    alekseev::BSTree< int, std::string > leftTree;
    leftTree.push(2, "two");
    leftTree.push(1, "one");
    leftTree.push(4, "four");
    leftTree.push(3, "three");
    alekseev::BSTree< int, std::string >::iterator stable = leftTree.begin();
    ++stable;
    alekseev::BSTree< int, std::string >::iterator raised = leftTree.rotateLeft(--leftTree.end());
    require(raised->first == 4, "rotate left returns raised node");
    require(keys(leftTree) == "1234", "rotate left keeps order");
    require(stable->first == 2 && stable->second == "two", "left rotation keeps iterator stable");

    alekseev::BSTree< int, std::string > rightTree;
    rightTree.push(3, "three");
    rightTree.push(1, "one");
    rightTree.push(4, "four");
    rightTree.push(2, "two");
    raised = rightTree.rotateRight(rightTree.begin());
    require(raised->first == 1, "rotate right returns raised node");
    require(keys(rightTree) == "1234", "rotate right keeps order");

    alekseev::BSTree< int, std::string > largeLeft;
    largeLeft.push(1, "one");
    largeLeft.push(4, "four");
    largeLeft.push(2, "two");
    largeLeft.push(3, "three");
    alekseev::BSTree< int, std::string >::iterator inner = largeLeft.begin();
    ++inner;
    raised = largeLeft.rotateLargeLeft(inner);
    require(raised->first == 2, "large left returns top node");
    require(keys(largeLeft) == "1234", "large left keeps order");

    alekseev::BSTree< int, std::string > largeRight;
    largeRight.push(4, "four");
    largeRight.push(1, "one");
    largeRight.push(3, "three");
    largeRight.push(2, "two");
    inner = largeRight.begin();
    ++inner;
    ++inner;
    raised = largeRight.rotateLargeRight(inner);
    require(raised->first == 3, "large right returns top node");
    require(keys(largeRight) == "1234", "large right keeps order");

    alekseev::BSTree< int, std::string > rootRotate;
    rootRotate.push(1, "one");
    rootRotate.push(2, "two");
    rootRotate.rotateLeft(--rootRotate.end());
    require(keys(rootRotate) == "12", "root rotation keeps order");
    require(rootRotate.height() == 2, "root rotation keeps fake root valid");

    bool thrown = false;
    try
    {
      rootRotate.rotateLeft(rootRotate.begin());
    }
    catch (const std::logic_error&)
    {
      thrown = true;
    }
    require(thrown, "invalid rotation throws");
  }
}

int runBSTreeTests()
{
  try
  {
    testEmptyTree();
    testInsertSearchAndIterators();
    testCopyMoveAndClear();
    testDropAndHeight();
    testRotations();
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "bstree tests failed: " << e.what() << '\n';
    return 1;
  }
}
