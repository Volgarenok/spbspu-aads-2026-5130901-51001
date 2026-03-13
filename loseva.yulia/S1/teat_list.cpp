#include <cassert>
#include "../include/list.hpp"

using namespace loseva;

int main()
{
  List<int> list;

  assert(list.empty());

  list.push_front(1);
  list.push_front(2);
  list.push_front(3);

  assert(list.front() == 3);

  list.pop_front();

  assert(list.front() == 2);

  int sum = 0;

  for (auto it = list.begin(); it != list.end(); ++it)
  {
    sum += *it;
  }

  assert(sum == 3);

  list.clear();

  assert(list.empty());
}
