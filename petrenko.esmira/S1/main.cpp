#include <iostream>

namespace petrenko
{
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
    size_t GetSize() { return Size; } // возвращает размер списка
    void insert(T data, int index); // добавление элемента по индексу
    void removeAt(int index); // удаление элемента по индексу
    void clear(); // очищение списка
  private:
    size_t Size;
  };
}

int main()
{
  while (!std::cin.eof()) {
    std::string title;
    std::cin >> title;
    std::cout << title << "название";
    //вставить титле в список титлов

    int num;
    while (std::cin >> num) {
      //вставить число в список
      std::cout << num << "число";
    }
  }
}
