#include "CommandHandler.hpp"
#include "KnapsackSolver.hpp"
#include "Helpers.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>

namespace karpenko {

CommandHandler::CommandHandler(ProfileDB& profiles, BaseDB& bases, MealDiary& diary)
    : profiles_(profiles), bases_(bases), diary_(diary) {}

void CommandHandler::printSuccess(const std::string& msg) {
    std::cout << "✓ " << msg << std::endl;
}
void CommandHandler::printError(const std::string& msg) {
    std::cout << "✗ ERROR: " << msg << std::endl;
}
void CommandHandler::printInfo(const std::string& msg) {
    std::cout << "ℹ " << msg << std::endl;
}

void CommandHandler::handleProfileCreate(const std::string* args, int count) {
    if (count != 5) {
        printError("profile-create requires: <name> <height> <weight> <goal> <activity>");
        return;
    }
    std::string name = args[0];
    double height = std::atof(args[1].c_str());
    double weight = std::atof(args[2].c_str());
    std::string goal = Helpers::toLower(args[3]);
    int activity = std::atoi(args[4].c_str());

    if (height <= 0 || weight <= 0) {
        printError("Height and weight must be positive");
        return;
    }
    if (!Helpers::isValidGoal(goal)) {
        printError("Goal must be weight_loss, maintenance, or mass_gain");
        return;
    }
    if (activity < 1 || activity > 5) {
        printError("Activity must be between 1 and 5");
        return;
    }
    if (profiles_.hasProfile(name)) {
        printError("Profile already exists");
        return;
    }
    Profile p(name, height, weight, goal, activity);
    profiles_.addProfile(p);
    printSuccess("Profile \"" + name + "\" created");
}

void CommandHandler::handleProfileList() {
    List<Profile> profs = profiles_.getAllProfiles();
    if (profs.size() == 0) {
        printInfo("No profiles found");
        return;
    }
    std::cout << "=== PROFILES ===\n";
    int idx = 1;
    for (auto it = profs.begin(); it != profs.end(); ++it) {
        std::cout << idx++ << ". " << it->toString() << "\n";
    }
    std::cout << "Total: " << profs.size() << " profiles\n";
}

void CommandHandler::handleProfileDelete(const std::string* args, int count) {
    if (count != 1) {
        printError("profile-delete requires <name>");
        return;
    }
    std::string name = args[0];
    if (!profiles_.hasProfile(name)) {
        printError("Profile not found");
        return;
    }
    List<MealEntry> entries = diary_.getEntriesByProfile(name);
    int removed = 0;
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if (diary_.removeEntry(it->getProfileName(), it->getDate(), it->getMealType()))
            ++removed;
    }
    profiles_.removeProfile(name);
    printSuccess("Profile \"" + name + "\" deleted (diary entries removed: " + std::to_string(removed) + ")");
}

void CommandHandler::handleBaseCreate(const std::string* args, int count) {
    if (count != 1) {
        printError("base-create requires <name>");
        return;
    }
    std::string name = args[0];
    if (name.empty()) {
        printError("Base name cannot be empty");
        return;
    }
    if (bases_.hasBase(name)) {
        printError("Base already exists");
        return;
    }
    bases_.addBase(FoodBase(name));
    printSuccess("Base \"" + name + "\" created");
}

void CommandHandler::handleBaseList() {
    List<const FoodBase*> all = bases_.getAllBases();
    if (all.size() == 0) {
        printInfo("No bases found");
        return;
    }
    int n = all.size();
    std::string* names = new std::string[n];
    int idx = 0;
    for (auto it = all.begin(); it != all.end(); ++it) {
        names[idx++] = (*it)->getName();
    }
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            if (names[j] > names[j+1]) {
                std::swap(names[j], names[j+1]);
            }
        }
    }
    std::cout << "=== BASES ===\n";
    for (int i = 0; i < n; ++i) {
        const FoodBase* base = bases_.getBase(names[i]);
        std::cout << (i+1) << ". " << base->getName() << " (" << base->size() << " products)\n";
    }
    delete[] names;
    std::cout << "Total: " << n << " bases\n";
}

void CommandHandler::handleBaseAdd(const std::string* args, int count) {
    if (count != 6) {
        printError("base-add requires: <base> <product> <proteins> <fats> <carbs> <calories>");
        return;
    }
    std::string baseName = args[0];
    std::string prodName = args[1];
    double p = std::atof(args[2].c_str());
    double f = std::atof(args[3].c_str());
    double c = std::atof(args[4].c_str());
    double cal = std::atof(args[5].c_str());
    if (p <= 0 || f < 0 || c < 0 || cal <= 0) {
        printError("Nutrition values must be positive (fats/carbs can be zero)");
        return;
    }
    FoodBase* base = bases_.getBase(baseName);
    if (!base) {
        printError("Base not found");
        return;
    }
    if (base->containsProduct(prodName)) {
        printError("Product already exists in base");
        return;
    }
    Product product(prodName, p, f, c, cal);
    base->addProduct(product);
    printSuccess("Product \"" + prodName + "\" added to base \"" + baseName + "\"");
}

void CommandHandler::handleBaseRemove(const std::string* args, int count) {
    if (count != 2) {
        printError("base-remove requires: <base> <product>");
        return;
    }
    std::string baseName = args[0];
    std::string prodName = args[1];
    FoodBase* base = bases_.getBase(baseName);
    if (!base) {
        printError("Base not found");
        return;
    }
    if (!base->containsProduct(prodName)) {
        printError("Product not found in base");
        return;
    }
    base->removeProduct(prodName);
    printSuccess("Product \"" + prodName + "\" removed from base \"" + baseName + "\"");
}

void CommandHandler::handleBaseShow(const std::string* args, int count) {
    if (count != 1) {
        printError("base-show requires <base>");
        return;
    }
    std::string baseName = args[0];
    const FoodBase* base = bases_.getBase(baseName);
    if (!base) {
        printError("Base not found");
        return;
    }
    List<Product> prods = base->getAllProducts();
    std::cout << "=== BASE: " << baseName << " ===\n";
    if (prods.size() == 0) {
        std::cout << "No products\n";
    } else {
        int idx = 1;
        for (auto it = prods.begin(); it != prods.end(); ++it) {
            std::cout << idx++ << ". " << it->toString() << "\n";
        }
    }
    std::cout << "Total: " << prods.size() << " products\n";
}
void CommandHandler::handlePlanMeal(const std::string* args, int count) {
    if (count != 6) {
        printError("plan-meal requires: <profile> <date> <meal> <base> <target-cal> <target-protein>");
        return;
    }
    std::string profileName = args[0];
    std::string date = args[1];
    std::string mealType = Helpers::toLower(args[2]);
    std::string baseName = args[3];
    double targetCal = std::atof(args[4].c_str());
    double targetProt = std::atof(args[5].c_str());

    if (!Helpers::isValidDate(date)) {
        printError("Invalid date format (YYYY-MM-DD)");
        return;
    }
    if (!Helpers::isValidMealType(mealType)) {
        printError("Invalid meal type. Use: breakfast, lunch, dinner, snack");
        return;
    }
    if (targetCal <= 0 || targetProt <= 0) {
        printError("Targets must be positive");
        return;
    }
    if (!profiles_.hasProfile(profileName)) {
        printError("Profile not found");
        return;
    }
    FoodBase* base = bases_.getBase(baseName);
    if (!base) {
        printError("Base not found");
        return;
    }

    List<MealSolution> solutions = KnapsackSolver::solve(*base, targetCal, targetProt);
    if (solutions.size() == 0) {
        printError("No suitable combination found (try adding more products)");
        return;
    }

    int opt = 1;
    for (auto it = solutions.begin(); it != solutions.end(); ++it) {
        std::cout << "=== OPTION " << opt << " ===\n";
        std::cout << "TOTAL: " << it->totalCalories << " kcal, " << it->totalProtein << "g protein\n";
        for (auto itemIt = it->items.begin(); itemIt != it->items.end(); ++itemIt) {
            std::cout << "- " << itemIt->productName << " (" << itemIt->grams << "g) — "
                      << itemIt->calories << " kcal, " << itemIt->protein << "g protein\n";
        }
        ++opt;
    }

    int selectedIdx = 0;
    bool editMode = false;
    while (true) {
        std::cout << "Select option (1-3) or edit (e): ";
        std::string choice;
        std::getline(std::cin, choice);
        if (choice == "e") {
            editMode = true;
            break;
        }
        if (choice.size() == 1 && choice[0] >= '1' && choice[0] <= '3') {
            selectedIdx = choice[0] - '0';
            break;
        }
        printError("Invalid choice. Enter 1-3 or e.");
    }

    MealSolution selected;

    if (!editMode) {
        int cur = 1;
        for (auto it = solutions.begin(); it != solutions.end(); ++it) {
            if (cur == selectedIdx) { selected = *it; break; }
            ++cur;
        }
    } else {
        int editIdx = 0;
        while (true) {
            std::cout << "Select option to edit (1-3): ";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice.size() == 1 && choice[0] >= '1' && choice[0] <= '3') {
                editIdx = choice[0] - '0';
                break;
            }
            printError("Invalid option. Enter 1-3.");
        }
        int cur = 1;
        for (auto it = solutions.begin(); it != solutions.end(); ++it) {
            if (cur == editIdx) { selected = *it; break; }
            ++cur;
        }

        std::cout << "\n=== EDIT MODE ===\n";
        std::cout << "Current meal (base: " << baseName << "):\n";
        for (auto itemIt = selected.items.begin(); itemIt != selected.items.end(); ++itemIt) {
            std::cout << "- " << itemIt->productName << " (" << itemIt->grams << "g) — "
                      << itemIt->calories << " kcal, " << itemIt->protein << "g protein\n";
        }
        std::cout << "TOTAL: " << selected.totalCalories << " kcal, " << selected.totalProtein << "g protein\n";
        std::cout << "\nCommands: add <product> <grams>, remove <product>, change <product> <grams>, done, cancel\n";

        bool editing = true;
        while (editing) {
            std::cout << "> ";
            std::string line;
            std::getline(std::cin, line);
            line = Helpers::trim(line);
            if (line.empty()) continue;

            if (line == "done") {
                editing = false;
                break;
            }
            if (line == "cancel") {
                std::cout << "Edit cancelled. Meal not added.\n";
                return;
            }

            karpenko::List<std::string> tokens = Helpers::split(line, ' ');
            if (tokens.size() == 0) continue;
            auto it = tokens.begin();
            std::string action = Helpers::toLower(*it);
            ++it;

            if (action == "add") {
                if (tokens.size() < 3) {
                    printError("Usage: add <product> <grams>");
                    continue;
                }
                std::string prodName = *it;
                ++it;
                double grams = std::atof(it->c_str());
                if (grams <= 0) {
                    printError("Grams must be positive");
                    continue;
                }
                const Product* product = base->getProduct(prodName);
                if (!product) {
                    printError("Product not found in base");
                    continue;
                }
                double cal = product->calculateCalories(grams);
                double prot = product->calculateProteins(grams);
                KnapItem newItem;
                newItem.productName = prodName;
                newItem.grams = grams;
                newItem.calories = cal;
                newItem.protein = prot;
                selected.items.push_back(newItem);
                selected.totalCalories += cal;
                selected.totalProtein += prot;
                std::cout << "Added " << prodName << " (" << grams << "g) — " << cal << " kcal, " << prot << "g protein\n";
            }
            else if (action == "remove") {
                if (tokens.size() < 2) {
                    printError("Usage: remove <product>");
                    continue;
                }
                std::string prodName = *it;
                bool removed = false;
                List<KnapItem> newItems;
                double newCal = 0, newProt = 0;
                for (auto item = selected.items.begin(); item != selected.items.end(); ++item) {
                    if (item->productName == prodName) {
                        removed = true;
                    } else {
                        newItems.push_back(*item);
                        newCal += item->calories;
                        newProt += item->protein;
                    }
                }
                if (removed) {
                    selected.items = newItems;
                    selected.totalCalories = newCal;
                    selected.totalProtein = newProt;
                    std::cout << "Removed " << prodName << "\n";
                } else {
                    printError("Product not found in current meal");
                }
            }
            else if (action == "change") {
                if (tokens.size() < 3) {
                    printError("Usage: change <product> <grams>");
                    continue;
                }
                std::string prodName = *it;
                ++it;
                double newGrams = std::atof(it->c_str());
                if (newGrams <= 0) {
                    printError("Grams must be positive");
                    continue;
                }
                const Product* product = base->getProduct(prodName);
                if (!product) {
                    printError("Product not found in base");
                    continue;
                }
                bool changed = false;
                List<KnapItem> newItems;
                double newCal = 0, newProt = 0;
                for (auto item = selected.items.begin(); item != selected.items.end(); ++item) {
                    if (item->productName == prodName) {
                        KnapItem newItem = *item;
                        newItem.grams = newGrams;
                        newItem.calories = product->calculateCalories(newGrams);
                        newItem.protein = product->calculateProteins(newGrams);
                        newItems.push_back(newItem);
                        newCal += newItem.calories;
                        newProt += newItem.protein;
                        changed = true;
                    } else {
                        newItems.push_back(*item);
                        newCal += item->calories;
                        newProt += item->protein;
                    }
                }
                if (changed) {
                    selected.items = newItems;
                    selected.totalCalories = newCal;
                    selected.totalProtein = newProt;
                    std::cout << "Changed " << prodName << " to " << newGrams << "g\n";
                } else {
                    printError("Product not found in current meal");
                }
            }
            else {
                printError("Unknown command. Use add/remove/change/done/cancel");
            }

            std::cout << "\nCurrent meal:\n";
            for (auto itemIt = selected.items.begin(); itemIt != selected.items.end(); ++itemIt) {
                std::cout << "- " << itemIt->productName << " (" << itemIt->grams << "g) — "
                          << itemIt->calories << " kcal, " << itemIt->protein << "g protein\n";
            }
            std::cout << "TOTAL: " << selected.totalCalories << " kcal, " << selected.totalProtein << "g protein\n";
        }
    }

    MealEntry entry(profileName, date, mealType);
    for (auto itemIt = selected.items.begin(); itemIt != selected.items.end(); ++itemIt) {
        MealItem mitem(baseName, itemIt->productName, itemIt->grams, itemIt->calories, itemIt->protein);
        entry.addItem(mitem);
    }
    diary_.addEntry(entry);
    printSuccess("Meal added to " + profileName + "'s diary for " + date + " " + mealType);
}

void CommandHandler::handleMealAdd(const std::string* args, int count) {
    if (count != 6) {
        printError("meal-add requires: <profile> <date> <meal> <base> <product> <grams>");
        return;
    }
    std::string profileName = args[0];
    std::string date = args[1];
    std::string mealType = Helpers::toLower(args[2]);
    std::string baseName = args[3];
    std::string productName = args[4];
    double grams = std::atof(args[5].c_str());

    if (!Helpers::isValidDate(date)) {
        printError("Invalid date format");
        return;
    }
    if (!Helpers::isValidMealType(mealType)) {
        printError("Invalid meal type");
        return;
    }
    if (grams <= 0) {
        printError("Grams must be positive");
        return;
    }
    if (!profiles_.hasProfile(profileName)) {
        printError("Profile not found");
        return;
    }
    FoodBase* base = bases_.getBase(baseName);
    if (!base) {
        printError("Base not found");
        return;
    }
    const Product* prod = base->getProduct(productName);
    if (!prod) {
        printError("Product not found in base");
        return;
    }
    double cal = prod->calculateCalories(grams);
    double prot = prod->calculateProteins(grams);
    MealItem item(baseName, productName, grams, cal, prot);
    MealEntry entry(profileName, date, mealType);
    entry.addItem(item);
    diary_.addEntry(entry);
    printSuccess(mealType + " added | " + std::to_string(cal) + " kcal, " + std::to_string(prot) + "g protein");
}

void CommandHandler::handleDayStats(const std::string* args, int count) {
    if (count != 2) {
        printError("day-stats requires: <profile> <date>");
        return;
    }
    std::string profileName = args[0];
    std::string date = args[1];
    if (!Helpers::isValidDate(date)) {
        printError("Invalid date");
        return;
    }
    if (!profiles_.hasProfile(profileName)) {
        printError("Profile not found");
        return;
    }
    List<MealEntry> entries = diary_.getEntriesByDate(profileName, date);
    const Profile* prof = profiles_.getProfile(profileName);
    double totalCal = 0, totalProt = 0;
    std::cout << "=== " << profileName << ": " << date << " ===\n";
    if (entries.size() == 0) {
        std::cout << "No meals recorded\n";
    } else {
        for (auto it = entries.begin(); it != entries.end(); ++it) {
            std::cout << it->toString() << "\n";
            totalCal += it->totalCalories();
            totalProt += it->totalProtein();
        }
    }
    std::cout << "-------------------\n";
    std::cout << "TOTAL: " << totalCal << " kcal, " << totalProt << "g protein\n";
    if (prof) {
        std::cout << "\nDaily goal: " << prof->getTargetCalories() << " kcal, " << prof->getTargetProtein() << "g protein\n";
        std::cout << "Remaining: " << (prof->getTargetCalories() - totalCal) << " kcal, "
                  << (prof->getTargetProtein() - totalProt) << "g protein\n";
    }
}

void CommandHandler::handleSave(const std::string* args, int count) {
    if (count != 1) {
        printError("save requires <filename>");
        return;
    }
    std::string filename = args[0];
    if (fileManager_.save(filename, profiles_, bases_, diary_))
        printSuccess("Data saved to " + filename);
    else
        printError("Failed to save data");
}

void CommandHandler::handleLoad(const std::string* args, int count) {
    if (count != 1) {
        printError("load requires <filename>");
        return;
    }
    std::string filename = args[0];
    if (fileManager_.load(filename, profiles_, bases_, diary_))
        printSuccess("Data loaded from " + filename);
    else
        printError("Failed to load data");
}

void CommandHandler::handleHelp(const std::string* args, int count) {
    if (count == 0)
        std::cout << CommandParser::getHelp();
    else
        std::cout << CommandParser::getHelpForCommand(args[0]);
}

void CommandHandler::handle(const ParsedCommand& cmd) {
    switch (cmd.type) {
        case CMD_PROFILE_CREATE: handleProfileCreate(cmd.args, cmd.argCount); break;
        case CMD_PROFILE_LIST: handleProfileList(); break;
        case CMD_PROFILE_DELETE: handleProfileDelete(cmd.args, cmd.argCount); break;
        case CMD_BASE_CREATE: handleBaseCreate(cmd.args, cmd.argCount); break;
        case CMD_BASE_LIST: handleBaseList(); break;
        case CMD_BASE_ADD: handleBaseAdd(cmd.args, cmd.argCount); break;
        case CMD_BASE_REMOVE: handleBaseRemove(cmd.args, cmd.argCount); break;
        case CMD_BASE_SHOW: handleBaseShow(cmd.args, cmd.argCount); break;
        case CMD_PLAN_MEAL: handlePlanMeal(cmd.args, cmd.argCount); break;
        case CMD_MEAL_ADD: handleMealAdd(cmd.args, cmd.argCount); break;
        case CMD_DAY_STATS: handleDayStats(cmd.args, cmd.argCount); break;
        case CMD_SAVE: handleSave(cmd.args, cmd.argCount); break;
        case CMD_LOAD: handleLoad(cmd.args, cmd.argCount); break;
        case CMD_HELP: handleHelp(cmd.args, cmd.argCount); break;
        case CMD_EXIT: break;
        default: printError("Unknown command"); break;
    }
}

}
