#ifndef LIST_HPP
#define LIST_HPP

#include <stdexcept>
#include <utility>

namespace yarmolinskaya
{
  template< class T >
  class List
  {
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

  private:
    struct Node
    {
      T data;
      Node* next;
      Node* prev;
    };

    Node* head_;
    Node* tail_;
  };
}

#endif
