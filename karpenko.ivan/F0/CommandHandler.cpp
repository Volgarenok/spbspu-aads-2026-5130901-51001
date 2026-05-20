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
        default: printError("Unknown command"); break;
    }
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
        printInfo("Edit mode not yet implemented");
        return;
    }

    MealEntry entry(profileName, date, mealType);
    for (auto itemIt = selected.items.begin(); itemIt != selected.items.end(); ++itemIt) {
        MealItem mitem(baseName, itemIt->productName, itemIt->grams, itemIt->calories, itemIt->protein);
        entry.addItem(mitem);
    }
    diary_.addEntry(entry);
    printSuccess("Meal added to " + profileName + "'s diary for " + date + " " + mealType);
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
        default: printError("Unknown command"); break;
    }
}

}

