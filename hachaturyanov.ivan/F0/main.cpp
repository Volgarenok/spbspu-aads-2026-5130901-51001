#include <iostream>
#include "commands.hpp"

int main()
{
  hachaturyanov::MatrixTable matrices;
  std::string line;

  while (std::getline(std::cin, line)) {
    const hachaturyanov::List< std::string > &tokens = hachaturyanov::tokenize(line);
    if (tokens.isEmpty()) {
      continue;
    }

    auto handler = hachaturyanov::findHandler(*tokens.begin());
    if (handler == nullptr) {
      std::cout << "<INVALID COMMAND>" << '\n';
      continue;
    }
    try {
      handler(tokens, matrices, std::cout);
    } catch (const std::bad_alloc &) {
      std::cout << "<BAD ALLOCATION>" << "\n";
    }
  }

  return 0;
}
