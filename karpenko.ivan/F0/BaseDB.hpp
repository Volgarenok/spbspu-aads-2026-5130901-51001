#ifndef BASE_DB_HPP
#define BASE_DB_HPP

#include <string>
#include "../common/robin_hood_hash.hpp"
#include "../common/list.hpp"
#include "FoodBase.hpp"

namespace karpenko {

class BaseDB {
private:
    RobinHoodHashTable<std::string, FoodBase> bases_by_name_;

public:
    BaseDB();

    bool addBase(const FoodBase& base);
    bool removeBase(const std::string& name);
    FoodBase* getBase(const std::string& name);
    const FoodBase* getBase(const std::string& name) const;
    bool hasBase(const std::string& name) const;

    karpenko::List<const FoodBase*> getAllBases() const;
    size_t size() const;

    std::string serialize() const;
    void deserialize(const std::string& data);
};

}

#endif
