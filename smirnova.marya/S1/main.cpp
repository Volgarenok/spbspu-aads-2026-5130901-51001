#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>

namespace smirnova
{
  template < typename T >
  struct Node
  {
    T data;
    Node* next;
    Node* prev;
    Node(const T& d):
      data(d),
      next(nullptr),
      prev(nullptr) {}
    Node(T&& d):
      data(std::move(d)),
      next(nullptr),
      prev(nullptr)
    {}
  };

  template < typename T >
  class LIter;

  template < typename T >
  class LCIter;

  template < typename T >
  class List
  {
  private:
    Node< T >* sentinel;
    size_t count;

  public:
    List():
      count(0)
    {
      sentinel = new Node< T >(T());
      sentinel->next = sentinel;
      sentinel->prev = sentinel;
    }

    void clear() noexcept {
      Node< T >* current = sentinel->next;
      while (current != sentinel) {
        Node< T >* tmp = current->next;
        delete current;
        current = tmp;
      }
      sentinel->next = sentinel;
      sentinel->prev = sentinel;
      count = 0;
    }

    ~List()
    {
      clear();
      delete sentinel;
    }

    List(const List& other):
      List() {
        for (LCIter<T> it = other.cbegin(); it.valid(); it.next())
        {
          push_back(it.value());
        }
    }

    List(List&& other) noexcept:
      sentinel(other.sentinel),
      count(other.count)
    {
      other.sentinel = new Node< T >(T());
      other.sentinel->next = other.sentinel;
      other.sentinel->prev = other.sentinel;
      other.count = 0;
    }

    List& operator=(const List& other)
    {
      if (this != &other) {
        List tmp(other);
        swap(tmp);
      }
      return *this;
    }

    List& operator=(List&& other) noexcept
    {
      if (this != &other) {
        clear();
        delete sentinel;
        sentinel = other.sentinel;
        count = other.count;
        other.sentinel = new Node< T >(T());
        other.sentinel->next = other.sentinel;
        other.sentinel->prev = other.sentinel;
        other.count = 0;
      }
      return *this;
    }

    void swap(List& other) noexcept
    {
      std::swap(sentinel, other.sentinel);
      std::swap(count, other.count);
    }

    bool empty() const noexcept
    {
      return count == 0;
    }
    size_t size() const noexcept
    {
      return count;
    }

    void push_back(const T& val)
    {
      Node< T >* n = new Node< T >(val);
      n->next = sentinel;
      n->prev = sentinel->prev;
      sentinel->prev->next = n;
      sentinel->prev = n;
      ++count;
    }

    void push_back(T&& val)
    {
      Node< T >* n = new Node< T >(std::move(val));
      n->next = sentinel;
      n->prev = sentinel->prev;
      sentinel->prev->next = n;
      sentinel->prev = n;
      ++count;
    }

    void push_front(const T& val)
    {
      Node< T >* n = new Node< T >(val);
      n->prev = sentinel;
      n->next = sentinel->next;
      sentinel->next->prev = n;
      sentinel->next = n;
      ++count;
    }

    void push_front(T&& val)
    {
      Node< T >* n = new Node< T >(std::move(val));
      n->prev = sentinel;
      n->next = sentinel->next;
      sentinel->next->prev = n;
      sentinel->next = n;
      ++count;
    }

    void pop_front()
    {
      if (empty()) {
        throw std::out_of_range("pop_front on empty list");
      }
      Node< T >* tmp = sentinel->next;
      sentinel->next = tmp->next;
      tmp->next->prev = sentinel;
      delete tmp;
      --count;
    }

    void pop_back()
    {
      if (empty()) {
        throw std::out_of_range("pop_back on empty list");
      }
      Node< T >* tmp = sentinel->prev;
      sentinel->prev = tmp->prev;
      tmp->prev->next = sentinel;
      delete tmp;
      --count;
    }

    LIter<T> begin()
    {
      return LIter<T>(sentinel->next, sentinel);
    }
    LIter<T> end()
    {
      return LIter<T>(sentinel, sentinel);
    }
    LCIter<T> begin() const
    {
      return LCIter<T>(sentinel->next, sentinel);
    }
    LCIter<T> end() const
    {
      return LCIter<T>(sentinel, sentinel);
    }
    LCIter<T> cbegin() const
    {
      return LCIter<T>(sentinel->next, sentinel);
    }
    LCIter<T> cend() const
    {
      return LCIter<T>(sentinel, sentinel);
    }
  };

  template < typename T >
  class LIter
  {
  private:
      Node< T >* node;
      Node< T >* sentinel;
  public:
    LIter(Node< T >* n = nullptr, Node< T >* s = nullptr):
      node(n),
      sentinel(s)
    {}
    bool valid() const
    {
      return node != sentinel;
    }
    void next()
    {
      if(node)
      {
        node = node->next;
      }
    }
    T& value()
    {
      return node->data;
    }
    const T& value() const
    {
      return node->data;
    }
  };

  template < typename T >
  class LCIter
  {
  private:
    const Node< T >* node;
    const Node< T >* sentinel;
  public:
    LCIter(const Node< T >* n = nullptr, const Node< T >* s = nullptr):
      node(n),
      sentinel(s)
    {}
    bool valid() const
    {
      return node != sentinel;
    }
    void next()
    {
      if(node)
      {
        node = node->next;
      }
    }
    const T& value() const
    {
      return node->data;
    }
  };

  template < typename T >
  LCIter< T > getElementAt(const List< T >& lst, size_t idx)
  {
    LCIter< T > it = lst.cbegin();
    size_t i = 0;
    while(it.valid() && i < idx) {
      it.next();
      ++i;
    }
    return it;
  }

  template < typename T >
  void printList(const List< T >& lst)
  {
    if(lst.empty()) {
      std::cout << "0\n";
      return;
    }
    bool first = true;
    for(LCIter< T > it = lst.cbegin(); it.valid(); it.next())
    {
      if(!first) {
        std::cout << " ";
      }
      std::cout << it.value();
      first = false;
    }
    std::cout << "\n";
  }

  bool willOverflow(size_t a, size_t b)
  {
    return b > std::numeric_limits< size_t >::max() - a;
  }

  bool processSequences(const List< std::pair< std::string, List< size_t > > >& sequences)
  {
    if(sequences.empty()) {
      std::cout << "0\n";
      return true;
    }

    bool firstName = true;
    for(LCIter< std::pair< std::string, List< size_t > > > it = sequences.cbegin(); it.valid(); it.next())
    {
      if(!firstName) {
        std::cout << " ";
      }
      std::cout << it.value().first;
      firstName = false;
    }
    std::cout << "\n";

    size_t maxLen = 0;
    for(LCIter< std::pair< std::string, List< size_t > > > it = sequences.cbegin(); it.valid(); it.next())
    {
      if(it.value().second.size() > maxLen) {
        maxLen = it.value().second.size();
      }
    }

    for(size_t row = 0; row < maxLen; ++row)
    {
      bool firstNum = true;
      for(LCIter< std::pair< std::string, List< size_t > > > it = sequences.cbegin(); it.valid(); it.next())
      {
        if(row < it.value().second.size()) {
          LCIter< size_t > numIt = getElementAt(it.value().second, row);
          if(!firstNum) {
            std::cout << " ";
          }
          std::cout << numIt.value();
          firstNum = false;
        }
      }
      if(!firstNum) {
        std::cout << "\n";
      }
    }

    List< size_t > sums;
    for(size_t row = 0; row < maxLen; ++row)
    {
      size_t sum = 0;
      for(LCIter< std::pair< std::string, List< size_t > > > it = sequences.cbegin(); it.valid(); it.next())
      {
        if(row < it.value().second.size()) {
          size_t val = getElementAt(it.value().second, row).value();
          if(willOverflow(sum, val)) {
            std::cerr << "overflow\n";
            return false;
          }
          sum += val;
        }
      }
      sums.push_back(sum);
    }

    printList(sums);
    return true;
  }
}

int main() {
  using namespace smirnova;

  List< std::pair< std::string, List< size_t > > > sequences;
  bool overflowDetected = false;

  std::string name;

  while (std::cin >> name) {
      List< size_t > numbers;

      while (true) {
          int c = std::cin.peek();
          if (c == '\n' || c == '\r' || c == EOF) {
              break;
          }

          size_t num = 0;

          if (!(std::cin >> num)) {
              overflowDetected = true;
              std::cin.clear();
              std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
              break;
          }

          numbers.push_back(num);
      }

      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      sequences.push_back({name, std::move(numbers)});
  }

  if (overflowDetected) {
    std::cerr << "overflow\n";
    return 1;
  }

  if (!processSequences(sequences)) {
    return 1;
  }

  return 0;
}
