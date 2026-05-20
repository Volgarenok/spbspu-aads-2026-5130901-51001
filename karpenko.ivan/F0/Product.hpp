#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>

namespace karpenko {

  class Product {
   private:
    std::string name_;
    double proteins_;
    double fats_;
    double carbs_;
    double calories_;

   public:
    Product();
    Product(const std::string& name, double proteins, double fats, double carbs, double calories);

    const std::string& getName() const;
    void setName(const std::string& new_name);

    double getProteins() const;
    double getFats() const;
    double getCarbs() const;
    double getCalories() const;

    double calculateCalories(double grams) const;
    double calculateProteins(double grams) const;

    std::string toString() const;
    std::string serialize() const;
    static Product deserialize(const std::string& line);
  };

}

#endif
