#include "KnapsackSolver.hpp"

#include <cmath>
#include <climits>

namespace karpenko {

  namespace {
    const int GRAMS_STEPS[] = {50, 100, 150, 200, 250, 300};
    const int NUM_STEPS = 6;

    double deviation(double calories, double protein, double targetCalories, double targetProtein)
    {
      double result = 0.0;
      if (targetCalories > 0.0) {
        result += std::fabs(calories - targetCalories) / targetCalories;
      }
      if (targetProtein > 0.0) {
        result += std::fabs(protein - targetProtein) / targetProtein;
      }
      return result;
    }
  }

  List<MealSolution> KnapsackSolver::solve(const FoodBase& base, double targetCalories, double targetProtein)
  {
    List<MealSolution> result;

    struct Item {
      std::string productName;
      int grams;
      double calories;
      double protein;
    };

    List<Item> items;
    List<Product> allProducts = base.getAllProducts();
    for (auto productIterator = allProducts.begin();
         productIterator != allProducts.end();
         ++productIterator) {
      const Product& product = *productIterator;
      for (int i = 0; i < NUM_STEPS; ++i) {
        int grams = GRAMS_STEPS[i];
        Item item;
        item.productName = product.getName();
        item.grams = grams;
        item.calories = product.calculateCalories(grams);
        item.protein = product.calculateProteins(grams);
        items.pushBack(item);
      }
    }

    int itemCount = 0;
    Item* itemArray = new Item[items.size()];
    for (auto it = items.begin(); it != items.end(); ++it) {
      itemArray[itemCount++] = *it;
    }

    struct Candidate {
      double calories;
      double protein;
      double deviation;
      int index1;
      int index2;
      int index3;
      Candidate() : calories(0.0), protein(0.0), deviation(1e9), index1(-1), index2(-1), index3(-1) {}
    };

    Candidate best[3];
    int bestCount = 0;

    auto updateBest = [&](const Candidate& cand) {
      if (bestCount == 0) {
        best[0] = cand;
        bestCount = 1;
        return;
      }
      int pos = bestCount;
      while (pos > 0 && best[pos - 1].deviation > cand.deviation) {
        if (pos < 3) {
          best[pos] = best[pos - 1];
        }
        --pos;
      }
      if (pos < 3) {
        best[pos] = cand;
        if (bestCount < 3) {
          ++bestCount;
        }
      }
    };

    for (int i = 0; i < itemCount; ++i) {
      Candidate cand;
      cand.calories = itemArray[i].calories;
      cand.protein = itemArray[i].protein;
      cand.deviation = deviation(cand.calories, cand.protein, targetCalories, targetProtein);
      cand.index1 = i;
      updateBest(cand);
    }

    for (int i = 0; i < itemCount; ++i) {
      for (int j = i + 1; j < itemCount; ++j) {
        if (itemArray[i].productName == itemArray[j].productName) {
          continue;
        }
        Candidate cand;
        cand.calories = itemArray[i].calories + itemArray[j].calories;
        cand.protein = itemArray[i].protein + itemArray[j].protein;
        cand.deviation = deviation(cand.calories, cand.protein, targetCalories, targetProtein);
        cand.index1 = i;
        cand.index2 = j;
        updateBest(cand);
      }
    }

    for (int i = 0; i < itemCount; ++i) {
      for (int j = i + 1; j < itemCount; ++j) {
        if (itemArray[i].productName == itemArray[j].productName) {
          continue;
        }
        for (int k = j + 1; k < itemCount; ++k) {
          if (itemArray[i].productName == itemArray[k].productName ||
              itemArray[j].productName == itemArray[k].productName) {
            continue;
          }
          Candidate cand;
          cand.calories = itemArray[i].calories + itemArray[j].calories + itemArray[k].calories;
          cand.protein = itemArray[i].protein + itemArray[j].protein + itemArray[k].protein;
          cand.deviation = deviation(cand.calories, cand.protein, targetCalories, targetProtein);
          cand.index1 = i;
          cand.index2 = j;
          cand.index3 = k;
          updateBest(cand);
        }
      }
    }

    for (int n = 0; n < bestCount; ++n) {
      MealSolution solution;
      solution.total_calories_ = best[n].calories;
      solution.total_protein_ = best[n].protein;
      if (best[n].index1 != -1) {
        KnapItem item;
        item.product_name_ = itemArray[best[n].index1].productName;
        item.grams_ = itemArray[best[n].index1].grams;
        item.calories_ = itemArray[best[n].index1].calories;
        item.protein_ = itemArray[best[n].index1].protein;
        solution.items_.pushBack(item);
      }
      if (best[n].index2 != -1) {
        KnapItem item;
        item.product_name_ = itemArray[best[n].index2].productName;
        item.grams_ = itemArray[best[n].index2].grams;
        item.calories_ = itemArray[best[n].index2].calories;
        item.protein_ = itemArray[best[n].index2].protein;
        solution.items_.pushBack(item);
      }
      if (best[n].index3 != -1) {
        KnapItem item;
        item.product_name_ = itemArray[best[n].index3].productName;
        item.grams_ = itemArray[best[n].index3].grams;
        item.calories_ = itemArray[best[n].index3].calories;
        item.protein_ = itemArray[best[n].index3].protein;
        solution.items_.pushBack(item);
      }
      result.pushBack(solution);
    }

    delete[] itemArray;
    return result;
  }

}
