#include "CommandParser.hpp"

#include <sstream>

#include "Helpers.hpp"

namespace karpenko {

  static const char* commandNames[] = {
    "profile-create", "profile-list", "profile-delete",
    "base-create", "base-list", "base-add", "base-remove", "base-show",
    "plan-meal", "meal-add", "day-stats", "save", "load",
    "help", "exit", nullptr
  };

  static const CommandType commandTypes[] = {
    CMD_PROFILE_CREATE, CMD_PROFILE_LIST, CMD_PROFILE_DELETE,
    CMD_BASE_CREATE, CMD_BASE_LIST, CMD_BASE_ADD, CMD_BASE_REMOVE, CMD_BASE_SHOW,
    CMD_PLAN_MEAL, CMD_MEAL_ADD, CMD_DAY_STATS, CMD_SAVE, CMD_LOAD,
    CMD_HELP, CMD_EXIT, CMD_INVALID
  };

  CommandParser::CommandParser()
  {}

  CommandType CommandParser::getCommandType(const std::string& cmdName) const
  {
    for (int i = 0; commandNames[i] != nullptr; ++i) {
      if (cmdName == commandNames[i]) {
        return commandTypes[i];
      }
    }
    return CMD_INVALID;
  }

  ParsedCommand CommandParser::parse(const std::string& input)
  {
    ParsedCommand result;
    result.rawInput = input;
    std::string trimmed = Helpers::trim(input);
    if (trimmed.empty()) {
      return result;
    }

    std::stringstream stream(trimmed);
    std::string token;
    if (!(stream >> token)) {
      return result;
    }
    result.type = getCommandType(Helpers::toLower(token));
    if (result.type == CMD_INVALID) {
      return result;
    }

    result.argCount = 0;
    while (stream >> token && result.argCount < 10) {
      result.args[result.argCount++] = token;
    }
    return result;
  }

  std::string CommandParser::getHelp()
  {
    return
      "=== SMART MEAL TRACKER - COMMANDS ===\n"
      "\n"
      "PROFILES:\n"
      "  profile-create <name> <height> <weight> <goal> <activity>\n"
      "  profile-list\n"
      "  profile-delete <name>\n"
      "\n"
      "BASES:\n"
      "  base-create <name>\n"
      "  base-list\n"
      "  base-add <base> <product> <proteins> <fats> <carbs> <calories>\n"
      "  base-remove <base> <product>\n"
      "  base-show <base>\n"
      "\n"
      "MEALS:\n"
      "  plan-meal <profile> <date> <meal> <base> <target-cal> <target-protein>\n"
      "  meal-add <profile> <date> <meal> <base> <product> <grams>\n"
      "  day-stats <profile> <date>\n"
      "\n"
      "SYSTEM:\n"
      "  save <file>\n"
      "  load <file>\n"
      "  help [command]\n"
      "  exit\n";
  }

  std::string CommandParser::getHelpForCommand(const std::string& cmd)
  {
    std::string lowerCommand = Helpers::toLower(cmd);
    if (lowerCommand == "profile-create") {
      return "profile-create <name> <height> <weight> <goal> <activity>\n"
             "  Create a new profile. Goal: weight_loss, maintenance, mass_gain.\n"
             "  Activity: 1(sedentary) to 5(very active).\n"
             "  Example: profile-create John 180 75 mass_gain 3";
    }
    if (lowerCommand == "plan-meal") {
      return "plan-meal <profile> <date> <meal> <base> <target-cal> <target-protein>\n"
             "  Find best meal combination using knapsack algorithm.\n"
             "  Example: plan-meal John 2025-04-08 dinner my_products 700 35";
    }
    if (lowerCommand == "meal-add") {
      return "meal-add <profile> <date> <meal> <base> <product> <grams>\n"
             "  Add a meal entry manually.\n"
             "  Example: meal-add John 2025-04-08 breakfast my_products chicken 150";
    }
    if (lowerCommand == "day-stats") {
      return "day-stats <profile> <date>\n"
             "  Show daily statistics for a profile.\n"
             "  Example: day-stats John 2025-04-08";
    }
    if (lowerCommand == "save") {
      return "save <file>\n  Save current session to file.";
    }
    if (lowerCommand == "load") {
      return "load <file>\n  Load data from file (merges with current).";
    }
    return getHelp();
  }

}
