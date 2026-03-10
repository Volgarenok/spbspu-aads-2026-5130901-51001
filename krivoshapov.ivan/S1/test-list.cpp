#include <iostream>
#include <string>
#include <limits>

#include "list.hpp"
namespace krivoshapov
{
  static int passed = 0;
  static int failed = 0;

  static void check(bool cond, const char *description)
  {
    if (cond)
    {
      ++passed;
    }
    else
    {
      ++failed;
      std::cerr << "FAIL: " << description << '\n';
    }
  }

  static void testEmptyList()
  {
    List<int> lst;
    check(lst.empty(), "empty list: empty() == true");
    check(lst.size() == 0, "empty list: size() == 0");
    check(lst.begin() == lst.end(), "empty list: begin() == end()");
    check(lst.cbegin() == lst.cend(), "empty list: cbegin() == cend()");
  }
}
