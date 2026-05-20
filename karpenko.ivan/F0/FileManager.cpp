#include "FileManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

namespace karpenko {

static std::string trim2(const std::string& str) {
    size_t start = 0, end = str.length();
    while (start < end && std::isspace(str[start])) ++start;
    while (end > start && std::isspace(str[end-1])) --end;
    return str.substr(start, end - start);
}

static std::string readSection(const std::string& content, const std::string& secName) {
    std::string start = "[" + secName + "]\n";
    std::string end = "\n---\n";
    size_t startPos = content.find(start);
    if (startPos == std::string::npos) return "";
    startPos += start.length();
    size_t endPos = content.find(end, startPos);
    if (endPos == std::string::npos) return "";
    return content.substr(startPos, endPos - startPos);
}

bool FileManager::save(const std::string& filename, const ProfileDB& profiles, const BaseDB& bases, const MealDiary& diary) {
    std::ofstream file(filename);
    if (!file) return false;
    file << "[PROFILES]\n" << profiles.serialize() << "---\n";
    file << "[BASES]\n" << bases.serialize() << "---\n";
    file << "[DIARY]\n" << diary.serialize() << "---\n";
    return true;
}

bool FileManager::load(const std::string& filename, ProfileDB& profiles, BaseDB& bases, MealDiary& diary) {
    std::ifstream file(filename);
    if (!file) return false;
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    std::string profData = readSection(content, "PROFILES");
    std::string baseData = readSection(content, "BASES");
    std::string diaryData = readSection(content, "DIARY");

    std::stringstream profSs(profData);
    std::string line;
    while (std::getline(profSs, line)) {
        line = trim2(line);
        if (line.empty()) continue;
        Profile p = Profile::deserialize(line);
        const std::string& name = p.getName();

        if (!profiles.hasProfile(name)) {
            profiles.addProfile(p);
            std::cout << "Profile \"" << name << "\": new profile added.\n";
        } else {
            std::cout << "Profile \"" << name << "\" already exists.\n";
            std::cout << "  Choose: (s)kip / (o)verwrite / (r)ename: ";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice == "o") {
                profiles.removeProfile(name);
                profiles.addProfile(p);
                std::cout << "  Overwritten.\n";
            } else if (choice == "r") {
                std::cout << "  Enter new name: ";
                std::string newName;
                std::getline(std::cin, newName);
                if (!newName.empty() && !profiles.hasProfile(newName)) {
                    Profile p2 = p;
                    Profile newProfile(newName, p.getHeight(), p.getWeight(), p.getGoal(), p.getActivity());
                    profiles.addProfile(newProfile);
                    std::cout << "  Profile added as \"" << newName << "\".\n";
                } else {
                    std::cout << "  Invalid or duplicate name. Skipped.\n";
                }
            } else {
                std::cout << "  Skipped.\n";
            }
        }
    }

    std::stringstream baseSs(baseData);
    while (std::getline(baseSs, line)) {
        line = trim2(line);
        if (line.empty()) continue;
        FoodBase newBase = FoodBase::deserialize(line);
        const std::string& baseName = newBase.getName();

        if (!bases.hasBase(baseName)) {
            bases.addBase(newBase);
            std::cout << "Base \"" << baseName << "\": new base added with " << newBase.size() << " products.\n";
        } else {
            std::cout << "Base \"" << baseName << "\" already exists.\n";
            std::cout << "  Choose: (s)kip / (o)verwrite / (m)erge products: ";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice == "o") {
                bases.removeBase(baseName);
                bases.addBase(newBase);
                std::cout << "  Base overwritten.\n";
            } else if (choice == "m") {
                FoodBase* existingBase = bases.getBase(baseName);
                if (existingBase) {
                    List<Product> productsFromNew = newBase.getAllProducts();
                    for (auto it = productsFromNew.begin(); it != productsFromNew.end(); ++it) {
                        const std::string& prodName = it->getName();
                        if (!existingBase->containsProduct(prodName)) {
                            existingBase->addProduct(*it);
                            std::cout << "    Product \"" << prodName << "\" added.\n";
                        } else {
                            std::cout << "    Product \"" << prodName << "\" already exists.\n";
                            std::cout << "      Choose: (s)kip / (o)verwrite / (r)ename: ";
                            std::string subChoice;
                            std::getline(std::cin, subChoice);
                            if (subChoice == "o") {
                                existingBase->removeProduct(prodName);
                                existingBase->addProduct(*it);
                                std::cout << "      Overwritten.\n";
                            } else if (subChoice == "r") {
                                std::cout << "        Enter new name: ";
                                std::string newProdName;
                                std::getline(std::cin, newProdName);
                                if (!newProdName.empty() && !existingBase->containsProduct(newProdName)) {
                                    Product renamedProduct = *it;
                                    renamedProduct.setName(newProdName);
                                    existingBase->addProduct(renamedProduct);
                                    std::cout << "      Product added as \"" << newProdName << "\".\n";
                                } else {
                                    std::cout << "      Invalid or duplicate name. Skipped.\n";
                                }
                            } else {
                                std::cout << "      Skipped.\n";
                            }
                        }
                    }
                    std::cout << "  Merged.\n";
                }
            } else {
                std::cout << "  Skipped.\n";
            }
        }
    }

    std::stringstream diarySs(diaryData);
    while (std::getline(diarySs, line)) {
        line = trim2(line);
        if (line.empty()) continue;
        MealEntry entry = MealEntry::deserialize(line);
        const std::string& profile = entry.getProfileName();
        const std::string& date = entry.getDate();
        const std::string& mealType = entry.getMealType();

        List<MealEntry> existing = diary.getEntriesByDate(profile, date);
        bool conflict = false;
        for (auto e = existing.begin(); e != existing.end(); ++e) {
            if (e->getMealType() == mealType) {
                conflict = true;
                break;
            }
        }

        if (!conflict) {
            diary.addEntry(entry);
            std::cout << "Diary entry " << profile << "|" << date << "|" << mealType << ": new entry added.\n";
        } else {
            std::cout << "Diary entry " << profile << "|" << date << "|" << mealType << " already exists.\n";
            std::cout << "  Choose: (s)kip / (o)verwrite / (a)dd both: ";
            std::string choice;
            std::getline(std::cin, choice);
            if (choice == "o") {
                diary.removeEntry(profile, date, mealType);
                diary.addEntry(entry);
                std::cout << "  Overwritten.\n";
            } else if (choice == "a") {
                diary.addEntry(entry);
                std::cout << "  Added both.\n";
            } else {
                std::cout << "  Skipped.\n";
            }
        }
    }

    std::cout << "Load completed.\n";
    return true;
}

}
