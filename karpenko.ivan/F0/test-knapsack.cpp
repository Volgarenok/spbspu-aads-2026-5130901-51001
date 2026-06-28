#include <boost/test/unit_test.hpp>

#include "KnapsackSolver.hpp"

BOOST_AUTO_TEST_SUITE(KnapsackSolverTests)

BOOST_AUTO_TEST_CASE(SolveBasic)
{
  karpenko::FoodBase base("test");
  base.addProduct(karpenko::Product("chicken", 23, 1.5, 0, 110));
  base.addProduct(karpenko::Product("buckwheat", 12, 3, 68, 330));
  base.addProduct(karpenko::Product("broccoli", 2.8, 0.4, 4, 34));
  karpenko::List< karpenko::MealSolution > solutions = karpenko::KnapsackSolver::solve(base, 700, 35);
  BOOST_TEST(solutions.size() > 0);
  BOOST_TEST(solutions.size() <= 3);
  for (auto sol = solutions.begin(); sol != solutions.end(); ++sol)
  {
    BOOST_TEST(sol->total_calories_ > 0);
    BOOST_TEST(sol->total_protein_ > 0);
  }
}

BOOST_AUTO_TEST_CASE(NoDuplicateProducts)
{
  karpenko::FoodBase base("test");
  base.addProduct(karpenko::Product("chicken", 23, 1.5, 0, 110));
  base.addProduct(karpenko::Product("buckwheat", 12, 3, 68, 330));
  karpenko::List< karpenko::MealSolution > solutions = karpenko::KnapsackSolver::solve(base, 700, 35);
  for (auto sol = solutions.begin(); sol != solutions.end(); ++sol)
  {
    karpenko::List< std::string > names;
    bool duplicate = false;
    for (auto item = sol->items_.begin(); item != sol->items_.end(); ++item)
    {
      for (auto n = names.begin(); n != names.end(); ++n)
      {
        if (*n == item->product_name_)
        {
          duplicate = true;
        }
      }
      if (duplicate)
      {
        break;
      }
      names.pushBack(item->product_name_);
    }
    BOOST_TEST(!duplicate);
  }
}

BOOST_AUTO_TEST_CASE(EmptyBase)
{
  karpenko::FoodBase base("empty");
  karpenko::List< karpenko::MealSolution > solutions = karpenko::KnapsackSolver::solve(base, 100, 10);
  BOOST_TEST(solutions.size() == 0);
}

BOOST_AUTO_TEST_CASE(SingleProductMatch)
{
  karpenko::FoodBase base("test");
  base.addProduct(karpenko::Product("buckwheat", 12, 3, 68, 330));
  karpenko::List< karpenko::MealSolution > solutions = karpenko::KnapsackSolver::solve(base, 330, 12);
  BOOST_TEST(solutions.size() > 0);
  bool found = false;
  for (auto sol = solutions.begin(); sol != solutions.end(); ++sol)
  {
    if (sol->items_.size() == 1 && sol->items_.begin()->product_name_ == "buckwheat")
    {
      found = true;
      break;
    }
  }
  BOOST_TEST(found);
}

BOOST_AUTO_TEST_SUITE_END()
