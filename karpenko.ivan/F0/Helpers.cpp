#include "Helpers.hpp"
#include <cctype>
#include <cstdio>
#include <sstream>

namespace karpenko {
namespace Helpers {

std::string trim(const std::string& str) {
    size_t start = 0, end = str.length();
    while (start < end && std::isspace(str[start])) ++start;
    while (end > start && std::isspace(str[end-1])) --end;
    return str.substr(start, end - start);
}

bool isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    int y, m, d;
    if (sscanf(date.c_str(), "%d-%d-%d", &y, &m, &d) != 3) return false;
    if (y < 2000 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) return false;
    if (m == 2) {
        bool leap = (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
        if (d > (leap ? 29 : 28)) return false;
    }
    return true;
}

bool isValidMealType(const std::string& meal) {
    return meal == "breakfast" || meal == "lunch" || meal == "dinner" || meal == "snack";
}

bool isValidGoal(const std::string& goal) {
    return goal == "weight_loss" || goal == "maintenance" || goal == "mass_gain";
}

std::string toLower(const std::string& str) {
    std::string res = str;
    for (size_t i = 0; i < res.size(); ++i) res[i] = std::tolower(res[i]);
    return res;
}

karpenko::List<std::string> split(const std::string& str, char delimiter) {
    karpenko::List<std::string> result;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

}
}
