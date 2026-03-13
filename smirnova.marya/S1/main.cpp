#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <climits>
#include <cstdlib>
#include <cstdint>

namespace smirnova {

template <class T>
struct Node {
    T data;
    Node* next;
    Node* prev;
    Node(const T& d) : data(d), next(nullptr), prev(nullptr) {}
    Node(T&& d) : data(std::move(d)), next(nullptr), prev(nullptr) {}
};

template <class T> class LIter;
template <class T> class LCIter;

template <class T>
class List {
private:
    Node<T>* fake;
    size_t sz;

public:
    List() : sz(0) {
        fake = new Node<T>(T());
        fake->next = fake;
        fake->prev = fake;
    }

    // Очистка списка — объявлена ДО деструктора
    void clear() noexcept {
        Node<T>* curr = fake->next;
        while (curr != fake) {
            Node<T>* tmp = curr->next;
            delete curr;
            curr = tmp;
        }
        fake->next = fake;
        fake->prev = fake;
        sz = 0;
    }

    ~List() {
        clear();  // Безопасно вызывается
        delete fake;
    }

    // Конструктор копирования
    List(const List& other) : List() {
        for (LCIter<T> it = other.cbegin(); it.valid(); it.next())
            push_back(it.value());
    }

    // Конструктор перемещения
    List(List&& other) noexcept : fake(other.fake), sz(other.sz) {
        other.fake = new Node<T>(T());
        other.fake->next = other.fake;
        other.fake->prev = other.fake;
        other.sz = 0;
    }

    // Копирующее присваивание
    List& operator=(const List& other) {
        if (this != &other) {
            List tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // Перемещающее присваивание
    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();
            delete fake;
            fake = other.fake;
            sz = other.sz;
            other.fake = new Node<T>(T());
            other.fake->next = other.fake;
            other.fake->prev = other.fake;
            other.sz = 0;
        }
        return *this;
    }

    // Обмен списками
    void swap(List& other) noexcept {
        std::swap(fake, other.fake);
        std::swap(sz, other.sz);
    }

    bool empty() const noexcept { return sz == 0; }
    size_t size() const noexcept { return sz; }

    void push_back(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->next = fake;
        n->prev = fake->prev;
        fake->prev->next = n;
        fake->prev = n;
        ++sz;
    }

    void push_back(T&& val) {
        Node<T>* n = new Node<T>(std::move(val));
        n->next = fake;
        n->prev = fake->prev;
        fake->prev->next = n;
        fake->prev = n;
        ++sz;
    }

    void push_front(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->prev = fake;
        n->next = fake->next;
        fake->next->prev = n;
        fake->next = n;
        ++sz;
    }

    void push_front(T&& val) {
        Node<T>* n = new Node<T>(std::move(val));
        n->prev = fake;
        n->next = fake->next;
        fake->next->prev = n;
        fake->next = n;
        ++sz;
    }

    void pop_front() {
        if (empty()) throw std::out_of_range("pop_front on empty list");
        Node<T>* tmp = fake->next;
        fake->next = tmp->next;
        tmp->next->prev = fake;
        delete tmp;
        --sz;
    }

    void pop_back() {
        if (empty()) throw std::out_of_range("pop_back on empty list");
        Node<T>* tail = fake->prev;
        fake->prev = tail->prev;
        tail->prev->next = fake;
        delete tail;
        --sz;
    }

    // Итераторы
    LIter<T> begin() { return LIter<T>(fake->next, fake); }
    LIter<T> end() { return LIter<T>(fake, fake); }
    LCIter<T> begin() const { return LCIter<T>(fake->next, fake); }
    LCIter<T> end() const { return LCIter<T>(fake, fake); }
    LCIter<T> cbegin() const { return LCIter<T>(fake->next, fake); }
    LCIter<T> cend() const { return LCIter<T>(fake, fake); }
};

template <class T>
class LCIter {
private:
    const Node<T>* node;
    const Node<T>* fake;
public:
    LCIter(const Node<T>* n = nullptr, const Node<T>* f = nullptr) : node(n), fake(f) {}
    bool valid() const { return node != fake; }
    void next() { if(node) node = node->next; }
    const T& value() const { return node->data; }
};

// Получение элемента по индексу
template <typename T>
LCIter<T> getElementAt(const List<T>& list, size_t index) {
    LCIter<T> it = list.cbegin();
    size_t i = 0;
    while(it.valid() && i < index) { it.next(); ++i; }
    return it;
}

// Печать списка чисел
template <typename T>
void printList(const List<T>& list) {
    if(list.empty()) { std::cout << "0\n"; return; }
    bool first = true;
    for(LCIter<T> it = list.cbegin(); it.valid(); it.next()) {
        if(!first) std::cout << " ";
        std::cout << it.value();
        first = false;
    }
    std::cout << "\n";
}

// Проверка переполнения при сложении
bool sumWillOverflow(int a, int b) {
    if (b > 0 && a > INT_MAX - b) return true;
    if (b < 0 && a < INT_MIN - b) return true;
    return false;
}

// Обработка последовательностей
bool processSequences(const List<std::pair<std::string, List<int>>>& seq) {
    if(seq.empty()) return true;

    size_t maxSize = 0;
    for(LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next())
        if(it.value().second.size() > maxSize) maxSize = it.value().second.size();

    // Вывод чисел "по строкам"
    for(size_t row = 0; row < maxSize; ++row) {
        bool firstInRow = true;
        for(LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
          if (row < it.value().second.size()) { // Если элемент существует
            LCIter<int> numIt = getElementAt(it.value().second, row);
            if (!firstInRow) std::cout << " ";
            std::cout << numIt.value();
            firstInRow = false;
          }
        }
        if(!firstInRow) std::cout << "\n";
    }

    // Вычисление суммы с проверкой переполнения
    List<int> sums;
    for(size_t row = 0; row < maxSize; ++row) {
        int sum = 0;
        for(LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
            if(row < it.value().second.size()) {
                int val = getElementAt(it.value().second, row).value();
                if(sumWillOverflow(sum, val)) {
                    std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
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

} // namespace smirnova

int main() {
    using namespace smirnova;

    List<std::pair<std::string, List<int>>> sequences;
    std::string line;
    bool overflowOccurred = false;

    while(std::getline(std::cin, line)) {
      if(line.empty()) continue;

      std::istringstream iss(line);
      std::string name;
      iss >> name;

      List<int> numbers;
      std::string token;
      while(iss >> token) {
          try {
              long long val = std::stoll(token);
              if(val > INT_MAX || val < INT_MIN) {
                  overflowOccurred = true;  // переполнение при чтении
              } else {
                  numbers.push_back(static_cast<int>(val));
              }
          } catch(...) {
              overflowOccurred = true;
          }
      }
      sequences.push_back({name, std::move(numbers)});
  }

  // Выводим имена
  bool first = true;
  for(LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next()) {
      if(!first) std::cout << " ";
      std::cout << it.value().first;
      first = false;
  }
  std::cout << "\n";

  // Обработка пустого списка
  if(sequences.empty()) {
      std::cout << "0\n";
      return 0;
  }

  // Проверка переполнения после чтения чисел
  if(overflowOccurred) {
      std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
      return 1;
  }

  // Обработка чисел по строкам с проверкой переполнения при сумме
  if(!processSequences(sequences)) return 1;

  return 0;
}
