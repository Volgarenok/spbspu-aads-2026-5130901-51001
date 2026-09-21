#include "command.hpp"
#include "parser.hpp"
#include "repl.hpp"

int main(int argc, char** argv)
{
  if (argc == 1) {
    return yafu::runInteractive();
  }

  yafu::Command command;
  if (!yafu::parseArguments(argc, argv, command)) {
    return 1;
  }

  return yafu::executeCommand(command);
}
