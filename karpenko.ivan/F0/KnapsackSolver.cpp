#include "KnapsackSolver.hpp"

#include <cmath>
#include <climits>

namespace karpenko
{

namespace
{

const int GRAMS_STEPS[] = {50, 100, 150, 200, 250, 300};
const int NUM_STEPS = 6;

double deviation(double calories, double protein, double targetCalories, double targetProtein)
{
  double result = 0.0;
  if (targetCalories > 0.0)
  {
    result += std::fabs(calories - targetCalories) / targetCalories;
  }
  if (targetProtein > 0.0)
  {
    result += std::fabs(protein - targetProtein) / targetProtein;
  }
  return result;
}

}

List< MealSolution > KnapsackSolver::solve(const FoodBase& base,
                                           double targetCalories,
                                           double targetProtein)
{
  List< MealSolution > result;

  struct Item
  {
    std::string productName;
    int grams;
    double calories;
    double protein;
  };

  List< Item > items;
  List< Product > allProducts = base.getAllProducts();
  for (auto productIterator = allProducts.begin();
       productIterator != allProducts.end();
       ++productIterator)
  {
    const Product& product = *productIterator;
    for (int i = 0; i < NUM_STEPS; ++i)
    {
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
  for (auto it = items.begin(); it != items.end(); ++it)
  {
    itemArray[itemCount++] = *it;
  }

  struct Candidate
  {
    double calories;
    double protein;
    double deviation;
    int index1;
    int index2;
    int index3;

    Candidate()
      : calories(0.0),
        protein(0.0),
        deviation(1e9),
        index1(-1),
        index2(-1),
        index3(-1)
    {
    }
  };

  List< Candidate > candidates;

  for (int i = 0; i < itemCount; ++i)
  {
    double cal = itemArray[i].calories;
    double prot = itemArray[i].protein;
    double dev = deviation(cal, prot, targetCalories, targetProtein);
    Candidate candidate;
    candidate.calories = cal;
    candidate.protein = prot;
    candidate.deviation = dev;
    candidate.index1 = i;
    candidates.pushBack(candidate);
  }

  for (int i = 0; i < itemCount; ++i)
  {
    for (int j = i + 1; j < itemCount; ++j)
    {
      if (itemArray[i].productName == itemArray[j].productName)
      {
        continue;
      }
      double cal = itemArray[i].calories + itemArray[j].calories;
      double prot = itemArray[i].protein + itemArray[j].protein;
      double dev = deviation(cal, prot, targetCalories, targetProtein);
      Candidate candidate;
      candidate.calories = cal;
      candidate.protein = prot;
      candidate.deviation = dev;
      candidate.index1 = i;
      candidate.index2 = j;
      candidates.pushBack(candidate);
    }
  }

  for (int i = 0; i < itemCount; ++i)
  {
    for (int j = i + 1; j < itemCount; ++j)
    {
      if (itemArray[i].productName == itemArray[j].productName)
      {
        continue;
      }
      for (int k = j + 1; k < itemCount; ++k)
      {
        if (itemArray[i].productName == itemArray[k].productName ||
            itemArray[j].productName == itemArray[k].productName)
        {
          continue;
        }
        double cal = itemArray[i].calories + itemArray[j].calories + itemArray[k].calories;
        double prot = itemArray[i].protein + itemArray[j].protein + itemArray[k].protein;
        double dev = deviation(cal, prot, targetCalories, targetProtein);
        Candidate candidate;
        candidate.calories = cal;
        candidate.protein = prot;
        candidate.deviation = dev;
        candidate.index1 = i;
        candidate.index2 = j;
        candidate.index3 = k;
        candidates.pushBack(candidate);
      }
    }
  }

  int candidateCount = 0;
  for (auto it = candidates.begin(); it != candidates.end(); ++it)
  {
    ++candidateCount;
  }
  Candidate* candidateArray = new Candidate[candidateCount];
  int position = 0;
  for (auto it = candidates.begin(); it != candidates.end(); ++it)
  {
    candidateArray[position++] = *it;
  }

  for (int i = 0; i < candidateCount - 1; ++i)
  {
    for (int j = 0; j < candidateCount - i - 1; ++j)
    {
      if (candidateArray[j].deviation > candidateArray[j + 1].deviation)
      {
        Candidate temporary = candidateArray[j];
        candidateArray[j] = candidateArray[j + 1];
        candidateArray[j + 1] = temporary;
      }
    }
  }

  int limit = (candidateCount < 3) ? candidateCount : 3;
  for (int n = 0; n < limit; ++n)
  {
    MealSolution solution;
    solution.total_calories_ = candidateArray[n].calories;
    solution.total_protein_ = candidateArray[n].protein;
    if (candidateArray[n].index1 != -1)
    {
      KnapItem item;
      item.product_name_ = itemArray[candidateArray[n].index1].productName;
      item.grams_ = itemArray[candidateArray[n].index1].grams;
      item.calories_ = itemArray[candidateArray[n].index1].calories;
      item.protein_ = itemArray[candidateArray[n].index1].protein;
      solution.items_.pushBack(item);
    }
    if (candidateArray[n].index2 != -1)
    {
      KnapItem item;
      item.product_name_ = itemArray[candidateArray[n].index2].productName;
      item.grams_ = itemArray[candidateArray[n].index2].grams;
      item.calories_ = itemArray[candidateArray[n].index2].calories;
      item.protein_ = itemArray[candidateArray[n].index2].protein;
      solution.items_.pushBack(item);
    }
    if (candidateArray[n].index3 != -1)
    {
      KnapItem item;
      item.product_name_ = itemArray[candidateArray[n].index3].productName;
      item.grams_ = itemArray[candidateArray[n].index3].grams;
      item.calories_ = itemArray[candidateArray[n].index3].calories;
      item.protein_ = itemArray[candidateArray[n].index3].protein;
      solution.items_.pushBack(item);
    }
    result.pushBack(solution);
  }

  delete[] itemArray;
  delete[] candidateArray;
  return result;
}

}
