#define BOOST_TEST_MODULE alekseev_s2_queue
#include <boost/test/included/unit_test.hpp>

#include "queue.hpp"

BOOST_AUTO_TEST_CASE(queue_skeleton_compiles)
{
  alekseev::Queue< int > queue;
  BOOST_TEST(queue.empty());
  BOOST_TEST(queue.size() == 0);
}
