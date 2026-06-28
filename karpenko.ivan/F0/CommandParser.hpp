#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <string>

namespace karpenko {

  enum CommandType {
    CMD_PROFILE_CREATE,
    CMD_PROFILE_LIST,
    CMD_PROFILE_DELETE,
    CMD_BASE_CREATE,
    CMD_BASE_LIST,
    CMD_BASE_ADD,
    CMD_BASE_REMOVE,
    CMD_BASE_SHOW,
    CMD_PLAN_MEAL,
    CMD_MEAL_ADD,
    CMD_DAY_STATS,
    CMD_SAVE,
    CMD_LOAD,
    CMD_HELP,
    CMD_EXIT,
    CMD_INVALID
  };

  struct ParsedCommand {
    CommandType type;
    std::string args[10];
    int argCount;
    std::string rawInput;

    ParsedCommand():
      type(CMD_INVALID),
      argCount(0)
    {}
  };

  class CommandParser {
  public:
    CommandParser();

    ParsedCommand parse(const std::string& input);

    static std::string getHelp();
    static std::string getHelpForCommand(const std::string& cmd);

  private:
    CommandType getCommandType(const std::string& cmdName) const;
  };

}

#endif
