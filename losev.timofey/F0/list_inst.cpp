#include "../common/list.cpp"
#include "types.hpp"

namespace losev {

// Явное инстанцирование для Training
template struct Node<Training>;
template class LIter<Training>;
template class LCIter<Training>;
template class List<Training>;

}
