#include "BaseDB.hpp"

#include <sstream>

namespace karpenko {

  BaseDB::BaseDB()
  {}

  bool BaseDB::addBase(const FoodBase& base)
  {
    const std::string& name = base.getName();
    if (bases_by_name_.contains(name)) {
      return false;
    }
    bases_by_name_.insert(name, base);
    sorted_bases_.insert(name, base);
    return true;
  }

  bool BaseDB::removeBase(const std::string& name)
  {
    if (!bases_by_name_.contains(name)) {
      return false;
    }
    bases_by_name_.erase(name);
    sorted_bases_.erase(name);
    return true;
  }

  FoodBase* BaseDB::getBase(const std::string& name)
  {
    if (!bases_by_name_.contains(name)) {
      return nullptr;
    }
    return &bases_by_name_.at(name);
  }

  const FoodBase* BaseDB::getBase(const std::string& name) const
  {
    if (!bases_by_name_.contains(name)) {
      return nullptr;
    }
    return &bases_by_name_.at(name);
  }

  bool BaseDB::hasBase(const std::string& name) const
  {
    return bases_by_name_.contains(name);
  }

  List<const FoodBase*> BaseDB::getAllBases() const
  {
    List<const FoodBase*> result;
    for (auto it = bases_by_name_.begin(); it != bases_by_name_.end(); ++it) {
      result.pushBack(&(*it).second);
    }
    return result;
  }

  List<const FoodBase*> BaseDB::getAllBasesSorted() const
  {
    List<const FoodBase*> result;
    for (auto it = sorted_bases_.begin(); it != sorted_bases_.end(); ++it) {
      result.pushBack(&it->second);
    }
    return result;
  }

  size_t BaseDB::size() const
  {
    return bases_by_name_.size();
  }

  std::string BaseDB::serialize() const
  {
    std::ostringstream oss;
    for (auto it = sorted_bases_.begin(); it != sorted_bases_.end(); ++it) {
      oss << it->second.serialize() << "\n";
    }
    return oss.str();
  }

  void BaseDB::deserialize(const std::string& data)
  {
    std::stringstream ss(data);
    std::string line;
    while (std::getline(ss, line)) {
      if (line.empty()) {
        continue;
      }
      FoodBase base = FoodBase::deserialize(line);
      addBase(base);
    }
  }

}
