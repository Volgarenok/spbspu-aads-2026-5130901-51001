namespace kitserov
{
  template< class T > class List;
  template < class T >
  class LIter
  {
    friend class List< T >;
    List<T>* node;
    LIter(List< T >* n) : node(n) {}
  public:
    LIter() : node(nullptr) {}
    T& operator*() const
    {
      return node->data;
    }
    T* operator->() const
    {
      return &node->data;
    }
    LIter& operator++()
    {
      if (node) {
      	node = node->next;
      }
      return *this;
    }
    bool operator==(const LIter& other) const
    {
      return node == other.node;
    }
    bool operator!=(const LIter& other) const
    {
      return node != other.node;
    }
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    
  };

  template< class T >
  class List
  {
  public:
    T data;
    List< T >* next;
    List(T v, List< T >* n) : data(v), next(n) {}
  };
}

int main()
{}
