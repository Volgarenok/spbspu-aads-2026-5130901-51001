#include <iostream>
#include <new>
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
    printTransposed(sequences);
    
    List< int > sums = calculateSums(sequences);
    printSums(sums);
    
    return 0;
  }
  catch (const std::bad_alloc& e)
  {
    std::cerr << "memory error: " << e.what() << "\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "error\n";
    return 1;
  }
}
