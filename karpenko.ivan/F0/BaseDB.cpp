#include "BaseDB.hpp"
#include <sstream>

namespace karpenko {

BaseDB::BaseDB() {}

bool BaseDB::addBase(const FoodBase& base) {
    const std::string& name = base.getName();
    if (bases_by_name_.contains(name))
        return false;
    bases_by_name_.insert(name, base);
    return true;
}

bool BaseDB::removeBase(const std::string& name) {
    if (!bases_by_name_.contains(name))
        return false;
    bases_by_name_.erase(name);
    return true;
}

FoodBase* BaseDB::getBase(const std::string& name) {
    if (!bases_by_name_.contains(name))
        return nullptr;
    return &bases_by_name_.at(name);
}

const FoodBase* BaseDB::getBase(const std::string& name) const {
    if (!bases_by_name_.contains(name))
        return nullptr;
    return &bases_by_name_.at(name);
}

bool BaseDB::hasBase(const std::string& name) const {
    return bases_by_name_.contains(name);
}

List<const FoodBase*> BaseDB::getAllBases() const {
    List<const FoodBase*> result;
    for (auto it = bases_by_name_.begin(); it != bases_by_name_.end(); ++it) {
        result.push_back(&(*it).second);
    }
    return result;
}

size_t BaseDB::size() const {
    return bases_by_name_.size();
}

std::string BaseDB::serialize() const {
    std::ostringstream oss;
    for (auto it = bases_by_name_.begin(); it != bases_by_name_.end(); ++it) {
        const FoodBase& base = (*it).second;
        oss << base.serialize() << "\n";
    }
    return oss.str();
}

void BaseDB::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string line;
    while (std::getline(ss, line)) {
        if (line.empty()) continue;
        FoodBase b = FoodBase::deserialize(line);
        addBase(b);
    }
}

}
