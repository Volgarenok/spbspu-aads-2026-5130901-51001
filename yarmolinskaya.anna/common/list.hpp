#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
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
      explicit Node(const T& value):
        data(value),
        next(nullptr)
      {}

      T data;
      Node* next;
    };

  public:
    class Iterator
    {
      friend class List< T >;

    public:
      explicit Iterator(Node* ptr = nullptr):
        ptr_(ptr)
      {}

      T& operator*() const
      {
        return ptr_->data;
      }

      Iterator& operator++()
      {
        ptr_ = ptr_->next;
        return *this;
      }

      bool operator!=(const Iterator& other) const noexcept
      {
        return ptr_ != other.ptr_;
      }

      bool operator==(const Iterator& other) const noexcept
      {
        return ptr_ == other.ptr_;
      }

    private:
      Node* ptr_;
    };

    List():
      head_(nullptr),
      tail_(nullptr)
    {}

    List(const List& other):
      head_(nullptr),
      tail_(nullptr)
    {
      Node* current = other.head_;

      while (current)
      {
        push_back(current->data);
        current = current->next;
      }
    }

    ~List()
    {
      clear();
    }

    List& operator=(const List& other)
    {
      if (this != &other)
      {
        List tmp(other);
        swap(tmp);
      }

      return *this;
    }

    bool empty() const noexcept
    {
      return head_ == nullptr;
    }

    T& front()
    {
      if (empty())
      {
        throw std::logic_error("list empty");
      }

      return head_->data;
    }

    const T& front() const
    {
      if (empty())
      {
        throw std::logic_error("list empty");
      }

      return head_->data;
    }

    T& back()
    {
      if (empty())
      {
        throw std::logic_error("list empty");
      }

      return tail_->data;
    }

    const T& back() const
    {
      if (empty())
      {
        throw std::logic_error("list empty");
      }

      return tail_->data;
    }

    void push_front(const T& value)
    {
      Node* node = new Node(value);

      node->next = head_;
      head_ = node;

      if (!tail_)
      {
        tail_ = node;
      }
    }

    void push_back(const T& value)
    {
      Node* node = new Node(value);

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

    void pop_front()
    {
      if (empty())
      {
        throw std::logic_error("list empty");
      }

      Node* tmp = head_;
      head_ = head_->next;

      if (!head_)
      {
        tail_ = nullptr;
      }

      delete tmp;
    }

    void clear() noexcept
    {
      while (!empty())
      {
        Node* tmp = head_;
        head_ = head_->next;
        delete tmp;
      }

      tail_ = nullptr;
    }

    Iterator begin() noexcept
    {
      return Iterator(head_);
    }

    Iterator end() noexcept
    {
      return Iterator(nullptr);
    }

    Iterator begin() const noexcept
    {
      return Iterator(head_);
    }

    Iterator end() const noexcept
    {
      return Iterator(nullptr);
    }

    void swap(List& other) noexcept
    {
      std::swap(head_, other.head_);
      std::swap(tail_, other.tail_);
    }

  private:
    Node* head_;
    Node* tail_;
  };
}

#endif
