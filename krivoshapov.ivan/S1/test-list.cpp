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
  static void testPushBackAndIterate()
  {
    List<int> lst;
    lst.pushBack(1);
    lst.pushBack(2);
    lst.pushBack(3);

    check(!lst.empty(), "pushBack: not empty");
    check(lst.size() == 3, "pushBack: size == 3");
    check(lst.front() == 1, "pushBack: front == 1");
    check(lst.back() == 3, "pushBack: back == 3");

    auto it = lst.begin();
    check(*it == 1, "iterate: 1st == 1");
    ++it;
    check(*it == 2, "iterate: 2nd == 2");
    ++it;
    check(*it == 3, "iterate: 3rd == 3");
    ++it;
    check(it == lst.end(), "iterate: reaches end");
  }

  static void testPushFrontAndIterate()
  {
    List<int> lst;
    lst.pushFront(3);
    lst.pushFront(2);
    lst.pushFront(1);

    check(lst.front() == 1, "pushFront: front == 1");
    check(lst.back() == 3, "pushFront: back == 3");
    check(lst.size() == 3, "pushFront: size == 3");

    auto it = lst.begin();
    check(*it == 1, "pushFront iterate: 1st == 1");
    ++it;
    check(*it == 2, "pushFront iterate: 2nd == 2");
    ++it;
    check(*it == 3, "pushFront iterate: 3rd == 3");
  }
}
