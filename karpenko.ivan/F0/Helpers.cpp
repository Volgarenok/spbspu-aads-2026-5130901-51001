#include "Helpers.hpp"

#include <cctype>
#include <cstdio>
#include <sstream>
#include <unistd.h>
#include <cstdlib>
#include <climits>

namespace karpenko {
  namespace Helpers {

    std::string trim(const std::string& str)
    {
      size_t start = 0;
      size_t end = str.length();
      while (start < end && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
      }
      while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        --end;
      }
      return str.substr(start, end - start);
    }

    bool isValidDate(const std::string& date)
    {
      if (date.length() != 10) return false;
      if (date[4] != '-' || date[7] != '-') return false;
      int year = 0, month = 0, day = 0;
      if (std::sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day) != 3) return false;
      if (year < 2000 || year > 2100) return false;
      if (month < 1 || month > 12) return false;
      if (day < 1 || day > 31) return false;
      if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
      if (month == 2) {
        bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (day > (isLeap ? 29 : 28)) return false;
      }
      return true;
    }

    bool isValidMealType(const std::string& meal)
    {
      return meal == "breakfast"
          || meal == "lunch"
          || meal == "dinner"
          || meal == "snack";
    }

    bool isValidGoal(const std::string& goal)
    {
      return goal == "weight_loss"
          || goal == "maintenance"
          || goal == "mass_gain";
    }

    std::string toLower(const std::string& str)
    {
      std::string result = str;
      for (size_t i = 0; i < result.size(); ++i) {
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
      }
      return result;
    }

    List<std::string> split(const std::string& str, char delimiter)
    {
      List<std::string> result;
      std::stringstream stream(str);
      std::string token;
      while (std::getline(stream, token, delimiter)) {
        result.pushBack(token);
      }
      return result;
    }

    bool isInteractive()
    {
      return isatty(fileno(stdin)) != 0;
    }

    bool parseDouble(const std::string& str, double& out)
    {
      char* end = nullptr;
      double val = std::strtod(str.c_str(), &end);
      if (end == str.c_str() || *end != '\0')
        return false;
      out = val;
      return true;
    }

    bool parseInt(const std::string& str, int& out)
    {
      char* end = nullptr;
      long val = std::strtol(str.c_str(), &end, 10);
      if (end == str.c_str() || *end != '\0' || val < INT_MIN || val > INT_MAX)
        return false;
      out = static_cast<int>(val);
      return true;
    }

  }
}
