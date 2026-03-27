#include <iostream>
#include <fstream>
#include <string>
#include "calculator.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  using namespace shevchenko;
<<<<<<< HEAD

  istream* input = &cin;
  ifstream file;

=======
  
  istream* input = &cin;
  ifstream file;
  
>>>>>>> 4ca3688 (test)
  if (argc >= 2)
  {
    file.open(argv[1]);
    if (!file)
    {
<<<<<<< HEAD
      cerr << "error: cannot open file\n";
=======
      std::cerr << "error: cannot open file\n";
>>>>>>> 4ca3688 (test)
      return 1;
    }
    input = &file;
  }
<<<<<<< HEAD

  List<ll_t> results;
  string line;

=======
  
  Stack< ll_t > results;
  string line;
  
>>>>>>> 4ca3688 (test)
  try
  {
    Calculator calc;
    while (getline(*input, line))
    {
      if (line.empty()) continue;
<<<<<<< HEAD

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

=======
      
      ll_t result = calc.evaluate(line);
      results.push(result);
    }
    
    Stack< ll_t > reversed;
    while (!results.empty())
    {
      reversed.push(results.pop());
    }
    
    bool first = true;
    while (!reversed.empty())
    {
      if (!first) cout << ' ';
      cout << reversed.pop();
      first = false;
    }
    cout << '\n';
  }
  catch (const exception& e)
  {
    cerr << e.what() << '\n';
    return 1;
  }
>>>>>>> 4ca3688 (test)
  return 0;
}
