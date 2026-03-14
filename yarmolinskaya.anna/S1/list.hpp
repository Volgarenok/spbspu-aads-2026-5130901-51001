#ifndef LIST_HPP
#define LIST_HPP

#include <stdexcept>
#include <utility>

namespace yarmolinskaya
{
  template< class T >
  class List
  {
  private:
    struct Node
    {
      T data;
      Node* next;
      Node* prev;
    };

  public:
    class Iterator
    {
      friend class List< T >;

    private:
      Node* node_;

      explicit Iterator(Node* node):
        node_(node)
      {}

    public:
      Iterator():
        node_(nullptr)
      {}

      T& operator*() const
      {
        return node_->data;
      }

      Iterator& operator++()
      {
        node_ = node_->next;
        return *this;
      }

      Iterator operator++(int)
      {
        Iterator tmp(*this);
        ++(*this);
        return tmp;
      }

      bool operator==(const Iterator& other) const
      {
        return node_ == other.node_;
      }

      bool operator!=(const Iterator& other) const
      {
        return node_ != other.node_;
      }
    };

    class ConstIterator
    {
      friend class List< T >;

    private:
      Node* node_;

      explicit ConstIterator(Node* node):
        node_(node)
      {}

    public:
      ConstIterator():
        node_(nullptr)
      {}

      const T& operator*() const
      {
        return node_->data;
      }

      ConstIterator& operator++()
      {
        node_ = node_->next;
        return *this;
      }

      ConstIterator operator++(int)
      {
        ConstIterator tmp(*this);
        ++(*this);
        return tmp;
      }

      bool operator==(const ConstIterator& other) const
      {
        return node_ == other.node_;
      }

      bool operator!=(const ConstIterator& other) const
      {
        return node_ != other.node_;
      }
    };

  private:
    Node* head_;
    Node* tail_;

  public:
    List():
      head_(nullptr),
      tail_(nullptr)
    {}

    ~List()
    {
      clear();
    }

    bool empty() const
    {
      return head_ == nullptr;
    }

    void clear()
    {
      while (!empty())
      {
        pop_front();
      }
    }

    Iterator begin()
    {
      return Iterator(head_);
    }

    Iterator end()
    {
      return Iterator(nullptr);
    }

    ConstIterator begin() const
    {
      return ConstIterator(head_);
    }

    ConstIterator end() const
    {
      return ConstIterator(nullptr);
    }
  };
}

#endif
