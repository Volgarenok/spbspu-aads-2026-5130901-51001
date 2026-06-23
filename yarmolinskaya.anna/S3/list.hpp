#ifndef YARMOLINSKAYA_LIST_HPP
#define YARMOLINSKAYA_LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace yarmolinskaya
{
  template< class T >
  class List;

  template< class T >
  class ConstListIterator;

  template< class T >
  class ListIterator
  {
    friend class List< T >;
    friend class ConstListIterator< T >;

  public:
    using value_type = T;

    using reference = T&;

    using pointer = T*;

    ListIterator():
      node_(nullptr)
    {}

    reference operator*() const
    {
      return node_->data;
    }

    pointer operator->() const
    {
      return std::addressof(
        node_->data
      );
    }

    ListIterator& operator++()
    {
      node_ = node_->next;

      return *this;
    }

    ListIterator operator++(int)
    {
      ListIterator temp(*this);

      ++(*this);

      return temp;
    }

    bool operator==(
      const ListIterator& rhs
    ) const
    {
      return node_ == rhs.node_;
    }

    bool operator!=(
      const ListIterator& rhs
    ) const
    {
      return !(*this == rhs);
    }

  private:
    struct Node
    {
      T data;

      Node* next;

      explicit Node(
        const T& value
      ):
        data(value),
        next(nullptr)
      {}
    };

    explicit ListIterator(
      Node* node
    ):
      node_(node)
    {}

    Node* node_;
  };

  template< class T >
  class ConstListIterator
  {
    friend class List< T >;

  public:
    using value_type = T;

    using reference = const T&;

    using pointer = const T*;

    ConstListIterator():
      node_(nullptr)
    {}

    ConstListIterator(
      const ListIterator< T >& rhs
    ):
      node_(rhs.node_)
    {}

    reference operator*() const
    {
      return node_->data;
    }

    pointer operator->() const
    {
      return std::addressof(
        node_->data
      );
    }

    ConstListIterator& operator++()
    {
      node_ = node_->next;

      return *this;
    }

    ConstListIterator operator++(int)
    {
      ConstListIterator temp(*this);

      ++(*this);

      return temp;
    }

    bool operator==(
      const ConstListIterator& rhs
    ) const
    {
      return node_ == rhs.node_;
    }

    bool operator!=(
      const ConstListIterator& rhs
    ) const
    {
      return !(*this == rhs);
    }

  private:
    using Node =
      typename ListIterator< T >::Node;

    explicit ConstListIterator(
      Node* node
    ):
      node_(node)
    {}

    Node* node_;
  };

  template< class T >
  class List
  {
  public:
    using iterator =
      ListIterator< T >;

    using const_iterator =
      ConstListIterator< T >;

    List():
      head_(nullptr),
      tail_(nullptr)
    {}

    List(
      const List& rhs
    ):
      List()
    {
      for (const T& value : rhs)
      {
        push_back(value);
      }
    }

    List(
      List&& rhs
    ) noexcept:
      head_(rhs.head_),
      tail_(rhs.tail_)
    {
      rhs.head_ = nullptr;

      rhs.tail_ = nullptr;
    }

    ~List()
    {
      clear();
    }

    List& operator=(List rhs)
    {
      swap(rhs);

      return *this;
    }

    void swap(List& rhs) noexcept
    {
      std::swap(head_, rhs.head_);

      std::swap(tail_, rhs.tail_);
    }

    iterator begin()
    {
      return iterator(head_);
    }

    iterator end()
    {
      return iterator(nullptr);
    }

    const_iterator begin() const
    {
      return const_iterator(head_);
    }

    const_iterator end() const
    {
      return const_iterator(nullptr);
    }

    bool empty() const noexcept
    {
      return head_ == nullptr;
    }

    void clear() noexcept
    {
      while (head_)
      {
        Node* temp = head_;

        head_ = head_->next;

        delete temp;
      }

      tail_ = nullptr;
    }

    void push_back(
      const T& value
    )
    {
      Node* node =
        new Node(value);

      if (!head_)
      {
        head_ = node;

        tail_ = node;
      }
      else
      {
        tail_->next = node;

        tail_ = node;
      }
    }

    bool contains(
      const T& value
    ) const
    {
      for (const T& current : *this)
      {
        if (current == value)
        {
          return true;
        }
      }

      return false;
    }

  private:
    using Node =
      typename iterator::Node;

    Node* head_;

    Node* tail_;
  };
}

#endif
