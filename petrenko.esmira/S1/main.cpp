#include <iostream>
#include <string>

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

    size_t getSize() {
      return Size;
    }

    void insert(T data, int index) {
      if (index < 0) {
        return;
      }

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
      if (index < 0) {
        return;
      }

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

  while (std::getline(std::cin, line)) {
    if (line.empty()) continue;

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
        int num = 0;
        while (count < line.size() && line[count] >= '0' && line[count] <= '9') {
          num = num * 10 + (line[count] - '0');
          ++count;
        }
        numbers.insert(num, numbers.getSize());
      } else {
        ++count;
      }
      while (count < line.size() && line[count] == ' ') {
        ++count;
      }
    }
    numNum.insert(numbers, numNum.getSize());
  }
  return 0;
}
