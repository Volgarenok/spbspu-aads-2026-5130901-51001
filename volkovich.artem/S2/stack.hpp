#include "../common/list.hpp"

namespace volkovich
{
  template <typename T>
  class Stack
  {
  List<T> data;

  public:
    bool isEmpty();
    size_t length();
    void push(T &&v);
    void push(const T &v);
    void pop();
  };
}
