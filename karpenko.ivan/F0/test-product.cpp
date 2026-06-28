#include <boost/test/unit_test.hpp>

#include "Product.hpp"

BOOST_AUTO_TEST_SUITE(ProductTests)

BOOST_AUTO_TEST_CASE(ConstructorDefault)
{
  karpenko::Product p;
  BOOST_TEST(p.getName() == "");
  BOOST_TEST(p.getProteins() == 0.0);
  BOOST_TEST(p.getFats() == 0.0);
  BOOST_TEST(p.getCarbs() == 0.0);
  BOOST_TEST(p.getCalories() == 0.0);
}

BOOST_AUTO_TEST_CASE(ConstructorWithParams)
{
  karpenko::Product p("chicken", 23.0, 1.5, 0.0, 110.0);
  BOOST_TEST(p.getName() == "chicken");
  BOOST_TEST(p.getProteins() == 23.0);
  BOOST_TEST(p.getFats() == 1.5);
  BOOST_TEST(p.getCarbs() == 0.0);
  BOOST_TEST(p.getCalories() == 110.0);
}

BOOST_AUTO_TEST_CASE(SetName)
{
  karpenko::Product p;
  p.setName("beef");
  BOOST_TEST(p.getName() == "beef");
}

BOOST_AUTO_TEST_CASE(CalculateCalories)
{
  karpenko::Product p("chicken", 23.0, 1.5, 0.0, 110.0);
  BOOST_TEST(p.calculateCalories(150) == 165.0);
  BOOST_TEST(p.calculateCalories(100) == 110.0);
}

BOOST_AUTO_TEST_CASE(CalculateProteins)
{
  karpenko::Product p("chicken", 23.0, 1.5, 0.0, 110.0);
  BOOST_TEST(p.calculateProteins(150) == 34.5);
  BOOST_TEST(p.calculateProteins(100) == 23.0);
}

BOOST_AUTO_TEST_CASE(ToString)
{
  karpenko::Product p("chicken", 23.0, 1.5, 0.0, 110.0);
  std::string str = p.toString();
  BOOST_TEST(str.find("chicken") != std::string::npos);
  BOOST_TEST(str.find("23.0") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(SerializeDeserialize)
{
  karpenko::Product p1("chicken", 23.0, 1.5, 0.0, 110.0);
  std::string ser = p1.serialize();
  karpenko::Product p2 = karpenko::Product::deserialize(ser);
  BOOST_TEST(p2.getName() == "chicken");
  BOOST_TEST(p2.getProteins() == 23.0);
  BOOST_TEST(p2.getFats() == 1.5);
  BOOST_TEST(p2.getCarbs() == 0.0);
  BOOST_TEST(p2.getCalories() == 110.0);
}

BOOST_AUTO_TEST_SUITE_END()
