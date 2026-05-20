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

void CommandHandler::handle(const ParsedCommand& cmd) {
    switch (cmd.type) {
        case CMD_PROFILE_CREATE: handleProfileCreate(cmd.args, cmd.argCount); break;
        case CMD_PROFILE_LIST: handleProfileList(); break;
        case CMD_PROFILE_DELETE: handleProfileDelete(cmd.args, cmd.argCount); break;
        default: printError("Unknown command"); break;
    }
}

}
