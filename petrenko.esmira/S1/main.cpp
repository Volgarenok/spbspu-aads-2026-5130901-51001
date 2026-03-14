#include <iostream>

namespace petrenko {
  template< class T > class List;

  template< class T >
  class LIter {
  friend class List< T >;
  private:
    typename List<T>::Node<T>* current;
  public:
    LIter() : current(nullptr) {}
    LIter(const LIter<T>& other) : current(other.current) {} // Копирующий конструктор
    LIter<T>& operator=(const LIter<T>& other) { // Оператор присваивания
      if (this != &other) {
        current = other.current;
      }
      return *this;
    }
  };

  template< class T >
  class LCIter {
  friend class List< T >;
  //
  };

  template< class T >
  class List {
  public:
    List();
    ~List();
    size_t GetSize() { return Size; } // возвращает размер списка
    void insert(T data, int index); // добавление элемента по индексу
    void removeAt(int index); // удаление элемента по индексу
    void clear(); // очищение списка
    LIter<T> begin() {
      return LIter<T>(head);
    }

    LIter<T> end() {
      return LIter<T>(nullptr);
    }

    LCIter<T> cbegin() const {
      return LIter<T>(head);
    }

    LСIter<T> сend() const {
      return LСIter<T>(nullptr);
    }

  private:
    size_t Size;

    class Node {
    public:
      Node * pNext;
      T data;

      Node(T data = T(), Node *pNext = nullptr) {
        this->data = data;
        this->pNext = pNext;
      }
    };

    Node *head;
  };

  template< class T >
  List<T>::List() {
    Size = 0;
    head = nullptr;
  }

  template< class T >
  List<T>::~List() {
    clear();
  }

  template< class T >
  void List<T>::insert(T data, int index) {
    if (index == 0) {
      head = new Node(data, head);
      ++Size;
    } else {
      Node *previous = this->head;

      for (int i = 0; i < index - 1; i++) {
        previous = previous->pNext;
      }

      Node *newNode = new Node(data, previous->pNext);
      previous->pNext = newNode;
      Size++;
    }
  }

  template< class T >
  void List<T>::removeAt(int index) {
    if (index == 0) {
      Node *temp = head;
      head = head->pNext;
      delete temp;
      Size--;
    } else {
      Node *previous = this->head;
      for (int i = 0; i < index - 1; i++) {
        previous = previous->pNext;
      }

      Node *toDelete = previous->pNext;
      previous->pNext = toDelete->pNext;

      delete toDelete;
      Size--;
    }
  }

  template< class T >
  void List<T>::clear() {
    while (Size) {
      Node *temp = head;
      head = head->pNext;
      delete temp;
      Size--;
    }
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
