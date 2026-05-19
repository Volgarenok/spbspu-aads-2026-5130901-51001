#include <boost/test/unit_test.hpp>
#include <cmath>
#include "route.hpp"

namespace vishnyakov
{
  BOOST_AUTO_TEST_SUITE(RouteTests)

  BOOST_AUTO_TEST_CASE(Interpolate)
  {
    auto coords = interpolate(0, 0, 100, 100, 0.5);
    BOOST_TEST(coords.first == 50);
    BOOST_TEST(coords.second == 50);

    coords = interpolate(0, 0, 100, 200, 0.3);
    BOOST_TEST(coords.first == 30);
    BOOST_TEST(coords.second == 60);
  }

  BOOST_AUTO_TEST_CASE(TraverseSegmentShortDay)
  {
    double startTime = 5.0;
    auto result = traverseSegment("A", 0, 0, "B", 100, 0, startTime, true);

    BOOST_TEST(result.endTime > 5.29);
    BOOST_TEST(result.endTime < 5.30);
    BOOST_TEST(result.totalNightTime == 0.0);
    BOOST_TEST(result.stops.size() == 1);
    BOOST_TEST(result.stops[0].name == "B");
    BOOST_TEST(!result.stops[0].isNightStop);
  }

  BOOST_AUTO_TEST_CASE(TraverseSegmentWithNight)
  {
    double startTime = 9.5;
    auto result = traverseSegment("A", 0, 0, "B", 500, 0, startTime, true);

    BOOST_TEST(result.totalNightTime > 9.99);
    BOOST_TEST(result.totalNightTime < 10.01);
    BOOST_TEST(result.endTime > 0.98);
    BOOST_TEST(result.endTime < 1.00);
  }

  BOOST_AUTO_TEST_CASE(TraverseSegmentStartAtNight)
  {
    double startTime = 15.0;
    auto result = traverseSegment("A", 0, 0, "B", 100, 0, startTime, true);

    BOOST_TEST(result.totalNightTime == 5.0);
    BOOST_TEST(result.stops.size() >= 1);
    BOOST_TEST(result.stops[0].isNightStop);
  }

  BOOST_AUTO_TEST_CASE(BuildGreedyRouteSimple)
  {
    std::vector< std::pair< std::string, Waypoint > > points;
    points.emplace_back("home", Waypoint(100, 0, "house"));
    points.emplace_back("mine", Waypoint(200, 0, "cave"));

    auto result = buildGreedyRoute(points, 0, 0, 5.0);

    BOOST_TEST(result.totalDistance == 200.0);
    BOOST_TEST(result.totalTime >= 5.0 + 200.0 / 336.0);
    BOOST_TEST(result.totalTime < 5.0 + 200.0 / 336.0 + 0.01);
    BOOST_TEST(result.allStops.size() == 3);
    BOOST_TEST(result.allStops[0].name == "start");
    BOOST_TEST(result.allStops[1].name == "home");
    BOOST_TEST(result.allStops[2].name == "mine");
  }

  BOOST_AUTO_TEST_CASE(BuildGreedyRouteEmpty)
  {
    std::vector< std::pair< std::string, Waypoint > > points;
    auto result = buildGreedyRoute(points, 0, 0, 5.0);
    BOOST_TEST(result.totalDistance == 0.0);
    BOOST_TEST(result.totalTime == 0.0);
    BOOST_TEST(result.allStops.empty());
  }

  BOOST_AUTO_TEST_CASE(GreedyRouteOrder)
  {
    std::vector< std::pair< std::string, Waypoint > > points;
    points.emplace_back("far", Waypoint(500, 0, "far"));
    points.emplace_back("near", Waypoint(100, 0, "near"));
    points.emplace_back("mid", Waypoint(250, 0, "mid"));

    auto result = buildGreedyRoute(points, 0, 0, 5.0);

    BOOST_TEST(result.allStops[1].name == "near");
    BOOST_TEST(result.allStops[2].name == "mid");
    BOOST_TEST(result.allStops[3].name == "far");
  }

  BOOST_AUTO_TEST_CASE(HungerAndBread)
  {
    std::vector< std::pair< std::string, Waypoint > > points;
    points.emplace_back("home", Waypoint(200, 0, "house"));

    auto result = buildGreedyRoute(points, 0, 0, 5.0);

    BOOST_TEST(result.totalHunger == 5.0);
    BOOST_TEST(result.breadNeeded == 1);
  }

  BOOST_AUTO_TEST_CASE(HungerAndBreadRoundUp)
  {
    std::vector< std::pair< std::string, Waypoint > > points;
    points.emplace_back("home", Waypoint(210, 0, "house"));

    auto result = buildGreedyRoute(points, 0, 0, 5.0);

    BOOST_TEST(result.totalHunger == 5.25);
    BOOST_TEST(result.breadNeeded == 2);
  }

  BOOST_AUTO_TEST_SUITE_END()
}

