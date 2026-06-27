#include "client.hpp"
#include "bank.hpp"

namespace smirnova
{
  double Client::getRisk() const
  {
      double dti = static_cast< double >(loan) / (income + 1.0);
      double risk = dti * 30.0;
      if (risk > 100.0)
      {
          risk = 100.0;
      }
      return risk;
  }

  double Client::expectedProfit(const Bank& bank) const
  {
      double rate = bank.rate + getRisk() / 1000.0;
      double termFactor = static_cast< double >(termMonths) / bank.maxTermMonths;
      double totalPay = loan * rate;
      totalPay *= (1.0 + termFactor * 0.2);
      return totalPay - loan;
  }

  double Client::score(const Bank& bank) const
  {
      double profit = expectedProfit(bank);
      double riskPenalty = (100.0 - getRisk()) / 100.0;
      double termPenalty = 1.0 / (1.0 + static_cast< double >(termMonths) / 12.0);
      return profit * riskPenalty * termPenalty;
  }
}

