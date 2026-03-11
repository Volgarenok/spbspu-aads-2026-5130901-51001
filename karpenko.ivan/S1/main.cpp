#include <cstddef>
#include <utility>

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

  template< typename T > class List;
  template< typename T > class LIter;
  template< typename T > class LCIter;

  template< typename T >
  class LIter
  {
  public:
    LIter() noexcept : ptr_(nullptr) {}
    explicit LIter(detail::NodeBase* p) noexcept : ptr_(p) {}

    T& operator*() const noexcept
    {
      detail::Node< T >* node_ptr = static_cast< detail::Node< T >* >(ptr_);
      return node_ptr->data;
    }

    T* operator->() const noexcept
    {
      detail::Node< T >* node_ptr = static_cast< detail::Node< T >* >(ptr_);
      return &(node_ptr->data);
    }

    LIter& operator++() noexcept
    {
      ptr_ = ptr_->next;
      return *this;
    }

    LIter operator++(int) noexcept
    {
      LIter tmp = *this;
      ptr_ = ptr_->next;
      return tmp;
    }

    bool operator==(const LIter& other) const noexcept
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LIter& other) const noexcept
    {
      return !(*this == other);
    }

    detail::NodeBase* get_ptr() const noexcept
    {
      return ptr_;
    }

  private:
    detail::NodeBase* ptr_;
    friend class List< T >;
    friend class LCIter< T >;
  };

  template< typename T >
  class LCIter
  {
  public:
    LCIter() noexcept : ptr_(nullptr) {}
    explicit LCIter(const detail::NodeBase* p) noexcept : ptr_(p) {}
    LCIter(const LIter< T >& it) noexcept : ptr_(it.get_ptr()) {}

    const T& operator*() const noexcept
    {
      const detail::Node< T >* node_ptr = static_cast< const detail::Node< T >* >(ptr_);
      return node_ptr->data;
    }

    const T* operator->() const noexcept
    {
      const detail::Node< T >* node_ptr = static_cast< const detail::Node< T >* >(ptr_);
      return &(node_ptr->data);
    }

    LCIter& operator++() noexcept
    {
      ptr_ = ptr_->next;
      return *this;
    }

    LCIter operator++(int) noexcept
    {
      LCIter tmp = *this;
      ptr_ = ptr_->next;
      return tmp;
    }

    bool operator==(const LCIter& other) const noexcept
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LCIter& other) const noexcept
    {
      return !(*this == other);
    }

  private:
    const detail::NodeBase* ptr_;
    friend class List< T >;
  };
}

int main()
{

}
