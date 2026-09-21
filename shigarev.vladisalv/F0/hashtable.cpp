#include "hashtable.hpp"

bool yafu::detail::isPrime(size_t value)
{
  if (value < 2) {
    return false;
  }
  if ((value % 2) == 0) {
    return value == 2;
  }
  for (size_t divisor = firstOddPrime; (divisor * divisor) <= value; divisor += 2) {
    if ((value % divisor) == 0) {
      return false;
    }
  }
  return true;
}

size_t yafu::detail::getNextPrime(size_t value)
{
  size_t candidate = (value < minimalTableCapacity) ? minimalTableCapacity : value;
  if ((candidate % 2) == 0) {
    ++candidate;
  }
  while (!isPrime(candidate)) {
    candidate += 2;
  }
  return candidate;
}

unsigned long long yafu::detail::getPolynomialHash(const String& key, unsigned long long base)
{
  unsigned long long hash = 0;
  for (size_t i = 0; i < key.getLength(); ++i) {
    hash = hash * base + static_cast< unsigned char >(key[i]);
  }
  return hash;
}
