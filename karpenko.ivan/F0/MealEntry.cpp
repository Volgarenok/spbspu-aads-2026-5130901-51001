#include "MealEntry.hpp"
#include <sstream>
#include <iomanip>

namespace karpenko {

MealItem::MealItem() : grams(0), calories(0), protein(0) {}
MealItem::MealItem(const std::string& base, const std::string& prod, double g, double cal, double prot)
    : baseName(base), productName(prod), grams(g), calories(cal), protein(prot) {}

std::string MealItem::toString() const {
    std::ostringstream oss;
    oss << baseName << ":" << productName << " (" << grams << "g) — " << calories << " kcal, " << protein << "g protein";
    return oss.str();
}

std::string MealItem::serialize() const {
    std::ostringstream oss;
    oss << baseName << "|" << productName << "|" << grams << "|" << calories << "|" << protein;
    return oss.str();
}

MealItem MealItem::deserialize(const std::string& str) {
    std::stringstream ss(str);
    std::string base, prod;
    double g, cal, prot;
    std::getline(ss, base, '|');
    std::getline(ss, prod, '|');
    ss >> g; ss.ignore();
    ss >> cal; ss.ignore();
    ss >> prot;
    return MealItem(base, prod, g, cal, prot);
}

MealEntry::MealEntry() {}
MealEntry::MealEntry(const std::string& profile, const std::string& date, const std::string& mealType)
    : profileName_(profile), date_(date), mealType_(mealType) {}

const std::string& MealEntry::getProfileName() const { return profileName_; }
const std::string& MealEntry::getDate() const { return date_; }
const std::string& MealEntry::getMealType() const { return mealType_; }
const List<MealItem>& MealEntry::getItems() const { return items_; }

void MealEntry::addItem(const MealItem& item) { items_.push_back(item); }
bool MealEntry::removeItem(size_t index) {
    size_t i = 0;
    for (auto it = items_.begin(); it != items_.end(); ++it) {
        if (i == index) {
            items_.erase_after(it);
            return true;
        }
        ++i;
    }
    return false;
}
void MealEntry::clearItems() { items_.clear(); }

double MealEntry::totalCalories() const {
    double sum = 0;
    for (auto it = items_.begin(); it != items_.end(); ++it)
        sum += it->calories;
    return sum;
}
double MealEntry::totalProtein() const {
    double sum = 0;
    for (auto it = items_.begin(); it != items_.end(); ++it)
        sum += it->protein;
    return sum;
}

std::string MealEntry::toString() const {
    std::ostringstream oss;
    oss << mealType_ << ":\n";
    int idx = 1;
    for (auto it = items_.begin(); it != items_.end(); ++it) {
        oss << "  " << idx++ << ". " << it->toString() << "\n";
    }
    oss << "  Total: " << totalCalories() << " kcal, " << totalProtein() << "g protein";
    return oss.str();
}

std::string MealEntry::serialize() const {
    std::ostringstream oss;
    oss << profileName_ << "|" << date_ << "|" << mealType_ << "|";
    bool first = true;
    for (auto it = items_.begin(); it != items_.end(); ++it) {
        if (!first) oss << ";";
        oss << it->serialize();
        first = false;
    }
    return oss.str();
}

MealEntry MealEntry::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string profile, date, meal, itemsStr;
    std::getline(ss, profile, '|');
    std::getline(ss, date, '|');
    std::getline(ss, meal, '|');
    std::getline(ss, itemsStr);
    MealEntry entry(profile, date, meal);
    if (itemsStr.empty()) return entry;
    std::stringstream itemsSs(itemsStr);
    std::string item;
    while (std::getline(itemsSs, item, ';')) {
        if (!item.empty())
            entry.addItem(MealItem::deserialize(item));
    }
    return entry;
}

}
