#define BOOST_TEST_MODULE CryptoTests
#include <boost/test/included/unit_test.hpp>
#include <string>

namespace losev {

std::string xorEncrypt(const std::string& input, char key);

} // namespace losev

BOOST_AUTO_TEST_SUITE(xor_encrypt_tests)

BOOST_AUTO_TEST_CASE(encrypt_decrypt)
{
  std::string original = "1234";
  char key = 0x5A;

  std::string encrypted = losev::xorEncrypt(original, key);
  std::string decrypted = losev::xorEncrypt(encrypted, key);

  BOOST_TEST(decrypted == original);
}

BOOST_AUTO_TEST_CASE(empty_string)
{
  std::string original = "";
  char key = 0x5A;

  std::string encrypted = losev::xorEncrypt(original, key);

  BOOST_TEST(encrypted.empty());
}

BOOST_AUTO_TEST_CASE(different_key)
{
  std::string original = "password";
  char key1 = 0x5A;
  char key2 = 0x3C;

  std::string encrypted1 = losev::xorEncrypt(original, key1);
  std::string encrypted2 = losev::xorEncrypt(original, key2);

  BOOST_TEST(encrypted1 != encrypted2);
}

BOOST_AUTO_TEST_SUITE_END()
