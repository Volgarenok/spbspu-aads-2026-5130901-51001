#include "CommandHandler.hpp"
#include "KnapsackSolver.hpp"
#include "Helpers.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>

namespace karpenko
{

CommandHandler::CommandHandler(ProfileDB& profiles, BaseDB& bases, MealDiary& diary)
  : profiles_(profiles),
    bases_(bases),
    diary_(diary)
{
}

void CommandHandler::printSuccess(const std::string& msg)
{
  std::cout << "✓ " << msg << std::endl;
}

void CommandHandler::printError(const std::string& msg)
{
  std::cout << "✗ ERROR: " << msg << std::endl;
}

void CommandHandler::printInfo(const std::string& msg)
{
  std::cout << "ℹ " << msg << std::endl;
}

void CommandHandler::handleProfileCreate(const std::string* args, int count)
{
  if (count != 5)
  {
    printError("profile-create requires: <name> <height> <weight> <goal> <activity>");
    return;
  }
  std::string name = args[0];
  double height = std::atof(args[1].c_str());
  double weight = std::atof(args[2].c_str());
  std::string goal = Helpers::toLower(args[3]);
  int activity = std::atoi(args[4].c_str());

  if (height <= 0 || weight <= 0)
  {
    printError("Height and weight must be positive");
    return;
  }
  if (!Helpers::isValidGoal(goal))
  {
    printError("Goal must be weight_loss, maintenance, or mass_gain");
    return;
  }
  if (activity < 1 || activity > 5)
  {
    printError("Activity must be between 1 and 5");
    return;
  }
  if (profiles_.hasProfile(name))
  {
    printError("Profile already exists");
    return;
  }
  Profile profile(name, height, weight, goal, activity);
  profiles_.addProfile(profile);
  printSuccess("Profile \"" + name + "\" created");
}

void CommandHandler::handleProfileList()
{
  List< Profile > profiles = profiles_.getAllProfiles();
  if (profiles.empty())
  {
    printInfo("No profiles found");
    return;
  }
  std::cout << "=== PROFILES ===\n";
  int idx = 1;
  for (auto it = profiles.begin(); it != profiles.end(); ++it)
  {
    std::cout << idx++ << ". " << it->toString() << "\n";
  }
  std::cout << "Total: " << profiles.size() << " profiles\n";
}

void CommandHandler::handleProfileDelete(const std::string* args, int count)
{
  if (count != 1)
  {
    printError("profile-delete requires <name>");
    return;
  }
  std::string name = args[0];
  if (!profiles_.hasProfile(name))
  {
    printError("Profile not found");
    return;
  }
  List< MealEntry > entries = diary_.getEntriesByProfile(name);
  int removed = 0;
  for (auto it = entries.begin(); it != entries.end(); ++it)
  {
    if (diary_.removeEntry(it->getProfileName(), it->getDate(), it->getMealType()))
    {
      ++removed;
    }
  }
  profiles_.removeProfile(name);
  printSuccess("Profile \"" + name + "\" deleted (diary entries removed: " + std::to_string(removed) + ")");
}

void CommandHandler::handleBaseCreate(const std::string* args, int count)
{
  if (count != 1)
  {
    printError("base-create requires <name>");
    return;
  }
  std::string name = args[0];
  if (name.empty())
  {
    printError("Base name cannot be empty");
    return;
  }
  if (bases_.hasBase(name))
  {
    printError("Base already exists");
    return;
  }
  bases_.addBase(FoodBase(name));
  printSuccess("Base \"" + name + "\" created");
}

void CommandHandler::handleBaseList()
{
  List< const FoodBase* > all = bases_.getAllBases();
  if (all.empty())
  {
    printInfo("No bases found");
    return;
  }
  int n = all.size();
  std::string* names = new std::string[n];
  int idx = 0;
  for (auto it = all.begin(); it != all.end(); ++it)
  {
    names[idx++] = (*it)->getName();
  }
  for (int i = 0; i < n - 1; ++i)
  {
    for (int j = 0; j < n - i - 1; ++j)
    {
      if (names[j] > names[j + 1])
      {
        std::swap(names[j], names[j + 1]);
      }
    }
  }
  std::cout << "=== BASES ===\n";
  for (int i = 0; i < n; ++i)
  {
    const FoodBase* base = bases_.getBase(names[i]);
    std::cout << (i + 1) << ". " << base->getName() << " (" << base->size() << " products)\n";
  }
  delete[] names;
  std::cout << "Total: " << n << " bases\n";
}

void CommandHandler::handleBaseAdd(const std::string* args, int count)
{
  if (count != 6)
  {
    printError("base-add requires: <base> <product> <proteins> <fats> <carbs> <calories>");
    return;
  }
  std::string baseName = args[0];
  std::string productName = args[1];
  double proteins = std::atof(args[2].c_str());
  double fats = std::atof(args[3].c_str());
  double carbs = std::atof(args[4].c_str());
  double calories = std::atof(args[5].c_str());
  if (proteins <= 0 || fats < 0 || carbs < 0 || calories <= 0)
  {
    printError("Nutrition values must be positive (fats/carbs can be zero)");
    return;
  }
  FoodBase* base = bases_.getBase(baseName);
  if (!base)
  {
    printError("Base not found");
    return;
  }
  if (base->containsProduct(productName))
  {
    printError("Product already exists in base");
    return;
  }
  Product product(productName, proteins, fats, carbs, calories);
  base->addProduct(product);
  printSuccess("Product \"" + productName + "\" added to base \"" + baseName + "\"");
}

void CommandHandler::handleBaseRemove(const std::string* args, int count)
{
  if (count != 2)
  {
    printError("base-remove requires: <base> <product>");
    return;
  }
  std::string baseName = args[0];
  std::string productName = args[1];
  FoodBase* base = bases_.getBase(baseName);
  if (!base)
  {
    printError("Base not found");
    return;
  }
  if (!base->containsProduct(productName))
  {
    printError("Product not found in base");
    return;
  }
  base->removeProduct(productName);
  printSuccess("Product \"" + productName + "\" removed from base \"" + baseName + "\"");
}

void CommandHandler::handleBaseShow(const std::string* args, int count)
{
  if (count != 1)
  {
    printError("base-show requires <base>");
    return;
  }
  std::string baseName = args[0];
  const FoodBase* base = bases_.getBase(baseName);
  if (!base)
  {
    printError("Base not found");
    return;
  }
  List< Product > products = base->getAllProducts();
  std::cout << "=== BASE: " << baseName << " ===\n";
  if (products.empty())
  {
    std::cout << "No products\n";
  }
  else
  {
    int idx = 1;
    for (auto it = products.begin(); it != products.end(); ++it)
    {
      std::cout << idx++ << ". " << it->toString() << "\n";
    }
  }
  std::cout << "Total: " << products.size() << " products\n";
}

void CommandHandler::handlePlanMeal(const std::string* args, int count)
{
  if (count != 6)
  {
    printError("plan-meal requires: <profile> <date> <meal> <base> <target-cal> <target-protein>");
    return;
  }
  std::string profileName = args[0];
  std::string date = args[1];
  std::string mealType = Helpers::toLower(args[2]);
  std::string baseName = args[3];
  double targetCalories = std::atof(args[4].c_str());
  double targetProtein = std::atof(args[5].c_str());

  if (!Helpers::isValidDate(date))
  {
    printError("Invalid date format (YYYY-MM-DD)");
    return;
  }
  if (!Helpers::isValidMealType(mealType))
  {
    printError("Invalid meal type. Use: breakfast, lunch, dinner, snack");
    return;
  }
  if (targetCalories <= 0 || targetProtein <= 0)
  {
    printError("Targets must be positive");
    return;
  }
  if (!profiles_.hasProfile(profileName))
  {
    printError("Profile not found");
    return;
  }
  FoodBase* base = bases_.getBase(baseName);
  if (!base)
  {
    printError("Base not found");
    return;
  }

  List< MealSolution > solutions = KnapsackSolver::solve(*base, targetCalories, targetProtein);
  if (solutions.empty())
  {
    printError("No suitable combination found (try adding more products)");
    return;
  }

  int optionNumber = 1;
  for (auto it = solutions.begin(); it != solutions.end(); ++it)
  {
    std::cout << "=== OPTION " << optionNumber << " ===\n";
    std::cout << "TOTAL: " << it->total_calories_ << " kcal, " << it->total_protein_ << "g protein\n";
    for (auto itemIt = it->items_.begin(); itemIt != it->items_.end(); ++itemIt)
    {
      std::cout << "- " << itemIt->product_name_ << " (" << itemIt->grams_ << "g) — "
                << itemIt->calories_ << " kcal, " << itemIt->protein_ << "g protein\n";
    }
    ++optionNumber;
  }

  int selectedIndex = 1;

  MealSolution selected;
  int current = 1;
  for (auto it = solutions.begin(); it != solutions.end(); ++it)
  {
    if (current == selectedIndex)
    {
      selected = *it;
      break;
    }
    ++current;
  }

  MealEntry entry(profileName, date, mealType);
  for (auto itemIt = selected.items_.begin(); itemIt != selected.items_.end(); ++itemIt)
  {
    MealItem mealItem(baseName, itemIt->product_name_, itemIt->grams_, itemIt->calories_, itemIt->protein_);
    entry.addItem(mealItem);
  }
  diary_.addEntry(entry);
  printSuccess("Meal added to " + profileName + "'s diary for " + date + " " + mealType);
}

void CommandHandler::handleMealAdd(const std::string* args, int count)
{
  if (count != 6)
  {
    printError("meal-add requires: <profile> <date> <meal> <base> <product> <grams>");
    return;
  }
  std::string profileName = args[0];
  std::string date = args[1];
  std::string mealType = Helpers::toLower(args[2]);
  std::string baseName = args[3];
  std::string productName = args[4];
  double grams = std::atof(args[5].c_str());

  if (!Helpers::isValidDate(date))
  {
    printError("Invalid date format");
    return;
  }
  if (!Helpers::isValidMealType(mealType))
  {
    printError("Invalid meal type");
    return;
  }
  if (grams <= 0)
  {
    printError("Grams must be positive");
    return;
  }
  if (!profiles_.hasProfile(profileName))
  {
    printError("Profile not found");
    return;
  }
  FoodBase* base = bases_.getBase(baseName);
  if (!base)
  {
    printError("Base not found");
    return;
  }
  const Product* product = base->getProduct(productName);
  if (!product)
  {
    printError("Product not found in base");
    return;
  }
  double calories = product->calculateCalories(grams);
  double protein = product->calculateProteins(grams);
  MealItem item(baseName, productName, grams, calories, protein);
  MealEntry entry(profileName, date, mealType);
  entry.addItem(item);
  diary_.addEntry(entry);
  printSuccess(mealType + " added | " + std::to_string(calories) + " kcal, " + std::to_string(protein) + "g protein");
}

void CommandHandler::handleDayStats(const std::string* args, int count)
{
  if (count != 2)
  {
    printError("day-stats requires: <profile> <date>");
    return;
  }
  std::string profileName = args[0];
  std::string date = args[1];
  if (!Helpers::isValidDate(date))
  {
    printError("Invalid date");
    return;
  }
  if (!profiles_.hasProfile(profileName))
  {
    printError("Profile not found");
    return;
  }
  List< MealEntry > entries = diary_.getEntriesByDate(profileName, date);
  const Profile* profile = profiles_.getProfile(profileName);
  double totalCalories = 0;
  double totalProtein = 0;
  std::cout << "=== " << profileName << ": " << date << " ===\n";
  if (entries.empty())
  {
    std::cout << "No meals recorded\n";
  }
  else
  {
    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
      std::cout << it->toString() << "\n";
      totalCalories += it->totalCalories();
      totalProtein += it->totalProtein();
    }
  }
  std::cout << "-------------------\n";
  std::cout << "TOTAL: " << totalCalories << " kcal, " << totalProtein << "g protein\n";
  if (profile)
  {
    std::cout << "\nDaily goal: " << profile->getTargetCalories() << " kcal, "
              << profile->getTargetProtein() << "g protein\n";
    std::cout << "Remaining: " << (profile->getTargetCalories() - totalCalories) << " kcal, "
              << (profile->getTargetProtein() - totalProtein) << "g protein\n";
  }
}

void CommandHandler::handleSave(const std::string* args, int count)
{
  if (count != 1)
  {
    printError("save requires <filename>");
    return;
  }
  std::string filename = args[0];
  if (fileManager_.save(filename, profiles_, bases_, diary_))
  {
    printSuccess("Data saved to " + filename);
  }
  else
  {
    printError("Failed to save data");
  }
}

void CommandHandler::handleLoad(const std::string* args, int count)
{
  if (count != 1)
  {
    printError("load requires <filename>");
    return;
  }
  std::string filename = args[0];
  if (fileManager_.load(filename, profiles_, bases_, diary_))
  {
    printSuccess("Data loaded from " + filename);
  }
  else
  {
    printError("Failed to load data");
  }
}

void CommandHandler::handleHelp(const std::string* args, int count)
{
  if (count == 0)
  {
    std::cout << CommandParser::getHelp();
  }
  else
  {
    std::cout << CommandParser::getHelpForCommand(args[0]);
  }
}

void CommandHandler::handle(const ParsedCommand& command)
{
  switch (command.type)
  {
  case CMD_PROFILE_CREATE:
    handleProfileCreate(command.args, command.argCount);
    break;
  case CMD_PROFILE_LIST:
    handleProfileList();
    break;
  case CMD_PROFILE_DELETE:
    handleProfileDelete(command.args, command.argCount);
    break;
  case CMD_BASE_CREATE:
    handleBaseCreate(command.args, command.argCount);
    break;
  case CMD_BASE_LIST:
    handleBaseList();
    break;
  case CMD_BASE_ADD:
    handleBaseAdd(command.args, command.argCount);
    break;
  case CMD_BASE_REMOVE:
    handleBaseRemove(command.args, command.argCount);
    break;
  case CMD_BASE_SHOW:
    handleBaseShow(command.args, command.argCount);
    break;
  case CMD_PLAN_MEAL:
    handlePlanMeal(command.args, command.argCount);
    break;
  case CMD_MEAL_ADD:
    handleMealAdd(command.args, command.argCount);
    break;
  case CMD_DAY_STATS:
    handleDayStats(command.args, command.argCount);
    break;
  case CMD_SAVE:
    handleSave(command.args, command.argCount);
    break;
  case CMD_LOAD:
    handleLoad(command.args, command.argCount);
    break;
  case CMD_HELP:
    handleHelp(command.args, command.argCount);
    break;
  case CMD_EXIT:
    break;
  default:
    printError("Unknown command");
    break;
  }
}

}
