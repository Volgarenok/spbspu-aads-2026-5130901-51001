#include <boost/test/unit_test.hpp>
#include "KnapsackSolver.hpp"

using namespace karpenko;

BOOST_AUTO_TEST_SUITE(KnapsackSolverTests)

BOOST_AUTO_TEST_CASE(SolveBasic) {
    FoodBase base("test");
    base.addProduct(Product("chicken", 23, 1.5, 0, 110));
    base.addProduct(Product("buckwheat", 12, 3, 68, 330));
    base.addProduct(Product("broccoli", 2.8, 0.4, 4, 34));
    auto solutions = KnapsackSolver::solve(base, 700, 35);
    BOOST_TEST(solutions.size() > 0);
    BOOST_TEST(solutions.size() <= 3);
    for (auto sol = solutions.begin(); sol != solutions.end(); ++sol) {
        BOOST_TEST(sol->totalCalories > 0);
        BOOST_TEST(sol->totalProtein > 0);
    }
}

BOOST_AUTO_TEST_CASE(NoDuplicateProducts) {
    FoodBase base("test");
    base.addProduct(Product("chicken", 23, 1.5, 0, 110));
    base.addProduct(Product("buckwheat", 12, 3, 68, 330));
    auto solutions = KnapsackSolver::solve(base, 700, 35);
    for (auto sol = solutions.begin(); sol != solutions.end(); ++sol) {
        List<std::string> names;
        bool duplicate = false;
        for (auto item = sol->items.begin(); item != sol->items.end(); ++item) {
            for (auto n = names.begin(); n != names.end(); ++n) {
                if (*n == item->productName) duplicate = true;
            }
            if (duplicate) break;
            names.push_back(item->productName);
        }
        BOOST_TEST(!duplicate);
    }
}

BOOST_AUTO_TEST_CASE(EmptyBase) {
    FoodBase base("empty");
    auto solutions = KnapsackSolver::solve(base, 100, 10);
    BOOST_TEST(solutions.size() == 0);
}

BOOST_AUTO_TEST_CASE(SingleProductMatch) {
    FoodBase base("test");
    base.addProduct(Product("buckwheat", 12, 3, 68, 330));
    auto solutions = KnapsackSolver::solve(base, 330, 12);
    BOOST_TEST(solutions.size() > 0);
    bool found = false;
    for (auto sol = solutions.begin(); sol != solutions.end(); ++sol) {
        if (sol->items.size() == 1 && sol->items.begin()->productName == "buckwheat") {
            found = true;
            break;
        }
    }
    BOOST_TEST(found);
}

BOOST_AUTO_TEST_SUITE_END()
