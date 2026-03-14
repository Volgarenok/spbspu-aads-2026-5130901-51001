#ifndef LIST_HPP
#define LIST_HPP

namespace nabieva
{
  template< class T >
  struct Node
  {
    T data;
    Node* next;
    Node(const T& value):
      data(value),
      next(nullptr)
    {}
  };

  template< class T > class List;
  template < class T >
  class LIter {
    friend class List< T >;
  private:
    Node<T>* node;
  public:
    LIter(Node<T>* n = nullptr):
      node(n)
    {}
  };

  template < class T >
  class LCIter {
    friend class List< T >;
  private:
    const Node<T>* node;
  public:
    LCIter(const Node<T>* n = nullptr):
      node(n)
    {}
  };

  template < class T >
  class List {
  private:
    Node<T>* head;
  public:
    List():
      head(nullptr)
    {}
  };
}

#endif
