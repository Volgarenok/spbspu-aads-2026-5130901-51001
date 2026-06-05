#define BOOST_TEST_MODULE StorageTests
#include <boost/test/included/unit_test.hpp>
#include <string>
#include "types.hpp"

namespace losev {

void saveData(const std::string& filename);
void loadData(const std::string& filename);

} // namespace losev

BOOST_AUTO_TEST_SUITE(storage_tests)

BOOST_AUTO_TEST_CASE(save_and_load_empty)
{
  const std::string filename = "test_empty.txt";

  losev::saveData(filename);
  losev::loadData(filename);

  BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END()
