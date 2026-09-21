#ifndef YAFU_COMMAND_HPP
#define YAFU_COMMAND_HPP

#include "string.hpp"

namespace yafu {
  struct Command
  {
    String cmdlet;
    String source;
    String path;
    String pattern;
    String savePath;
    bool save = false;
    bool force = false;
    bool interactive = false;
  };

  bool isKnownCmdlet(const String& name);
  int executeCommand(const Command& command);
}

#endif
