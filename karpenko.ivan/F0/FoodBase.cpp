#include "FoodBase.hpp"

#include <sstream>

namespace karpenko {

  FoodBase::FoodBase():
    name_("")
  {}

  FoodBase::FoodBase(const std::string& name):
    name_(name)
  {}

  const std::string& FoodBase::getName() const
  {
    return name_;
  }

  void FoodBase::setName(const std::string& newName)
  {
    name_ = newName;
  }

  bool FoodBase::addProduct(const Product& product)
  {
    if (products_.contains(product.getName())) {
      return false;
    }
    products_.insert(product.getName(), product);
    sortedProducts_.insert(product.getName(), product);
    return true;
  }

  bool FoodBase::removeProduct(const std::string& productName)
  {
    if (!products_.contains(productName)) {
      return false;
    }
    products_.erase(productName);
    sortedProducts_.erase(productName);
    return true;
  }

  bool FoodBase::containsProduct(const std::string& productName) const
  {
    return products_.contains(productName);
  }

  const Product* FoodBase::getProduct(const std::string& productName) const
  {
    if (!products_.contains(productName)) {
      return nullptr;
    }
    return &products_.at(productName);
  }

  List<Product> FoodBase::getAllProducts() const
  {
    List<Product> result;
    for (auto it = sortedProducts_.begin(); it != sortedProducts_.end(); ++it) {
      result.pushBack(it->second);
    }
    return result;
  }

  size_t FoodBase::size() const
  {
    return products_.size();
  }

  std::string FoodBase::serialize() const
  {
    std::ostringstream output;
    output << name_ << "|";
    bool first = true;
    for (auto it = sortedProducts_.begin(); it != sortedProducts_.end(); ++it) {
      if (!first) {
        output << ";";
      }
      output << it->second.serialize();
      first = false;
    }
    return output.str();
  }

  FoodBase FoodBase::deserialize(const std::string& line)
  {
    size_t pos = line.find('|');
    std::string baseName = line.substr(0, pos);
    FoodBase base(baseName);
    std::string productsPart = line.substr(pos + 1);
    if (productsPart.empty()) {
      return base;
    }
    std::stringstream stream(productsPart);
    std::string productLine;
    while (std::getline(stream, productLine, ';')) {
      if (!productLine.empty()) {
        Product product = Product::deserialize(productLine);
        base.addProduct(product);
      }
    }
    return base;
  }

}
