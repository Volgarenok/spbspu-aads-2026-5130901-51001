#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <utility>
#include <stdexcept>

namespace smirnova
{
  template < class T >
  struct Node {
    T data;
    Node* next;
    Node* prev;

    Node(const T& d) :
      data(d),
      next(nullptr),
      prev(nullptr)
    {}
  };

  template < class T >
  class LIter;

  template < class T >
  class LCIter;
  
  template< class T >
  class List
  {
  private:
    Node<T>* fake;
    size_t sz;

  public:
    List() :
      sz(0)
    {
      fake = new Node< T >(T());
      fake->next = nullptr;
      fake->prev = nullptr;
    }

    ~List()
    {
      clear();
      delete fake;
    }

    List(const List& other) :
      List()
    {
      for (LCIter< T > it = other.begin(); it.valid(); it.next())
      {
        push_back(it.value());
      }
    }
    List& operator=(const List& other)
    {
      if (this != &other) {
        clear();
        for (LCIter< T > it = other.begin(); it.valid(); it.next())
        {
          push_back(it.value());
        }
      }
      return *this;
    }
    bool empty() const noexcept
    {
      return sz == 0;
    }
    size_t size() const noexcept
    {
      return sz;
    }

    void push_back(const T& val)
    {
      Node< T >* n = new Node< T >(val);
      n->next = nullptr;
      if (empty()) {
        fake->next = n;
        fake->prev = n;
        n->prev = fake;
      } else {
        Node< T >* tail = fake->prev;
        tail->next = n;
        n->prev = tail;
        fake->prev = n;
      }
      ++sz;
    }

    void push_front(const T& val)
    {
      Node< T >* n = new Node< T >(val);
      n->next = fake->next;
      n->prev = fake;
      if (fake->next) {
        fake->next->prev = n;
      }
      fake->next = n;
      if (sz == 0) {
        fake->prev = n;
      }
      ++sz;
    }

    void pop_front()
    {
      if (empty() || !fake->next) {
        throw std::out_of_range("pop_front on empty list");
      }
      Node< T >* tmp = fake->next;
      fake->next = tmp->next;
      if (tmp->next) {
        tmp->next->prev = fake;
      } else {
        fake->prev = nullptr;
      }
      delete tmp;
      --sz;
    }

    void pop_back()
    {
      if (empty()) {
        throw std::out_of_range("pop_back on empty list");
      }
      Node< T >* tail = fake->prev;
      if (tail == fake->next) {
        fake->next = nullptr;
        fake->prev = nullptr;
      } else {
        fake->prev = tail->prev;
        tail->prev->next = nullptr;
      }
      delete tail;
      --sz;
    }

    void clear() noexcept
    {
      Node< T >* curr = fake->next;
      while (curr) {
        Node< T >* tmp = curr->next;
        delete curr;
        curr = tmp;
      }
      fake->next = nullptr;
      fake->prev = nullptr;
      sz = 0;
    }

    LIter< T > begin()
    {
      return LIter< T >(fake->next);
    }
    LIter< T > tail()
    {
      return LIter< T >(fake->prev);
    }
    LCIter< T > begin() const
    {
      return LCIter< T >(fake->next);
    }
    LCIter< T > tail() const
    {
      return LCIter< T >(fake->prev);
    }
  };

  template < class T >
  class LIter
  {
    friend class List< T >;
  private:
    Node< T >* node;
  public:
    LIter(Node< T >* n = nullptr) :
      node(n)
    {}
    bool valid() const
    {
      return node != nullptr;
    }
    void next()
    { 
      if (node){
        node = node->next;
      }
    }
    void prev()
    {
      if (node) {
        node = node->prev;
      }
    }
    T& value()
    {
      if (!node) {
        throw std::out_of_range("Invalid iterator");
      }
      return node->data; 
    }
  };

  template < class T >
  class LCIter
  {
    friend class List< T >;
  private:
    const Node< T >* node;
  public:
    LCIter(const Node< T >* n = nullptr) :
      node(n)
    {}
    bool valid() const
    {
      return node;
    }
    void next()
    { 
      if (node) {
        node = node->next;
      }
    }
    void prev()
    {
      if (node) {
        node = node->prev;
      }
    }
    const T& value()
    {
      if (!node) {
        throw std::out_of_range("Invalid iterator");
      }
      return node->data; 
    }
  };

  void formColSeq(List< std::pair< std::string, List< int > > >& seq) {
    size_t maxSize = 0;

    for (LIter< std::pair< std::string, List< int > > > it = seq.begin(); it.valid(); it.next())
    {
      if (it.value().second.size() > maxSize) {
        maxSize = it.value().second.size();
      }
    }

    List< LIter< int > > iterators;
    for (LIter< std::pair< std::string, List< int > > > it = seq.begin(); it.valid(); it.next())
    {
      iterators.push_back(it.value().second.begin());
    }

    for (size_t row = 0; row < maxSize; ++row)
    {
      for (LIter< LIter< int > > it_it = iterators.begin(); it_it.valid(); it_it.next())
      {
        LIter< int >& it_list = it_it.value();
        if (it_list.valid()) {
          std::cout << std::setw(5) << it_list.value() << " ";
          it_list.next();
        } else {
          std::cout << "     ";
        }
      }
      std::cout << std::endl;
    }

    for (LIter< std::pair< std::string, List< int > > > it = seq.begin(); it.valid(); it.next())
    {
      int sum = 0;
      const List< int >& lst = it.value().second;
      for (LCIter< int > jt = lst.begin(); jt.valid(); jt.next())
      {
        sum += jt.value();
      }
      std::cout << std::setw(5) << sum << " ";
    }
    std::cout << std::endl;
  }
}

int main() {
  using namespace smirnova;

  List< std::pair< std::string, List< int > > > seq;
  std::string line;

  try {
    while (std::getline(std::cin, line)) {
      if (line.empty()) {
        continue;
      }
      std::istringstream iss(line);
      std::string name;
      iss >> name;

      List< int > numbers;
      int x;
      while (iss >> x) {
        numbers.push_back(x);
      }
      seq.push_back({ name, numbers });
      if (std::cin.peek() == '\n') {
        std::cin.get();
      }
    }

    if (seq.empty()) {
      std::cout << 0 << std::endl;
      return 0;
    }

    std::cout << std::endl;
    for (LIter< std::pair<std::string, List< int > > > it = seq.begin(); it.valid(); it.next())
    {
      std::cout << it.value().first << " ";
    }
    std::cout << std::endl;

    formColSeq(seq);

    seq.clear();

  } catch(const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "Unknown exception" << std::endl;
    return 1;
  }

  return 0;
}