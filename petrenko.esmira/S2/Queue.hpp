#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "../S1/list.hpp"

namespace petrenko {
  template< class T >
  struct Queue {
  private:
    List< T > data;
  public:
    void push(const T& rhs) {
      data.insert(rhs, data.getSize());
    }

    bool empty() const {
      return data.getSize() == 0;
    }

    T drop() {
      T value = data[0];
      data.removeAt(0);
      return value;
    }

    const T& front() const {
      return data[0];
    }

    const T& back() const {
      return data[data.getSize() - 1];
    }

    size_t size() const {
      return data.getSize();
    }

    void clear() {
      data.clear();
    }
  };
}
#endif
