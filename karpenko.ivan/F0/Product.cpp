#include "Product.hpp"

#include <iomanip>
#include <sstream>

namespace karpenko
{

  Product::Product():
    name_(""),
    proteins_(0.0),
    fats_(0.0),
    carbs_(0.0),
    calories_(0.0)
  {}

  Product::Product(const std::string& name, double proteins, double fats, double carbs, double calories):
    name_(name),
    proteins_(proteins),
    fats_(fats),
    carbs_(carbs),
    calories_(calories)
  {}

  const std::string& Product::getName() const
  {
    return name_;
  }

  void Product::setName(const std::string& newName)
  {
    name_ = newName;
  }

  double Product::getProteins() const
  {
    return proteins_;
  }

  double Product::getFats() const
  {
    return fats_;
  }

  double Product::getCarbs() const
  {
    return carbs_;
  }

  double Product::getCalories() const
  {
    return calories_;
  }

  double Product::calculateCalories(double grams) const
  {
    return calories_ * grams / 100.0;
  }

  double Product::calculateProteins(double grams) const
  {
    return proteins_ * grams / 100.0;
  }

  std::string Product::toString() const
  {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << name_ << " | P:" << proteins_ << " F:" << fats_<< " C:" << carbs_ << " | " << calories_ << " kcal/100g";
    return oss.str();
  }

  std::string Product::serialize() const
  {
    std::ostringstream oss;
    oss << name_ << "|" << proteins_ << "|" << fats_ << "|" << carbs_ << "|" << calories_;
    return oss.str();
  }

  Product Product::deserialize(const std::string& line)
  {
    std::istringstream ss(line);
    std::string name = "";
    double proteins = 0.0;
    double fats = 0.0;
    double carbs = 0.0;
    double calories = 0.0;
    std::getline(ss, name, '|');
    ss >> proteins;
    ss.ignore();
    ss >> fats;
    ss.ignore();
    ss >> carbs;
    ss.ignore();
    ss >> calories;
    return Product(name, proteins, fats, carbs, calories);
  }

}
