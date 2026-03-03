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
