#include <cstddef>

namespace karpenko
{
  namespace detail
  {
    struct NodeBase
    {
      NodeBase* next;
      explicit NodeBase(NodeBase* nxt = nullptr) noexcept : next(nxt) {}
    };

    template< typename T >
    struct Node final : NodeBase
    {
      T data;

      explicit Node(NodeBase* nxt, const T& value) : NodeBase(nxt), data(value) {}
      explicit Node(NodeBase* nxt, T&& value) : NodeBase(nxt), data(std::move(value)) {}
    };
  }
}

int main()
{

}
