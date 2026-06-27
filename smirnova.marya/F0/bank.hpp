#ifndef BANK_HPP
#define BANK_HPP

#include <string>
#include "../common/vector.hpp"
#include "client.hpp"

namespace smirnova
{
  struct Bank
  {
    std::string name;

    double rate = 1.0;
    long long limit = 0;
    int maxTermMonths = 0;

    double minScore = 0.2;

    Vector< Client > clients;

    Vector< std::string > lastApproved;
    Vector< std::string > approved;
    Vector< std::string > rejected;
  };
}

#endif

