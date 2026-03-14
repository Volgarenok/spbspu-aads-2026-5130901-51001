#include <iostream>
#include <new>
#include <limits>
#include "list.hpp"
#include "io.hpp"

int main()
{
  using namespace shevchenko;

  try
  {
    List< Seq > sequences = readSequences();

    if (sequences.empty())
    {
      std::cout << "0\n";
      return 0;
    }

    printNames(sequences);

    bool hasNumbers = false;
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
    {
      if (!(*it).second.empty())
      {
        hasNumbers = true;
        break;
      }
    }
    if (!hasNumbers)
    {
      std::cout << "0\n";
      return 0;
    }

    printTransposed(sequences);

    List< size_t > sums = calculateSums(sequences);
    printSums(sums);
    return 0;
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << "memory error: " << e.what() << '\n';
    return 1;
  }
  catch (...)
  {
    std::cerr << "error\n";
    return 1;
  }
}
