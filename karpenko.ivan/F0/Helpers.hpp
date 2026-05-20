#ifndef KARPENKO_HELPERS_HPP
#define KARPENKO_HELPERS_HPP

#include <string>

#include "../common/list.hpp"

namespace karpenko
{
namespace Helpers
{

std::string trim(const std::string& str);
bool isValidDate(const std::string& date);
bool isValidMealType(const std::string& meal);
bool isValidGoal(const std::string& goal);
std::string toLower(const std::string& str);
List< std::string > split(const std::string& str, char delimiter);

}
}

#endif
