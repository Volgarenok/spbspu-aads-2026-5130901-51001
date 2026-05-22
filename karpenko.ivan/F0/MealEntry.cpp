#include "MealEntry.hpp"

#include <sstream>

namespace karpenko {

  MealItem::MealItem(const std::string& base, const std::string& prod, double g, double cal, double prot):
    base_name_(base),
    product_name_(prod),
    grams_(g),
    calories_(cal),
    protein_(prot)
  {}

  std::string MealItem::toString() const
  {
    std::ostringstream oss;
    oss << base_name_ << ":" << product_name_ << " (" << grams_ << "g) — "
        << calories_ << " kcal, " << protein_ << "g protein";
    return oss.str();
  }

  std::string MealItem::serialize() const
  {
    std::ostringstream oss;
    oss << base_name_ << "|" << product_name_ << "|" << grams_ << "|"
        << calories_ << "|" << protein_;
    return oss.str();
  }

  MealItem MealItem::deserialize(const std::string& str)
  {
    std::stringstream ss(str);
    std::string base, prod;
    double g, cal, prot;
    std::getline(ss, base, '|');
    std::getline(ss, prod, '|');
    ss >> g;
    ss.ignore();
    ss >> cal;
    ss.ignore();
    ss >> prot;
    return MealItem(base, prod, g, cal, prot);
  }

  MealEntry::MealEntry(const std::string& profile, const std::string& date, const std::string& meal_type):
    profile_name_(profile),
    date_(date),
    meal_type_(meal_type)
  {}

  const std::string& MealEntry::getProfileName() const
  {
    return profile_name_;
  }

  const std::string& MealEntry::getDate() const
  {
    return date_;
  }

  const std::string& MealEntry::getMealType() const
  {
    return meal_type_;
  }

  const List<MealItem>& MealEntry::getItems() const
  {
    return items_;
  }

  void MealEntry::addItem(const MealItem& item)
  {
    items_.pushBack(item);
  }

  bool MealEntry::removeItem(size_t index)
  {
    if (index < 1 || index > items_.size()) {
      return false;
    }
    size_t i = 1;
    List<MealItem> new_items;
    for (auto it = items_.begin(); it != items_.end(); ++it) {
      if (i != index) {
        new_items.pushBack(*it);
      }
      ++i;
    }
    items_ = new_items;
    return true;
  }

  void MealEntry::clearItems()
  {
    items_.clear();
  }

  double MealEntry::totalCalories() const
  {
    double sum = 0.0;
    for (auto it = items_.begin(); it != items_.end(); ++it) {
      sum += it->calories_;
    }
    return sum;
  }

  double MealEntry::totalProtein() const
  {
    double sum = 0.0;
    for (auto it = items_.begin(); it != items_.end(); ++it) {
      sum += it->protein_;
    }
    return sum;
  }

  std::string MealEntry::toString() const
  {
    std::ostringstream oss;
    oss << meal_type_ << ":\n";
    size_t idx = 1;
    for (auto it = items_.begin(); it != items_.end(); ++it) {
      oss << "  " << idx++ << ". " << it->toString() << "\n";
    }
    oss << "  Total: " << totalCalories() << " kcal, "
        << totalProtein() << "g protein";
    return oss.str();
  }

  std::string MealEntry::serialize() const
  {
    std::ostringstream oss;
    oss << profile_name_ << "|" << date_ << "|" << meal_type_ << "|";
    bool first = true;
    for (auto it = items_.begin(); it != items_.end(); ++it) {
      if (!first) {
        oss << ";";
      }
      oss << it->serialize();
      first = false;
    }
    return oss.str();
  }

  MealEntry MealEntry::deserialize(const std::string& line)
  {
    std::stringstream ss(line);
    std::string profile, date, meal, items_str;
    std::getline(ss, profile, '|');
    std::getline(ss, date, '|');
    std::getline(ss, meal, '|');
    std::getline(ss, items_str);
    MealEntry entry(profile, date, meal);
    if (items_str.empty()) {
      return entry;
    }
    std::stringstream items_ss(items_str);
    std::string item;
    while (std::getline(items_ss, item, ';')) {
      if (!item.empty()) {
        entry.addItem(MealItem::deserialize(item));
      }
    }
    return entry;
  }

}
