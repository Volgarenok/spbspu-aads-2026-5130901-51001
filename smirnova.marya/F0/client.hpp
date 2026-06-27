#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

namespace smirnova
{
  struct Bank;

  class Client
  {
  public:
    std::string name;
    long long loan;
    long long income;
    int termMonths;

    Client() = default;
    Client(const std::string& n, long long l, long long i, int t):
      name(n),
      loan(l),
      income(i),
      termMonths(t)
    {}

    double getRisk() const;
    double expectedProfit(const Bank& bank) const;
    double score(const Bank& bank) const;
  };
}

#endif

