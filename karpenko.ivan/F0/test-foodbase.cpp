#include <boost/test/unit_test.hpp>
#include "FoodBase.hpp"

using namespace karpenko;

BOOST_AUTO_TEST_SUITE(FoodBaseTests)

BOOST_AUTO_TEST_CASE(ConstructorDefault) {
    FoodBase fb;
    BOOST_TEST(fb.getName() == "");
    BOOST_TEST(fb.size() == 0);
}

BOOST_AUTO_TEST_CASE(ConstructorWithName) {
    FoodBase fb("mybase");
    BOOST_TEST(fb.getName() == "mybase");
    BOOST_TEST(fb.size() == 0);
}

BOOST_AUTO_TEST_CASE(AddProduct) {
    FoodBase fb("test");
    Product p("apple", 0.3, 0.2, 14.0, 52.0);
    BOOST_TEST(fb.addProduct(p));
    BOOST_TEST(fb.containsProduct("apple"));
    BOOST_TEST(fb.size() == 1);
    BOOST_TEST(!fb.addProduct(p));
}

BOOST_AUTO_TEST_CASE(RemoveProduct) {
    FoodBase fb("test");
    Product p("apple", 0.3, 0.2, 14.0, 52.0);
    fb.addProduct(p);
    BOOST_TEST(fb.removeProduct("apple"));
    BOOST_TEST(!fb.containsProduct("apple"));
    BOOST_TEST(fb.size() == 0);
    BOOST_TEST(!fb.removeProduct("nonexistent"));
}

BOOST_AUTO_TEST_CASE(GetProduct) {
    FoodBase fb("test");
    Product p("banana", 1.1, 0.3, 23.0, 89.0);
    fb.addProduct(p);
    const Product* found = fb.getProduct("banana");
    BOOST_REQUIRE(found != nullptr);
    BOOST_TEST(found->getName() == "banana");
    BOOST_TEST(found->getCalories() == 89.0);
    BOOST_TEST(fb.getProduct("none") == nullptr);
}

BOOST_AUTO_TEST_CASE(GetAllProducts) {
    FoodBase fb("test");
    fb.addProduct(Product("a", 0,0,0,0));
    fb.addProduct(Product("b", 0,0,0,0));
    auto prods = fb.getAllProducts();
    BOOST_TEST(prods.size() == 2);
}

BOOST_AUTO_TEST_CASE(SerializeDeserialize) {
    FoodBase fb("mybase");
    fb.addProduct(Product("cheese", 25.0, 33.0, 2.0, 402.0));
    std::string ser = fb.serialize();
    FoodBase fb2 = FoodBase::deserialize(ser);
    BOOST_TEST(fb2.getName() == "mybase");
    BOOST_TEST(fb2.containsProduct("cheese"));
    BOOST_TEST(fb2.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
