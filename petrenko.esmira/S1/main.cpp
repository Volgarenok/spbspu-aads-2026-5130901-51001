#include <iostream>

namespace petrenko {
  template< class T > class List;
  template< class T >
  class LIter {
  friend class List< T >;
  ...
  };
  template< class T >
  class LCIter {
  friend class List< T >;
  ...
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
  private:
    size_t Size;

    template<typename T>
    class Node {
    public:
      Node * pNext;
      T data;

      Node(T data = T(), Node *pNext = nullptr)
      {
	this->data = data;
	this->pNext = pNext;
      }
    };
  };
}

int main()
{
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
