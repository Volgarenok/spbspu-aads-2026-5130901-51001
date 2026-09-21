#include <cstdio>
#include "repl.hpp"
#include "command.hpp"
#include "parser.hpp"

namespace yafu {
  namespace {
    constexpr size_t inputBufferSize = 4096;
  }
}

int yafu::runInteractive()
{
  char buffer[inputBufferSize] = {0};

  while (true) {
    std::fputs("yafu> ", stdout);
    std::fflush(stdout);

    if (std::fgets(buffer, static_cast< int >(inputBufferSize), stdin) == nullptr) {
      std::fputc('\n', stdout);
      break;
    }

    const String line(buffer);
    Command command;
    bool isExit = false;
    if (parseInteractiveLine(line, command, isExit)) {
      executeCommand(command);
    } else if (isExit) {
      break;
    }
  }

  return 0;
}
