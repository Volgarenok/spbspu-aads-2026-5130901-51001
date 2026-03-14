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

    Node* head_;
    Node* tail_;

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

    List():
      head_(nullptr),
      tail_(nullptr)
    {}

    List(const List& other):
      head_(nullptr),
      tail_(nullptr)
    {
      for (auto it = other.begin(); it != other.end(); ++it)
      {
        push_back(*it);
      }
    }

    List(List&& other) noexcept:
      head_(other.head_),
      tail_(other.tail_)
    {
      other.head_ = nullptr;
      other.tail_ = nullptr;
    }

    List& operator=(const List& other)
    {
      if (this != &other)
      {
        List tmp(other);
        std::swap(head_, tmp.head_);
        std::swap(tail_, tmp.tail_);
      }
      return *this;
    }

    List& operator=(List&& other) noexcept
    {
      if (this != &other)
      {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
      }
      return *this;
    }

    ~List()
    {
      clear();
    }

    bool empty() const
    {
      return head_ == nullptr;
    }

    T& front()
    {
      return head_->data;
    }

    const T& front() const
    {
      return head_->data;
    }

    T& back()
    {
      return tail_->data;
    }

    const T& back() const
    {
      return tail_->data;
    }

    void push_back(const T& value)
    {
      Node* node = new Node{value, nullptr, tail_};

      if (tail_)
      {
        tail_->next = node;
      }
      else
      {
        head_ = node;
      }

      tail_ = node;
    }

    void push_front(const T& value)
    {
      Node* node = new Node{value, head_, nullptr};

      if (head_)
      {
        head_->prev = node;
      }
      else
      {
        tail_ = node;
      }

      head_ = node;
    }

    void pop_front()
    {
      if (empty())
      {
        throw std::runtime_error("pop_front on empty list");
      }

      Node* tmp = head_;
      head_ = head_->next;

      if (head_)
      {
        head_->prev = nullptr;
      }
      else
      {
        tail_ = nullptr;
      }

      delete tmp;
    }

    void pop_back()
    {
      if (empty())
      {
        throw std::runtime_error("pop_back on empty list");
      }

      Node* tmp = tail_;
      tail_ = tail_->prev;

      if (tail_)
      {
        tail_->next = nullptr;
      }
      else
      {
        head_ = nullptr;
      }

      delete tmp;
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

    ConstIterator cbegin() const
    {
      return ConstIterator(head_);
    }

    ConstIterator cend() const
    {
      return ConstIterator(nullptr);
    }
  };
}

#endif
