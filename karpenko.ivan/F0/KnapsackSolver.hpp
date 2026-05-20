#ifndef KNAPSACK_SOLVER_HPP
#define KNAPSACK_SOLVER_HPP

#include "FoodBase.hpp"
#include "../common/list.hpp"

namespace karpenko {

struct KnapItem {
    std::string productName;
    int grams;
    double calories;
    double protein;
};

struct MealSolution {
    List<KnapItem> items;
    double totalCalories;
    double totalProtein;
};

class KnapsackSolver {
public:
    static List<MealSolution> solve(const FoodBase& base, double targetCal, double targetProtein);
};

}

#endif
