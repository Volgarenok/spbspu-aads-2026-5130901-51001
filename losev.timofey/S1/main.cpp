#include <iostream>
#include <string>
#include <climits>
#include <cerrno>
#include <cstdlib>
#include "list.h"

namespace losev {

template< typename T >
List< T > reverse(const List< T >& list) {
  List< T > result;
  for (typename List< T >::const_iterator it = list.begin(); it != list.end(); ++it) {
    result.push_front(*it);
  }
  return result;
}

template< typename T >
size_t length(const List< T >& list) {
  size_t len = 0;
  for (typename List< T >::const_iterator it = list.begin(); it != list.end(); ++it) {
    ++len;
  }
  return len;
}

template< typename T >
T get_element_at(const List< T >& list, size_t index) {
  typename List< T >::const_iterator it = list.begin();
  for (size_t i = 0; i < index; ++i) {
    ++it;
  }
  return *it;
}

bool read_number(const std::string& token, unsigned long long& result) {
  if (token.empty()) return false;

  char* endptr;
  errno = 0;
  result = strtoull(token.c_str(), &endptr, 10);

  if (errno == ERANGE || *endptr != '\0') {
    return false;
  }
  return true;
}

}
int main() {
  using namespace losev;

  List< NamedList > sequences;
  std::string name;

  while (std::cin >> name) {
    NamedList seq;
    seq.name = name;

    int num;
    while (std::cin.peek() != '\n' && std::cin.peek() != EOF) {
      if (!(std::cin >> num)) {
        std::cin.clear();
        std::string token;
        std::cin >> token;
        unsigned long long big_num;
        if (!read_number(token, big_num)) {
          std::cerr << "overflow" << std::endl;
          std::exit(1);
        }
        std::cerr << "overflow" << "\n";
        std::exit(1);
      }
      seq.numbers.push_front(num);
    }

    if (std::cin.peek() == '\n') {
      std::cin.ignore();
    }

    seq.numbers = reverse(seq.numbers);
    sequences.push_front(seq);
  }
  sequences = reverse(sequences);
  if (sequences.empty()) {
    std::cout << "0" << "\n";
    return 0;
  }
  bool first = true;
  for (List< NamedList >::iterator it = sequences.begin(); it != sequences.end(); ++it) {
    if (!first) std::cout << " ";
    std::cout << it->name;
    first = false;
  }
  std::cout << "\n";
  size_t max_len = 0;
  for (List< NamedList >::iterator seq_it = sequences.begin(); seq_it != sequences.end(); ++seq_it) {
    size_t len = length(seq_it->numbers);
    if (len > max_len) max_len = len;
  }
  if (max_len == 0) {
    std::cout << "0" << "\n";
    return 0;
  }
  List< int > row_sums;
  for (size_t i = 0; i < max_len; ++i) {
    row_sums.push_front(0);
  }
  row_sums = reverse(row_sums);
  for (size_t i = 0; i < max_len; ++i) {
    first = true;
    List< int >::iterator sum_it = row_sums.begin();
    for (size_t pos = 0; pos < i; ++pos) ++sum_it;
    for (List< NamedList >::iterator seq_it = sequences.begin(); seq_it != sequences.end(); ++seq_it) {
      size_t len = length(seq_it->numbers);
      if (i < len) {
        int value = get_element_at(seq_it->numbers, i);
        if (!first) std::cout << " ";
        std::cout << value;
        first = false;
        *sum_it += value;
      }
    }
    std::cout << "\n";
  }
  first = true;
  for (List< int >::iterator sum_it = row_sums.begin(); sum_it != row_sums.end(); ++sum_it) {
    if (!first) std::cout << " ";
    std::cout << *sum_it;
    first = false;
  }
  std::cout << "\n";

  return 0;
}
