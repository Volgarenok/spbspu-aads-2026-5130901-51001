#include "list.hpp"
#ifdef UNIT_TEST
int main()
{
  loseva::List<int> list;

  if (!list.empty()) {
    return 1;
  }

  list.pushBack(1);
  list.pushBack(2);

  if (list.size() != 2) {
    return 1;
  }

  if (list.front() != 1) {
    return 1;
  }

  if (list.back() != 2) {
    return 1;
  }

  list.popFront();

  if (list.front() != 2) {
    return 1;
  }

  list.popBack();

  if (!list.empty()) {
    return 1;
  }

  return 0;
}
