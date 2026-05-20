#include <iostream>
#include <string>

#include "CommandParser.hpp"
#include "CommandHandler.hpp"
#include "ProfileDB.hpp"
#include "BaseDB.hpp"
#include "MealDiary.hpp"

using namespace karpenko;

int main()
{
  std::cout << "=== SMART MEAL TRACKER ===\nType 'help' for commands, 'exit' to quit.\n\n";

  ProfileDB profiles;
  BaseDB bases;
  MealDiary diary;
  CommandParser parser;
  CommandHandler handler(profiles, bases, diary);

  std::string input;
  while (true)
  {
    std::cout << "> ";
    std::getline(std::cin, input);
    if (input.empty())
    {
      continue;
    }
    const ParsedCommand cmd = parser.parse(input);
    if (cmd.type == CMD_EXIT)
    {
      std::cout << "Goodbye!\n";
      break;
    }
    handler.handle(cmd);
  }
  return 0;
}
