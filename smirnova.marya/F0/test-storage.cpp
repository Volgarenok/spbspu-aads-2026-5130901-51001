#include <boost/test/unit_test.hpp>
#include "storage.hpp"

BOOST_AUTO_TEST_SUITE(StorageTests)

BOOST_AUTO_TEST_CASE(loads_banks_from_file)
{
  smirnova::CommandContext ctx;
  auto& map = ctx.banks();
  BOOST_CHECK(map.begin() != map.end());
}

BOOST_AUTO_TEST_CASE(get_bank_existing)
{
  smirnova::CommandContext ctx;
  auto& map = ctx.banks();
  auto it = map.begin();
  std::string name = it->key;
  smirnova::Bank* b = ctx.getBank(name);
  BOOST_REQUIRE(b != nullptr);
  BOOST_CHECK_EQUAL(b->name, name);
}

BOOST_AUTO_TEST_CASE(get_bank_missing)
{
  smirnova::CommandContext ctx;
  BOOST_CHECK(ctx.getBank("__NO_BANK__") == nullptr);
}

BOOST_AUTO_TEST_CASE(insert_via_context)
{
  smirnova::CommandContext ctx;

  smirnova::Bank b;
  b.name = "TEST";
  b.limit = 1000;
  b.rate = 1.2;
  b.maxTermMonths = 12;

  ctx.banks().insert("TEST", b);

  BOOST_CHECK(ctx.getBank("TEST") != nullptr);
}

BOOST_AUTO_TEST_SUITE_END()

