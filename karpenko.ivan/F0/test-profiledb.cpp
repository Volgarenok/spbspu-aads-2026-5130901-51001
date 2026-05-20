#include <boost/test/unit_test.hpp>

#include "ProfileDB.hpp"

BOOST_AUTO_TEST_SUITE(ProfileDBTests)

BOOST_AUTO_TEST_CASE(AddProfile)
{
  karpenko::ProfileDB db;
  karpenko::Profile p("John", 180, 75, "mass_gain", 3);
  BOOST_TEST(db.addProfile(p));
  BOOST_TEST(db.hasProfile("John"));
  BOOST_TEST(!db.addProfile(p));
  BOOST_TEST(db.size() == 1);
}

BOOST_AUTO_TEST_CASE(RemoveProfile)
{
  karpenko::ProfileDB db;
  db.addProfile(karpenko::Profile("John", 180, 75, "mass_gain", 3));
  BOOST_TEST(db.removeProfile("John"));
  BOOST_TEST(!db.hasProfile("John"));
  BOOST_TEST(db.size() == 0);
  BOOST_TEST(!db.removeProfile("none"));
}

BOOST_AUTO_TEST_CASE(GetProfile)
{
  karpenko::ProfileDB db;
  karpenko::Profile p("John", 180, 75, "mass_gain", 3);
  db.addProfile(p);
  karpenko::Profile* found = db.getProfile("John");
  BOOST_REQUIRE(found != nullptr);
  BOOST_TEST(found->getName() == "John");
  const karpenko::Profile* const_found = db.getProfile("John");
  BOOST_TEST(const_found->getHeight() == 180);
  BOOST_TEST(db.getProfile("none") == nullptr);
}

BOOST_AUTO_TEST_CASE(GetAllProfiles)
{
  karpenko::ProfileDB db;
  db.addProfile(karpenko::Profile("B", 170, 70, "maintenance", 2));
  db.addProfile(karpenko::Profile("A", 180, 80, "mass_gain", 3));
  karpenko::List< karpenko::Profile > list = db.getAllProfiles();
  BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(SerializeDeserialize)
{
  karpenko::ProfileDB db1;
  db1.addProfile(karpenko::Profile("John", 180, 75, "mass_gain", 3));
  std::string ser = db1.serialize();
  karpenko::ProfileDB db2;
  db2.deserialize(ser);
  BOOST_TEST(db2.hasProfile("John"));
  BOOST_TEST(db2.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
