#include <boost/test/unit_test.hpp>
#include "../common/list.hpp"
#include <stdexcept>

using namespace karpenko;

BOOST_AUTO_TEST_SUITE(ListTests)

BOOST_AUTO_TEST_CASE(ConstructorDefault) {
    List<int> lst;
    BOOST_TEST(lst.empty());
    BOOST_TEST(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(PushFront) {
    List<int> lst;
    lst.push_front(1);
    lst.push_front(2);
    lst.push_front(3);
    BOOST_TEST(lst.size() == 3);
    BOOST_TEST(lst.front() == 3);
    BOOST_TEST(lst.back() == 1);
    auto it = lst.begin();
    BOOST_TEST(*it == 3);
    ++it;
    BOOST_TEST(*it == 2);
    ++it;
    BOOST_TEST(*it == 1);
}

BOOST_AUTO_TEST_CASE(PushBack) {
    List<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    BOOST_TEST(lst.size() == 3);
    BOOST_TEST(lst.front() == 1);
    BOOST_TEST(lst.back() == 3);
    auto it = lst.begin();
    BOOST_TEST(*it == 1);
    ++it;
    BOOST_TEST(*it == 2);
    ++it;
    BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(PopFront) {
    List<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    lst.pop_front();
    BOOST_TEST(lst.size() == 2);
    BOOST_TEST(lst.front() == 2);
    lst.pop_front();
    BOOST_TEST(lst.size() == 1);
    BOOST_TEST(lst.front() == 3);
    lst.pop_front();
    BOOST_TEST(lst.empty());
    BOOST_CHECK_THROW(lst.pop_front(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(PopBack) {
    List<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    lst.pop_back();
    BOOST_TEST(lst.size() == 2);
    BOOST_TEST(lst.back() == 2);
    lst.pop_back();
    BOOST_TEST(lst.size() == 1);
    BOOST_TEST(lst.back() == 1);
    lst.pop_back();
    BOOST_TEST(lst.empty());
    BOOST_CHECK_THROW(lst.pop_back(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(FrontBackOnEmpty) {
    List<int> lst;
    BOOST_CHECK_THROW(lst.front(), std::out_of_range);
    BOOST_CHECK_THROW(lst.back(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(InsertAfter) {
    List<int> lst;
    lst.push_back(1);
    lst.push_back(3);
    auto it = lst.begin();
    lst.insert_after(it, 2);
    BOOST_TEST(lst.size() == 3);
    auto check = lst.begin();
    BOOST_TEST(*check == 1); ++check;
    BOOST_TEST(*check == 2); ++check;
    BOOST_TEST(*check == 3);
}

BOOST_AUTO_TEST_CASE(InsertAfterEnd) {
    List<int> lst;
    lst.push_back(1);
    auto it = lst.begin();
    lst.insert_after(it, 2);
    BOOST_TEST(lst.size() == 2);
    auto check = lst.begin();
    BOOST_TEST(*check == 1);
    ++check;
    BOOST_TEST(*check == 2);
}

BOOST_AUTO_TEST_CASE(EraseAfter) {
    List<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    auto it = lst.begin();
    lst.erase_after(it);
    BOOST_TEST(lst.size() == 2);
    auto check = lst.begin();
    BOOST_TEST(*check == 1);
    ++check;
    BOOST_TEST(*check == 3);
}

BOOST_AUTO_TEST_CASE(EraseAfterInvalid) {
    List<int> lst;
    lst.push_back(1);
    auto it = lst.begin();
    BOOST_CHECK_THROW(lst.erase_after(it), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Clear) {
    List<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    lst.clear();
    BOOST_TEST(lst.empty());
    BOOST_TEST(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(IteratorTraversal) {
    List<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    int sum = 0;
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        sum += *it;
    }
    BOOST_TEST(sum == 6);
}

BOOST_AUTO_TEST_CASE(ConstIterator) {
    List<int> lst;
    lst.push_back(10);
    lst.push_back(20);
    const auto& constLst = lst;
    int sum = 0;
    for (auto it = constLst.begin(); it != constLst.end(); ++it) {
        sum += *it;
    }
    BOOST_TEST(sum == 30);
}

BOOST_AUTO_TEST_CASE(CopyConstructor) {
    List<int> original;
    original.push_back(1);
    original.push_back(2);
    List<int> copy = original;
    BOOST_TEST(copy.size() == 2);
    auto it = copy.begin();
    BOOST_TEST(*it == 1);
    ++it;
    BOOST_TEST(*it == 2);
    original.push_back(3);
    BOOST_TEST(original.size() == 3);
    BOOST_TEST(copy.size() == 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructor) {
    List<int> original;
    original.push_back(1);
    original.push_back(2);
    List<int> moved = std::move(original);
    BOOST_TEST(moved.size() == 2);
    auto it = moved.begin();
    BOOST_TEST(*it == 1);
    ++it;
    BOOST_TEST(*it == 2);
    BOOST_TEST(original.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssignment) {
    List<int> a, b;
    a.push_back(1);
    a.push_back(2);
    b = a;
    BOOST_TEST(b.size() == 2);
    auto it = b.begin();
    BOOST_TEST(*it == 1);
    ++it;
    BOOST_TEST(*it == 2);
    a.push_back(3);
    BOOST_TEST(a.size() == 3);
    BOOST_TEST(b.size() == 2);
}

BOOST_AUTO_TEST_CASE(MoveAssignment) {
    List<int> a, b;
    a.push_back(1);
    a.push_back(2);
    b = std::move(a);
    BOOST_TEST(b.size() == 2);
    auto it = b.begin();
    BOOST_TEST(*it == 1);
    ++it;
    BOOST_TEST(*it == 2);
    BOOST_TEST(a.empty());
}

BOOST_AUTO_TEST_CASE(Size) {
    List<int> lst;
    BOOST_TEST(lst.size() == 0);
    lst.push_back(1);
    BOOST_TEST(lst.size() == 1);
    lst.push_back(2);
    BOOST_TEST(lst.size() == 2);
    lst.pop_front();
    BOOST_TEST(lst.size() == 1);
    lst.clear();
    BOOST_TEST(lst.size() == 0);
}

BOOST_AUTO_TEST_CASE(Swap) {
    List<int> a, b;
    a.push_back(1);
    a.push_back(2);
    b.push_back(3);
    b.push_back(4);
    swap(a, b);
    BOOST_TEST(a.size() == 2);
    BOOST_TEST(b.size() == 2);
    auto it = a.begin();
    BOOST_TEST(*it == 3); ++it;
    BOOST_TEST(*it == 4);
    it = b.begin();
    BOOST_TEST(*it == 1); ++it;
    BOOST_TEST(*it == 2);
}

BOOST_AUTO_TEST_SUITE_END()
