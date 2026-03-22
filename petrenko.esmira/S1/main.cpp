#include <iostream>
#include <string>
#include <cctype>

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

    List();
    ~List();
    List(const List& other);
    List& operator=(const List& other);

    size_t getSize() { return Size; }
    void insert(T data, int index);
    void removeAt(int index);
    void clear();

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

  template<class T>
  List<T>::~List() {
    clear();
  }

  template<class T>
  List<T>::List(const List<T>& other) : Size(0), head(nullptr) {
    Node* current = other.head;
    while (current != nullptr) {
      insert(current->data, Size);
      current = current->pNext;
    }
  }

  template<class T>
  List<T>& List<T>::operator=(const List<T>& other) {
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

  template<class T>
  void List<T>::insert(T data, int index) {
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

  template<class T>
  void List<T>::removeAt(int index) {
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

  template<class T>
  void List<T>::clear() {
    while (head != nullptr) {
      Node* temp = head;
      head = head->pNext;
      delete temp;
    }
    Size = 0;
  }
}

int main() {
  // список списков
  // список названий
  while (!std::cin.eof()) {
    std::string title;
    std::cin >> title;
    std::cout << title << "название";
    //вставить титле в список титлов
    // список чисел
    int num;
    while (std::cin >> num) {
      //вставить число в список
      std::cout << num << "число";
    }
  }
}

