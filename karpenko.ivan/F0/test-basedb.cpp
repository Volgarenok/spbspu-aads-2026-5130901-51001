#include <boost/test/unit_test.hpp>

#include "BaseDB.hpp"

BOOST_AUTO_TEST_SUITE(BaseDBTests)

BOOST_AUTO_TEST_CASE(AddBase)
{
  karpenko::BaseDB db;
  karpenko::FoodBase fb("mybase");
  BOOST_TEST(db.addBase(fb));
  BOOST_TEST(db.hasBase("mybase"));
  BOOST_TEST(!db.addBase(fb));
  BOOST_TEST(db.size() == 1);
}

BOOST_AUTO_TEST_CASE(RemoveBase)
{
  karpenko::BaseDB db;
  db.addBase(karpenko::FoodBase("mybase"));
  BOOST_TEST(db.removeBase("mybase"));
  BOOST_TEST(!db.hasBase("mybase"));
  BOOST_TEST(db.size() == 0);
  BOOST_TEST(!db.removeBase("none"));
}

BOOST_AUTO_TEST_CASE(GetBase)
{
  karpenko::BaseDB db;
  karpenko::FoodBase fb("mybase");
  db.addBase(fb);
  karpenko::FoodBase* found = db.getBase("mybase");
  BOOST_REQUIRE(found != nullptr);
  BOOST_TEST(found->getName() == "mybase");
  const karpenko::FoodBase* const_found = db.getBase("mybase");
  BOOST_TEST(const_found->size() == 0);
  BOOST_TEST(db.getBase("none") == nullptr);
}

BOOST_AUTO_TEST_CASE(GetAllBases)
{
  karpenko::BaseDB db;
  db.addBase(karpenko::FoodBase("Bbase"));
  db.addBase(karpenko::FoodBase("Abase"));
  karpenko::List< const karpenko::FoodBase* > list = db.getAllBases();
  BOOST_TEST(list.size() == 2);
  bool has_a = false;
  bool has_b = false;
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    std::string name = (*it)->getName();
    if (name == "Abase")
    {
      has_a = true;
    }
    if (name == "Bbase")
    {
      has_b = true;
    }
  }
  BOOST_TEST(has_a);
  BOOST_TEST(has_b);
}

BOOST_AUTO_TEST_CASE(SerializeDeserialize)
{
  karpenko::BaseDB db1;
  db1.addBase(karpenko::FoodBase("mybase"));
  std::string ser = db1.serialize();
  karpenko::BaseDB db2;
  db2.deserialize(ser);
  BOOST_TEST(db2.hasBase("mybase"));
  BOOST_TEST(db2.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
