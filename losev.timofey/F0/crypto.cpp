#include "crypto.hpp"

namespace losev {

std::string xorEncrypt(const std::string& input, char key)
{
  std::string result = input;

  for (std::size_t i = 0; i < result.size(); ++i)
  {
    result[i] = result[i] ^ key;
  }

  return result;
}

}
