#include <iostream>
#include <string>

#include "CommandParser.hpp"
#include "CommandHandler.hpp"
#include "ProfileDB.hpp"
#include "BaseDB.hpp"
#include "MealDiary.hpp"

int main()
{
  std::cout << "=== SMART MEAL TRACKER ===\nType 'help' for commands, 'exit' to quit.\n\n";

  karpenko::ProfileDB profiles;
  karpenko::BaseDB bases;
  karpenko::MealDiary diary;
  karpenko::CommandParser parser;
  karpenko::CommandHandler handler(profiles, bases, diary);

  std::string input;
  while (true) {
    std::cout << "> ";
    if (!std::getline(std::cin, input)) {
      break;
    }
    if (input.empty()) {
      continue;
    }
    const karpenko::ParsedCommand cmd = parser.parse(input);
    if (cmd.type == karpenko::CMD_EXIT) {
      std::cout << "Goodbye!\n";
      break;
    }
    handler.handle(cmd);
  }
  return 0;
}
