#include <iostream>
#include <fstream>
#include <string>
#include "calculator.hpp"

using namespace std;

int main(int argc, char* argv[])
{
  using namespace shevchenko;
<<<<<<< HEAD
<<<<<<< HEAD

  istream* input = &cin;
  ifstream file;

=======

  istream* input = &cin;
  ifstream file;

>>>>>>> 4ca3688 (test)
=======

  istream* input = &cin;
  ifstream file;

>>>>>>> dc1dfea (content)
  if (argc >= 2)
  {
    file.open(argv[1]);
    if (!file)
    {
<<<<<<< HEAD
<<<<<<< HEAD
      cerr << "error: cannot open file\n";
=======
      std::cerr << "error: cannot open file\n";
>>>>>>> 4ca3688 (test)
=======
      cerr << "error: cannot open file\n";
>>>>>>> a3c873f (fix)
      return 1;
    }
    input = &file;
  }
<<<<<<< HEAD
<<<<<<< HEAD

  List<ll_t> results;
  string line;

=======

  Stack< ll_t > results;
  string line;

>>>>>>> 4ca3688 (test)
=======

  List<ll_t> results;
  string line;

>>>>>>> dc1dfea (content)
  try
  {
    Calculator calc;
    while (getline(*input, line))
    {
<<<<<<< HEAD
<<<<<<< HEAD
      if (line.empty()) continue;
<<<<<<< HEAD
<<<<<<< HEAD
=======
      if (line.empty())
      {
        continue;
      }
>>>>>>> a3c873f (fix)

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

=======

>>>>>>> dc1dfea (content)
      ll_t result = calc.evaluate(line);
      results.push(result);
=======
      if (line.empty()) continue;

      ll_t result = calc.evaluate(line);
      results.pushBack(result);
>>>>>>> 8a25341 (add iters)
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
<<<<<<< HEAD
>>>>>>> 4ca3688 (test)
=======
  catch (...)
  {
    cerr << "unknown error\n";
    return 1;
  }

>>>>>>> a3c873f (fix)
  return 0;
}
