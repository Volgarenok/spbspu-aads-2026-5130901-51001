#include "seq.hpp"
#include "read_seq.hpp"
#include "print_seq.hpp"

int main() {
  using namespace smirnova;
  List<Seq> sequences = readSequences(std::cin);
  if (!processSequences(sequences)) {
    return 1;
  }
  return 0;
}

