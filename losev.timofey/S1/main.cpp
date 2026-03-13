#include <iostream>
#include <string>
#include <limits>
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

bool add_checked(size_t a, size_t b, size_t& result) {
  if (b > std::numeric_limits<size_t>::max() - a) {
    return false;
  }
  result = a + b;
  return true;
}

void skip_spaces(std::istream& in) {
  while (in.peek() == ' ' || in.peek() == '\t') {
    in.get();
  }
}

bool is_line_end(std::istream& in) {
  int c = in.peek();
  return c == '\n' || c == '\r' || c == EOF;
}

void skip_line(std::istream& in) {
  while (!is_line_end(in)) {
    in.get();
  }
  if (in.peek() == '\r') {
    in.get();
  }
  if (in.peek() == '\n') {
    in.get();
  }
}

}

int main() {
  using namespace losev;

  List< NamedList > sequences;
  std::string name;
  bool input_overflow = false;

  while (std::cin >> name) {
    NamedList seq;
    seq.name = name;
    skip_spaces(std::cin);

    while (!is_line_end(std::cin)) {
      size_t num = 0;
      if (!(std::cin >> num)) {
        input_overflow = true;
        std::cin.clear();
        skip_line(std::cin);
        break;
      }
      seq.numbers.push_front(num);
      skip_spaces(std::cin);
    }
    skip_line(std::cin);
    seq.numbers = reverse(seq.numbers);
    sequences.push_front(seq);
  }

  sequences = reverse(sequences);

  if (sequences.empty()) {
    std::cout << "0\n";
    return 0;
  }

  size_t max_len = 0;
  for (List< NamedList >::iterator seq_it = sequences.begin(); seq_it != sequences.end(); ++seq_it) {
    size_t len = length(seq_it->numbers);
    if (len > max_len) max_len = len;
  }

  bool has_overflow = input_overflow;

  if (!has_overflow) {
    for (size_t i = 0; i < max_len && !has_overflow; ++i) {
      size_t sum = 0;
      for (List< NamedList >::iterator seq_it = sequences.begin(); seq_it != sequences.end() && !has_overflow; ++seq_it) {
        size_t len = length(seq_it->numbers);
        if (i < len) {
          size_t val = get_element_at(seq_it->numbers, i);
          size_t new_sum;
          if (!add_checked(sum, val, new_sum)) {
            has_overflow = true;
          } else {
            sum = new_sum;
          }
        }
      }
    }
  }

  if (has_overflow) {
    std::cerr << "overflow\n";
    return 1;
  }

  bool first = true;
  for (List< NamedList >::iterator it = sequences.begin(); it != sequences.end(); ++it) {
    if (!first) std::cout << " ";
    std::cout << it->name;
    first = false;
  }
  std::cout << "\n";

  for (size_t i = 0; i < max_len; ++i) {
    first = true;
    for (List< NamedList >::iterator seq_it = sequences.begin(); seq_it != sequences.end(); ++seq_it) {
      size_t len = length(seq_it->numbers);
      if (i < len) {
        if (!first) std::cout << " ";
        std::cout << get_element_at(seq_it->numbers, i);
        first = false;
      }
    }
    std::cout << "\n";
  }

  first = true;
  for (size_t i = 0; i < max_len; ++i) {
    size_t sum = 0;
    for (List< NamedList >::iterator seq_it = sequences.begin(); seq_it != sequences.end(); ++seq_it) {
      size_t len = length(seq_it->numbers);
      if (i < len) {
        sum += get_element_at(seq_it->numbers, i);
      }
    }
    if (!first) std::cout << " ";
    std::cout << sum;
    first = false;
  }
  std::cout << "\n";

  return 0;
}
