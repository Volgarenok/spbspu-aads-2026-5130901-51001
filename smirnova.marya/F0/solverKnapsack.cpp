#include "solverKnapsack.hpp"
#include "client.hpp"

namespace smirnova
{
  struct Item
  {
    std::string name;
    long long loan;
    double profit;
  };

  void solveKnapsack(Bank& bank)
  {
    bank.lastApproved = Vector<std::string>{};
    bank.approved = Vector<std::string>{};
    bank.rejected = Vector<std::string>{};

    Vector< Item > items;

    for (auto it = bank.clients.begin(); it != bank.clients.end(); ++it)
    {
      const Client& c = *it;

      if (c.loan <= 0 || c.termMonths <= 0)
      {
        continue;
      }

      items.pushBack({c.name, c.loan, c.expectedProfit(bank)});
    }

    for (size_t i = 0; i < items.size(); ++i)
    {
      for (size_t j = i + 1; j < items.size(); ++j)
      {
        double di = items[i].profit / static_cast< double >(items[i].loan);
        double dj = items[j].profit / static_cast< double >(items[j].loan);

        if (dj > di)
        {
          auto tmp = items[i];
          items[i] = items[j];
          items[j] = tmp;
        }
      }
    }

    Vector< Item > chosen;
    long long used = 0;

    for (auto it = items.begin(); it != items.end(); ++it)
    {
      if (used + it->loan <= bank.limit)
      {
        used += it->loan;
        chosen.pushBack(*it);
      }
    }

    double total = 0.0;
    double overpay = 0.0;
    double monthly = 0.0;

    for (auto it = chosen.begin(); it != chosen.end(); ++it)
    {
      for (auto cit = bank.clients.begin(); cit != bank.clients.end(); ++cit)
      {
        if (cit->name == it->name)
        {
          double t = cit->loan * bank.rate;
          double o = t - cit->loan;
          double m = t / cit->termMonths;

          total += t;
          overpay += o;
          monthly += m;
          break;
        }
      }
    }

    for (auto it = chosen.begin(); it != chosen.end(); ++it)
    {
      bank.lastApproved.pushBack(it->name);
    }

    for (auto it = bank.clients.begin(); it != bank.clients.end(); ++it)
    {
      bool ok = false;
      for (auto& n : bank.lastApproved)
      {
        if (n == it->name)
        {
          ok = true;
        }
      }

      if (ok)
      {
        bank.approved.pushBack(it->name);
      }
      else
      {
        bank.rejected.pushBack(it->name);
      }
    }
  }
}

