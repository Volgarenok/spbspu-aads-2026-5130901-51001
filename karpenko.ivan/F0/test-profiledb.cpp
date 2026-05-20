#include <boost/test/unit_test.hpp>
#include "ProfileDB.hpp"

using namespace karpenko;

BOOST_AUTO_TEST_SUITE(ProfileDBTests)

BOOST_AUTO_TEST_CASE(AddProfile) {
    ProfileDB db;
    Profile p("John", 180, 75, "mass_gain", 3);
    BOOST_TEST(db.addProfile(p));
    BOOST_TEST(db.hasProfile("John"));
    BOOST_TEST(!db.addProfile(p));
    BOOST_TEST(db.size() == 1);
}

BOOST_AUTO_TEST_CASE(RemoveProfile) {
    ProfileDB db;
    db.addProfile(Profile("John", 180, 75, "mass_gain", 3));
    BOOST_TEST(db.removeProfile("John"));
    BOOST_TEST(!db.hasProfile("John"));
    BOOST_TEST(db.size() == 0);
    BOOST_TEST(!db.removeProfile("none"));
}

BOOST_AUTO_TEST_CASE(GetProfile) {
    ProfileDB db;
    Profile p("John", 180, 75, "mass_gain", 3);
    db.addProfile(p);
    Profile* found = db.getProfile("John");
    BOOST_REQUIRE(found != nullptr);
    BOOST_TEST(found->getName() == "John");
    const Profile* constFound = db.getProfile("John");
    BOOST_TEST(constFound->getHeight() == 180);
    BOOST_TEST(db.getProfile("none") == nullptr);
}

BOOST_AUTO_TEST_CASE(GetAllProfiles) {
    ProfileDB db;
    db.addProfile(Profile("B", 170, 70, "maintenance", 2));
    db.addProfile(Profile("A", 180, 80, "mass_gain", 3));
    auto list = db.getAllProfiles();
    BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(SerializeDeserialize) {
    ProfileDB db1;
    db1.addProfile(Profile("John", 180, 75, "mass_gain", 3));
    std::string ser = db1.serialize();
    ProfileDB db2;
    db2.deserialize(ser);
    BOOST_TEST(db2.hasProfile("John"));
    BOOST_TEST(db2.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
