#include <iostream>
#include <string>

namespace kitserov
{
  template< class T > class List;
  template < class T >
  class LIter
  {
    friend class List< T >;
    typename List< T >::Node* node_;
  public:
    LIter() : node_(nullptr) {}
    LIter(typename List<T>::Node* n) : node_(n) {}
    T& operator*() const
    {
      return node_->data;
    }
    T* operator->() const
    {
      return &node_->data;
    }
    LIter& operator++()
    {
      if (node_) {
      	node_ = node_->next;
      }
      return *this;
    }
    bool operator==(const LIter& other) const
    {
      return node_ == other.node;
    }
    bool operator!=(const LIter& other) const
    {
      return node_ != other.node;
    }
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    
  };

  template< class T >
  struct List
  {
  public:
    struct Node {
      T data;
      Node* next;
      Node(T v, Node* n = nullptr) : data(v), next(n) {}
    };
    List() : head(nullptr), size(0) {}
    ~List()
    {
      clear();
    }
    LIter< T > begin() 
    {
      return LIter< T >(head);
    }
    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }
    LIter< T > operator[](size_t index)
    {
      if (index >= size) {
        throw std::out_of_range("out_of_range");
      }
      Node* current = head;
      for (size_t i = 0; i < index; i++) {
        current = current->next;
      }
      return LIter< T >(current);
    }
    Node* add(T v)
    {
      Node* newNode = new Node(v, head);
      head = newNode;
      size++;
      return newNode;
    }
    Node* insert(T v, LIter< T > pos)
    {
      size++;
      Node* newNode = new Node(v, pos.node_->next);
      pos.node_->next = newNode;
      return newNode;
    }
    Node* insert_tail(T v)
    {
      size++;
      return insert(v, this[size - 1]);
    }
    T& front()
    {
      if (!size) {
        throw std::out_of_range("list is empty");
      }
      return head->data;
    }
    T& back()
    {
      if (!size) {
        throw std::out_of_range("list is empty");
      }
      return *(this[size - 1]);
    }
  private:
    Node* head;
    size_t size;
  };


}

int main()
{
  using namespace kitserov;
  size_t names_cap = 8;
  size_t names_count = 0;
  std::string* names = new std::string[names_cap];
  List< List < int > > list_of_lists;
  while (true) {
    std::string name;
    if (!(std::cin >> name)) {
      break;
    }
    names[names_count] = name;
    names_count++;
    if (names_count >= names_cap) {
      names_cap *= 2;
      std::string* tmp = new std::string[names_cap];
      for (size_t i = 0; i < names_count; ++i) {
        tmp[i] = names[i];
      }
      delete[] names;
      names = tmp;
    }
    while (true) {
      
    }
  }
  std::cout << "\n";
  for (size_t i = 0; i < names_count; ++i) {
    std::cout << names[i] << " ";
  }
  return 0;
}
