#include <boost/test/unit_test.hpp>
#include "BaseDB.hpp"

using namespace karpenko;

BOOST_AUTO_TEST_SUITE(BaseDBTests)

BOOST_AUTO_TEST_CASE(AddBase) {
    BaseDB db;
    FoodBase fb("mybase");
    BOOST_TEST(db.addBase(fb));
    BOOST_TEST(db.hasBase("mybase"));
    BOOST_TEST(!db.addBase(fb));
    BOOST_TEST(db.size() == 1);
}

BOOST_AUTO_TEST_CASE(RemoveBase) {
    BaseDB db;
    db.addBase(FoodBase("mybase"));
    BOOST_TEST(db.removeBase("mybase"));
    BOOST_TEST(!db.hasBase("mybase"));
    BOOST_TEST(db.size() == 0);
    BOOST_TEST(!db.removeBase("none"));
}

BOOST_AUTO_TEST_CASE(GetBase) {
    BaseDB db;
    FoodBase fb("mybase");
    db.addBase(fb);
    FoodBase* found = db.getBase("mybase");
    BOOST_REQUIRE(found != nullptr);
    BOOST_TEST(found->getName() == "mybase");
    const FoodBase* constFound = db.getBase("mybase");
    BOOST_TEST(constFound->size() == 0);
    BOOST_TEST(db.getBase("none") == nullptr);
}

BOOST_AUTO_TEST_CASE(GetAllBases) {
    BaseDB db;
    db.addBase(FoodBase("Bbase"));
    db.addBase(FoodBase("Abase"));
    auto list = db.getAllBases();
    BOOST_TEST(list.size() == 2);
    bool hasA = false, hasB = false;
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::string name = (*it)->getName();
        if (name == "Abase") hasA = true;
        if (name == "Bbase") hasB = true;
    }
    BOOST_TEST(hasA);
    BOOST_TEST(hasB);
}

BOOST_AUTO_TEST_CASE(SerializeDeserialize) {
    BaseDB db1;
    db1.addBase(FoodBase("mybase"));
    std::string ser = db1.serialize();
    BaseDB db2;
    db2.deserialize(ser);
    BOOST_TEST(db2.hasBase("mybase"));
    BOOST_TEST(db2.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
