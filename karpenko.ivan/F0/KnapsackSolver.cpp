#include "KnapsackSolver.hpp"
#include <cmath>
#include <climits>

namespace karpenko {

static const int GRAMS_STEPS[] = {50, 100, 150, 200, 250, 300};
static const int NUM_STEPS = 6;

static double deviation(double cal, double prot, double tCal, double tProt) {
    double d = 0.0;
    if (tCal > 0) d += std::fabs(cal - tCal) / tCal;
    if (tProt > 0) d += std::fabs(prot - tProt) / tProt;
    return d;
}

List<MealSolution> KnapsackSolver::solve(const FoodBase& base, double targetCal, double targetProtein) {
    List<MealSolution> result;

    struct Item {
        std::string productName;
        int grams;
        double calories;
        double protein;
    };
    List<Item> items;
    List<Product> allProducts = base.getAllProducts();
    for (auto pit = allProducts.begin(); pit != allProducts.end(); ++pit) {
        const Product& prod = *pit;
        for (int i = 0; i < NUM_STEPS; ++i) {
            int g = GRAMS_STEPS[i];
            Item it;
            it.productName = prod.getName();
            it.grams = g;
            it.calories = prod.calculateCalories(g);
            it.protein = prod.calculateProteins(g);
            items.push_back(it);
        }
    }

    int itemCount = 0;
    Item* arr = new Item[items.size()];
    for (auto it = items.begin(); it != items.end(); ++it) {
        arr[itemCount++] = *it;
    }

    struct Candidate {
        double cal;
        double prot;
        double dev;
        int idx1, idx2, idx3;
        Candidate() : cal(0), prot(0), dev(1e9), idx1(-1), idx2(-1), idx3(-1) {}
    };
    List<Candidate> candidates;

    for (int i = 0; i < itemCount; ++i) {
        double cal = arr[i].calories;
        double prot = arr[i].protein;
        double dev = deviation(cal, prot, targetCal, targetProtein);
        Candidate cand;
        cand.cal = cal; cand.prot = prot; cand.dev = dev;
        cand.idx1 = i;
        candidates.push_back(cand);
    }

    for (int i = 0; i < itemCount; ++i) {
        for (int j = i+1; j < itemCount; ++j) {
            if (arr[i].productName == arr[j].productName) continue;
            double cal = arr[i].calories + arr[j].calories;
            double prot = arr[i].protein + arr[j].protein;
            double dev = deviation(cal, prot, targetCal, targetProtein);
            Candidate cand;
            cand.cal = cal; cand.prot = prot; cand.dev = dev;
            cand.idx1 = i; cand.idx2 = j;
            candidates.push_back(cand);
        }
    }

    for (int i = 0; i < itemCount; ++i) {
        for (int j = i+1; j < itemCount; ++j) {
            if (arr[i].productName == arr[j].productName) continue;
            for (int k = j+1; k < itemCount; ++k) {
                if (arr[i].productName == arr[k].productName ||
                    arr[j].productName == arr[k].productName) continue;
                double cal = arr[i].calories + arr[j].calories + arr[k].calories;
                double prot = arr[i].protein + arr[j].protein + arr[k].protein;
                double dev = deviation(cal, prot, targetCal, targetProtein);
                Candidate cand;
                cand.cal = cal; cand.prot = prot; cand.dev = dev;
                cand.idx1 = i; cand.idx2 = j; cand.idx3 = k;
                candidates.push_back(cand);
            }
        }
    }

    int candCount = 0;
    for (auto it = candidates.begin(); it != candidates.end(); ++it) ++candCount;
    Candidate* candArr = new Candidate[candCount];
    int pos = 0;
    for (auto it = candidates.begin(); it != candidates.end(); ++it) {
        candArr[pos++] = *it;
    }

    for (int i = 0; i < candCount-1; ++i) {
        for (int j = 0; j < candCount-i-1; ++j) {
            if (candArr[j].dev > candArr[j+1].dev) {
                Candidate tmp = candArr[j];
                candArr[j] = candArr[j+1];
                candArr[j+1] = tmp;
            }
        }
    }

    int limit = (candCount < 3) ? candCount : 3;
    for (int n = 0; n < limit; ++n) {
        MealSolution sol;
        sol.totalCalories = candArr[n].cal;
        sol.totalProtein = candArr[n].prot;
        if (candArr[n].idx1 != -1) {
            KnapItem it;
            it.productName = arr[candArr[n].idx1].productName;
            it.grams = arr[candArr[n].idx1].grams;
            it.calories = arr[candArr[n].idx1].calories;
            it.protein = arr[candArr[n].idx1].protein;
            sol.items.push_back(it);
        }
        if (candArr[n].idx2 != -1) {
            KnapItem it;
            it.productName = arr[candArr[n].idx2].productName;
            it.grams = arr[candArr[n].idx2].grams;
            it.calories = arr[candArr[n].idx2].calories;
            it.protein = arr[candArr[n].idx2].protein;
            sol.items.push_back(it);
        }
        if (candArr[n].idx3 != -1) {
            KnapItem it;
            it.productName = arr[candArr[n].idx3].productName;
            it.grams = arr[candArr[n].idx3].grams;
            it.calories = arr[candArr[n].idx3].calories;
            it.protein = arr[candArr[n].idx3].protein;
            sol.items.push_back(it);
        }
        result.push_back(sol);
    }

    delete[] arr;
    delete[] candArr;
    return result;
}

}

