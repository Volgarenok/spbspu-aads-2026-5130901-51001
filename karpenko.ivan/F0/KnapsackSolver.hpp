#ifndef KNAPSACK_SOLVER_HPP
#define KNAPSACK_SOLVER_HPP

#include <string>

#include "FoodBase.hpp"
#include "../common/list.hpp"

namespace karpenko {

  struct KnapItem {
    std::string product_name_;
    int grams_;
    double calories_;
    double protein_;
  };

  struct MealSolution {
    List< KnapItem > items_;
    double total_calories_;
    double total_protein_;
  };

  class KnapsackSolver {
  public:
    static List< MealSolution > solve(const FoodBase& base, double target_cal, double target_protein);
  };

}

#endif
