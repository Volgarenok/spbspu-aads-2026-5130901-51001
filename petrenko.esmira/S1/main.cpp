#include <iostream>
#include <string>
#include <climits>
#include <limits>
namespace petrenko {
  template<class T> class List;

template<class T>
  class LIter {
    friend class List<T>;
  private:
    typename List<T>::Node *current;
    LIter(typename List<T>::Node *node) : current(node) {}
  public:
    LIter() : current(nullptr) {}
    LIter(const LIter<T>& other) : current(other.current) {}
    LIter<T>& operator=(const LIter<T>& other) {
      if (this != &other) {
        current = other.current;
      }
      return *this;
    }
    LIter<T>& operator++() {
      if (current) {
        current = current->pNext;
      }
      return *this;
    }
    T& operator*() const {
      return current->data;
    }
    T* operator->() const {
      return &current->data;
    }
    bool operator!=(const LIter<T>& other) const {
      return current != other.current;
    }
    bool operator==(const LIter<T>& other) const {
      return current == other.current;
    }
  };

  template<class T>
  class LCIter {
    friend class List<T>;
  private:
    const typename List<T>::Node *current;
    LCIter(const typename List<T>::Node *node) : current(node) {}
  public:
    LCIter() : current(nullptr) {}
    LCIter(const LCIter<T>& other) : current(other.current) {}
    LCIter<T>& operator=(const LCIter<T>& other) {
      if (this != &other) {
        current = other.current;
      }
      return *this;
    }
    LCIter<T>& operator++() {
      if (current) {
        current = current->pNext;
      }
      return *this;
    }
    const T& operator*() const {
      return current->data;
    }
    const T* operator->() const {
      return &current->data;
    }
    bool operator!=(const LCIter<T>& other) const {
      return current != other.current;
    }
    bool operator==(const LCIter<T>& other) const {
      return current == other.current;
    }
  };

  template<class T>
  class List {
  public:
    friend class LIter<T>;
    friend class LCIter<T>;

    List() : Size(0), head(nullptr) {}

    List(const List<T>& other) : Size(0), head(nullptr) {
      Node* current = other.head;
      while (current != nullptr) {
        insert(current->data, Size);
        current = current->pNext;
      }
    }

    ~List() {
      clear();
    }

    List& operator=(const List<T>& other) {
      if (this != &other) {
        clear();
        Node* current = other.head;
        while (current != nullptr) {
          insert(current->data, Size);
          current = current->pNext;
        }
      }
      return *this;
    }

    T& operator[](const int index) {
      int counter = 0;
      Node* current = this->head;

      while (current != nullptr) {
        if (counter == index) {
          return current->data;
        }
        current = current->pNext;
        counter++;
      }

      return current->data;
    }

    size_t getSize() {
      return Size;
    }

    void insert(T data, int index) {
      if (index == 0) {
        head = new Node(data, head);
        ++Size;
      } else {
        Node* previous = head;
        for (int i = 0; i < index - 1; ++i) {
          previous = previous->pNext;
        }
        Node* newNode = new Node(data, previous->pNext);
        previous->pNext = newNode;
        ++Size;
      }
    }

    void removeAt(int index) {
      if (index == 0) {
        Node* temp = head;
        head = head->pNext;
        delete temp;
        --Size;
      } else {
        Node* previous = head;
        for (int i = 0; i < index - 1; ++i) {
          previous = previous->pNext;
        }
        Node* toDelete = previous->pNext;
        previous->pNext = toDelete->pNext;
        delete toDelete;
        --Size;
      }
    }

    void clear() {
      while (head != nullptr) {
        Node* temp = head;
        head = head->pNext;
        delete temp;
      }
      Size = 0;
    }

    LIter<T> begin() {
      return LIter<T>(head);
    }
    LIter<T> end() {
      return LIter<T>(nullptr);
    }
    LCIter<T> cbegin() const {
      return LCIter<T>(head);
    }
    LCIter<T> cend() const {
      return LCIter<T>(nullptr);
    }

  private:
    size_t Size;

    class Node {
    public:
      Node* pNext;
      T data;
      Node(T data = T(), Node *pNext = nullptr) {
        this->data = data;
        this->pNext = pNext;
      }
    };
    Node* head;
  };
}

int main() {
  petrenko::List<std::string> titles;
  petrenko::List<petrenko::List<int>> numNum;
  std::string line;
  bool overflow = false;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }

    std::string title;
    size_t count = 0;

    while (count < line.size() && line[count] != ' ') {
      title += line[count];
      ++count;
    }
    titles.insert(title, titles.getSize());

    while (count < line.size() && line[count] == ' ') {
      ++count;
    }

    petrenko::List<int> numbers;
    while (count < line.size()) {
      if (line[count] >= '0' && line[count] <= '9') {
        long long num = 0;

        while (count < line.size() && line[count] >= '0' && line[count] <= '9') {
          num = num * 10 + (line[count] - '0');
          if (num > 2147483647) {
            overflow = true;
          }
          ++count;
        }
        if (overflow) {
          throw std::overflow_error("overflow");
          return 1;
        }
        numbers.insert(num, numbers.getSize());
      } else {
        ++count;
      }
      while (count < line.size() && (line[count] == ' ' || line[count] == '\n')) {
        ++count;
      }
    }
    numNum.insert(numbers, numNum.getSize());
  }

  if (titles.getSize() == 0) {
     std::cout << "0\n";
     return 0;
  }

  size_t countL = 0;
  for (petrenko::LIter<std::string> tit = titles.begin(); tit != titles.end(); ++tit) {
    ++countL;
    std::cout << *tit;
    if (countL != titles.getSize()) {
      std::cout << ' ';
    }
  }
  if (countL > 0) {
    std::cout << "\n";
  }

  petrenko::List<int> lastLine;
  size_t maxi = 1;
  for (petrenko::LIter<petrenko::List<int>> numbers = numNum.begin(); numbers != numNum.end(); ++numbers) {
    if ((*numbers).getSize() > maxi) {
      maxi = (*numbers).getSize();
    }
  }

  if (titles.getSize() == 0) {
    maxi = 0;
  }

  for (size_t counter = 0; counter < maxi; ++counter) {
    int summa = 0;
    bool firstInRow = true;
    for (petrenko::LIter<petrenko::List<int>> numbers = numNum.begin(); numbers != numNum.end(); ++numbers) {
      if (counter < (*numbers).getSize()) {
        if (!firstInRow) {
          std::cout << ' ';
        }
        std::cout << (*numbers)[counter];
        summa += (*numbers)[counter];
        firstInRow = false;
      }
    }
    lastLine.insert(summa, lastLine.getSize());
    if (summa || counter < maxi - 1) {
      std::cout << "\n";
    }
  }

  if (titles.getSize() > 0) {
    size_t count = 0;
    for (petrenko::LIter<int> sums = lastLine.begin(); sums != lastLine.end(); ++sums) {
      ++count;
      std::cout << (*sums);
      if (count != lastLine.getSize()) {
        std::cout << ' ';
      }
    }
  }

  if (overflow) {
    throw std::overflow_error("overflow");
    return 1;
  }

  std::cout << "\n";
  return 0;
}
