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
      std::cerr << "error: cannot open file\n";
      return 1;
    }
    input = &file;
  }

  Stack< ll_t > results;
  string line;

  try
  {
    Calculator calc;
    while (getline(*input, line))
    {
      if (line.empty()) continue;

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
  return 0;
}
