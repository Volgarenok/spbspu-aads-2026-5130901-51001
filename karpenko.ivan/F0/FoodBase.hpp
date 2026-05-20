#ifndef FOOD_BASE_HPP
#define FOOD_BASE_HPP

#include <string>
#include "Product.hpp"
#include "../common/robin_hood_hash.hpp"
#include "../common/avl_tree.hpp"
#include "../common/list.hpp"

namespace karpenko {

class FoodBase {
private:
    std::string name_;
    RobinHoodHashTable<std::string, Product> products_;
    AVLTree<std::string, Product> sortedProducts_;

public:
    FoodBase();
    explicit FoodBase(const std::string& name);

    FoodBase(const FoodBase&) = default;
    FoodBase& operator=(const FoodBase&) = default;
    FoodBase(FoodBase&&) = default;
    FoodBase& operator=(FoodBase&&) = default;

    const std::string& getName() const;
    void setName(const std::string& n);

    bool addProduct(const Product& product);
    bool removeProduct(const std::string& productName);
    bool containsProduct(const std::string& productName) const;
    const Product* getProduct(const std::string& productName) const;

    karpenko::List<Product> getAllProducts() const;
    size_t size() const;

    std::string serialize() const;
    static FoodBase deserialize(const std::string& line);
};

}

#endif
