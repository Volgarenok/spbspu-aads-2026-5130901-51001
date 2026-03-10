#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <stdexcept>
#include <climits>
#include <limits>

namespace smirnova {

template <class T>
struct Node {
    T data;
    Node* next;
    Node* prev;

    Node(const T& d) : data(d), next(nullptr), prev(nullptr) {}
    Node(T&& d) : data(std::move(d)), next(nullptr), prev(nullptr) {}
};

template <class T>
class LIter;

template <class T>
class LCIter;

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

    ~List() {
        clear();
        delete fake;
    }

    List(const List& other) : List() {
        for (LCIter<T> it = other.cbegin(); it.valid(); it.next())
            push_back(it.value());
    }

    List(List&& other) noexcept : fake(other.fake), sz(other.sz) {
        other.fake = new Node<T>(T());
        other.fake->next = other.fake;
        other.fake->prev = other.fake;
        other.sz = 0;
    }

    List& operator=(const List& other) {
        if (this != &other) {
            List tmp(other);
            swap(tmp);
        }
        return *this;
    }

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

    LIter<T> begin() { return LIter<T>(fake->next, fake); }
    LIter<T> end() { return LIter<T>(fake, fake); }
    LCIter<T> begin() const { return LCIter<T>(fake->next, fake); }
    LCIter<T> end() const { return LCIter<T>(fake, fake); }
    LCIter<T> cbegin() const { return LCIter<T>(fake->next, fake); }
    LCIter<T> cend() const { return LCIter<T>(fake, fake); }
};

// Итератор для изменения элементов
template <class T>
class LIter {
private:
    Node<T>* node;
    Node<T>* fake;

public:
    LIter(Node<T>* n = nullptr, Node<T>* f = nullptr) : node(n), fake(f) {}

    bool valid() const { return node != fake; }
    void next() { if (node) node = node->next; }
    void prev() { if (node) node = node->prev; }
    T& value() { return node->data; }
};

// Константный итератор
template <class T>
class LCIter {
private:
    const Node<T>* node;
    const Node<T>* fake;

public:
    LCIter(const Node<T>* n = nullptr, const Node<T>* f = nullptr) : node(n), fake(f) {}
    LCIter(const LIter<T>& iter) : node(iter.node), fake(iter.fake) {}

    bool valid() const { return node != fake; }
    void next() { if (node) node = node->next; }
    void prev() { if (node) node = node->prev; }
    const T& value() const { return node->data; }
};

// Вспомогательная функция для вывода списка
template <typename T>
void printList(const List<T>& list) {
    if (list.empty()) {
        std::cout << "0\n";
        return;
    }

    bool first = true;
    for (LCIter<T> it = list.cbegin(); it.valid(); it.next()) {
        if (!first) std::cout << " ";
        std::cout << it.value();
        first = false;
    }
    std::cout << "\n";
}

// Получение элемента по индексу
template <typename T>
LCIter<T> getElementAt(const List<T>& list, size_t index) {
    LCIter<T> it = list.cbegin();
    size_t i = 0;
    while (it.valid() && i < index) { it.next(); ++i; }
    return it;
}

// Обработка последовательностей
void processSequences(List<std::pair<std::string, List<int>>>& seq) {
    if (seq.empty()) return;

    size_t maxSize = 0;
    for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
        if (it.value().second.size() > maxSize)
            maxSize = it.value().second.size();
    }
    if (maxSize == 0) {
    std::cout << "0\n";
    return;
  }
    // Вывод "по строкам"
    for (size_t row = 0; row < maxSize; ++row) {
        bool firstInRow = true;
        for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
            if (row < it.value().second.size()) {
                LCIter<int> numIt = getElementAt(it.value().second, row);
                if (!firstInRow) std::cout << " ";
                std::cout << numIt.value();
                firstInRow = false;
            }
        }
        if (!firstInRow) std::cout << "\n";
    }

    // Вычисление суммы "по строкам"
    List<int> sums;
    bool sumPossible = true;

    for (size_t row = 0; row < maxSize && sumPossible; ++row) {
        long long sum = 0;
        for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid() && sumPossible; it.next()) {
            if (row < it.value().second.size()) {
                LCIter<int> numIt = getElementAt(it.value().second, row);
                if (numIt.value() > 0 && sum > std::numeric_limits<long long>::max() - numIt.value()) {
                    sumPossible = false;
                } else {
                    sum += numIt.value();
                }
            }
        }
        if (sumPossible) {
            if (sum > std::numeric_limits<int>::max()) sumPossible = false;
            else sums.push_back(static_cast<int>(sum));
        }
    }

    if (sumPossible && !sums.empty()) printList(sums);
    else if (!sumPossible) throw std::overflow_error("overflow");
}

} // namespace smirnova

int main() {
    using namespace smirnova;

    List<std::pair<std::string, List<int>>> sequences;
    std::string line;

    while (std::getline(std::cin, line)) {
      if (line.empty()) continue;

      std::istringstream iss(line);
      std::string name;
      iss >> name;

      List<int> numbers;

      try {
          int x;
          while (iss >> x) {
              if (x < 1000000) {
                  numbers.push_back(x);
              }
          }

          if (!iss.eof()) {
              throw std::out_of_range("number exceeds int range");
          }

          sequences.push_back(std::make_pair(name, std::move(numbers)));
      }
      catch (const std::out_of_range&) {
          throw;
      }
  }



    try {
        while (std::getline(std::cin, line)) {
          if (line.empty()) continue;
          std::istringstream iss(line);
          std::string name;
          iss >> name;

          List<int> numbers;
          try {
            int x;
            while (iss >> x) {
                if (x < INT_MAX) {
                  numbers.push_back(x);
                }
            }

            if (!iss.eof()) {
                throw std::out_of_range("number exceeds int range");
            }

            sequences.push_back(std::make_pair(name, std::move(numbers)));
          } catch (const std::out_of_range&) {
              throw;
          }
        }

        if (sequences.empty()) {
            std::cout << "0\n";
            return 0;
        }

        // Вывод названий
        bool first = true;
        for (LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next()) {
            if (!first) std::cout << " ";
            std::cout << it.value().first;
            first = false;
        }
        std::cout << "\n";

        processSequences(sequences);

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return 1;
    }

    return 0;
}
