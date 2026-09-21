#include <cstdio>
#include <unistd.h>
#include "parser.hpp"

namespace yafu {
  namespace {
    constexpr size_t localArgumentCount = 4;
    constexpr size_t setArgumentCount = 3;

    bool isLocalKeyword(const String& source)
    {
      return source.equalsIgnoreCase(String("local"));
    }

    bool isSpace(char symbol)
    {
      return (symbol == ' ') || (symbol == '\t') || (symbol == '\r') || (symbol == '\n');
    }

    bool fillPositional(const DynArray< String >& arguments, Command& result, bool allowSavePath)
    {
      if (arguments.getSize() < setArgumentCount) {
        std::fputs("yafu: not enough arguments\n", stderr);
        return false;
      }

      result.cmdlet = arguments[0];
      result.source = arguments[1];

      if (!isKnownCmdlet(result.cmdlet)) {
        std::fputs("yafu: unknown cmdlet \"", stderr);
        std::fputs(result.cmdlet.getCString(), stderr);
        std::fputs("\"\n", stderr);
        return false;
      }

      const size_t expected = isLocalKeyword(result.source) ? localArgumentCount : setArgumentCount;
      const size_t allowed = allowSavePath ? (expected + 1) : expected;

      if (arguments.getSize() < expected) {
        std::fputs("yafu: not enough arguments\n", stderr);
        return false;
      }
      if (arguments.getSize() > allowed) {
        std::fputs("yafu: too many arguments\n", stderr);
        return false;
      }

      if (isLocalKeyword(result.source)) {
        result.path = arguments[2];
        result.pattern = arguments[3];
      } else {
        result.pattern = arguments[2];
      }

      if (arguments.getSize() == (expected + 1)) {
        if (!result.save) {
          std::fputs("yafu: save directory requires -s\n", stderr);
          return false;
        }
        result.savePath = arguments[expected];
      }
      return true;
    }

    bool isOptionToken(const char* token)
    {
      return (token != nullptr) && (token[0] == '-') && (token[1] != '\0');
    }

    int findFirstPositional(int argc, char** argv)
    {
      int index = 1;
      while ((index < argc) && isOptionToken(argv[index])) {
        ++index;
      }
      return index;
    }

    bool hasMisplacedOptions(int firstPositional, int argc, char** argv)
    {
      for (int i = firstPositional; i < argc; ++i) {
        if (isOptionToken(argv[i])) {
          std::fputs("yafu: options must be placed before the cmdlet\n", stderr);
          return true;
        }
      }
      return false;
    }

    bool readOptions(int optionCount, char** argv, Command& result)
    {
      opterr = 0;
      int option = getopt(optionCount, argv, "sf");
      while (option != -1) {
        if (option == 's') {
          result.save = true;
        } else if (option == 'f') {
          result.force = true;
        } else {
          std::fputs("yafu: unknown option\n", stderr);
          return false;
        }
        option = getopt(optionCount, argv, "sf");
      }

      if (result.force && !result.save) {
        result.force = false;
      }
      return true;
    }

    bool hasOptions(const DynArray< String >& tokens)
    {
      for (size_t i = 0; i < tokens.getSize(); ++i) {
        if ((tokens[i].getLength() > 0) && (tokens[i][0] == '-')) {
          return true;
        }
      }
      return false;
    }
  }
}

void yafu::splitIntoTokens(const String& line, DynArray< String >& result)
{
  String current;
  for (size_t i = 0; i < line.getLength(); ++i) {
    if (isSpace(line[i])) {
      if (!current.isEmpty()) {
        result.pushBack(current);
        current.clear();
      }
    } else {
      current.pushBack(line[i]);
    }
  }

  if (!current.isEmpty()) {
    result.pushBack(current);
  }
}

bool yafu::parseArguments(int argc, char** argv, Command& result)
{
  result.interactive = false;

  const int firstPositional = findFirstPositional(argc, argv);
  if (hasMisplacedOptions(firstPositional, argc, argv)) {
    return false;
  }
  if (!readOptions(firstPositional, argv, result)) {
    return false;
  }

  DynArray< String > positional;
  for (int i = firstPositional; i < argc; ++i) {
    positional.pushBack(String(argv[i]));
  }
  return fillPositional(positional, result, true);
}

bool yafu::parseInteractiveLine(const String& line, Command& result, bool& isExit)
{
  isExit = false;

  DynArray< String > tokens;
  splitIntoTokens(line, tokens);
  if (tokens.isEmpty()) {
    return false;
  }

  if ((tokens[0] == "exit") || (tokens[0] == "quit")) {
    isExit = true;
    return false;
  }

  if (hasOptions(tokens)) {
    std::fputs("yafu: options are not used in interactive mode\n", stderr);
    return false;
  }

  result.interactive = true;
  return fillPositional(tokens, result, false);
}
