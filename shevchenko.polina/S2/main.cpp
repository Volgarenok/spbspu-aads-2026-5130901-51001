#include <iostream>
#include <fstream>
#include <string>
#include "calculator.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  using namespace shevchenko;
  
  istream* input = &cin;
  ifstream file;
  
  if (argc >= 2)
  {
    file.open(argv[1]);
    if (!file)
    {
      cerr << "error: cannot open file\n";
      return 1;
    }
    input = &file;
  }
  
  List<ll_t> results;
  string line;
  
  try
  {
    Calculator calc;
    while (getline(*input, line))
    {
      if (line.empty()) continue;
      
      ll_t result = calc.evaluate(line);
      results.pushBack(result);
    }
    
    if (results.empty())
    {
      cout << endl;
      return 0;
    }
    
    bool first = true;
    auto it = results.last();
    while (true)
    {
      if (!first)
      {
        cout << " ";
      }
      cout << *it;
      first = false;
      
      if (it == results.begin())
      {
        break;
      }
      --it;
    }
    cout << "\n";
  }
  catch (const overflow_error& e)
  {
    cerr << e.what() << "\n";
    return 1;
  }
  catch (const logic_error& e)
  {
    cerr << e.what() << "\n";
    return 1;
  }
  catch (const exception& e)
  {
    cerr << e.what() << "\n";
    return 1;
  }
  catch (...)
  {
    cerr << "unknown error\n";
    return 1;
  }
  
  return 0;
}
