#ifndef KARPENKO_COMMAND_HANDLER_HPP
#define KARPENKO_COMMAND_HANDLER_HPP

#include "ProfileDB.hpp"
#include "BaseDB.hpp"
#include "MealDiary.hpp"
#include "CommandParser.hpp"
#include "FileManager.hpp"

namespace karpenko
{

class CommandHandler
{
public:
  CommandHandler(ProfileDB& profiles, BaseDB& bases, MealDiary& diary);

  void handle(const ParsedCommand& cmd);

private:
  ProfileDB& profiles_;
  BaseDB& bases_;
  MealDiary& diary_;
  FileManager fileManager_;

  void handleProfileCreate(const std::string* args, int count);
  void handleProfileList();
  void handleProfileDelete(const std::string* args, int count);
  void handleBaseCreate(const std::string* args, int count);
  void handleBaseList();
  void handleBaseAdd(const std::string* args, int count);
  void handleBaseRemove(const std::string* args, int count);
  void handleBaseShow(const std::string* args, int count);
  void handlePlanMeal(const std::string* args, int count);
  void handleMealAdd(const std::string* args, int count);
  void handleDayStats(const std::string* args, int count);
  void handleSave(const std::string* args, int count);
  void handleLoad(const std::string* args, int count);
  void handleHelp(const std::string* args, int count);

  void printSuccess(const std::string& msg);
  void printError(const std::string& msg);
  void printInfo(const std::string& msg);
};

}

#endif
