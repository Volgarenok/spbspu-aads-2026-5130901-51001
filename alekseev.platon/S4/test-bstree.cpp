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
  }
}

int runBSTreeTests()
{
  try
  {
    testEmptyTree();
    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "bstree tests failed: " << e.what() << '\n';
    return 1;
  }
}
