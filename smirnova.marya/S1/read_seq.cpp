#include "read_seq.hpp"
#include <sstream>

namespace smirnova
{
  List< Seq > readSequences(std::istream& in)
  {
    List< Seq > sequences;
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) {
        continue;
      }
      std::istringstream iss(line);
      std::string name;
      if (!(iss >> name)) {
        continue;
      }
      List< size_t > numbers;
      size_t num = 0;
      while (iss >> num) {
        numbers.pushBack(num);
      }
      sequences.pushBack(Seq{std::move(name), std::move(numbers)});
    }
    return sequences;
  }
}

