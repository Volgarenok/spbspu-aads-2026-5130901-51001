#include <boost/test/unit_test.hpp>
#include "Profile.hpp"

using namespace karpenko;

BOOST_AUTO_TEST_SUITE(ProfileTests)

BOOST_AUTO_TEST_CASE(ConstructorDefault) {
    Profile p;
    BOOST_TEST(p.getName() == "");
    BOOST_TEST(p.getHeight() == 0.0);
    BOOST_TEST(p.getWeight() == 0.0);
    BOOST_TEST(p.getGoal() == "");
    BOOST_TEST(p.getActivity() == 1);
    BOOST_TEST(p.getTargetCalories() == 0.0);
    BOOST_TEST(p.getTargetProtein() == 0.0);
}

BOOST_AUTO_TEST_CASE(ConstructorWithParams) {
    Profile p("John", 180.0, 75.0, "mass_gain", 3);
    BOOST_TEST(p.getName() == "John");
    BOOST_TEST(p.getHeight() == 180.0);
    BOOST_TEST(p.getWeight() == 75.0);
    BOOST_TEST(p.getGoal() == "mass_gain");
    BOOST_TEST(p.getActivity() == 3);
    BOOST_TEST(p.getTargetCalories() > 0);
    BOOST_TEST(p.getTargetProtein() > 0);
}

BOOST_AUTO_TEST_CASE(CalculateTargets) {
    Profile p("Mary", 165.0, 60.0, "weight_loss", 2);
    BOOST_TEST(p.getTargetCalories() < 2500);
    BOOST_TEST(p.getTargetProtein() > 50);
}

BOOST_AUTO_TEST_CASE(ToString) {
    Profile p("John", 180.0, 75.0, "mass_gain", 3);
    std::string str = p.toString();
    BOOST_TEST(str.find("John") != std::string::npos);
    BOOST_TEST(str.find("mass_gain") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(SerializeDeserialize) {
    Profile p1("John", 180.0, 75.0, "mass_gain", 3);
    std::string ser = p1.serialize();
    Profile p2 = Profile::deserialize(ser);
    BOOST_TEST(p2.getName() == "John");
    BOOST_TEST(p2.getHeight() == 180.0);
    BOOST_TEST(p2.getWeight() == 75.0);
    BOOST_TEST(p2.getGoal() == "mass_gain");
    BOOST_TEST(p2.getActivity() == 3);
    BOOST_TEST(p2.getTargetCalories() == p1.getTargetCalories());
    BOOST_TEST(p2.getTargetProtein() == p1.getTargetProtein());
}

BOOST_AUTO_TEST_SUITE_END()
