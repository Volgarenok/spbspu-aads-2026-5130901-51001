#include "route.hpp"
#include <cmath>
#include <algorithm>
#include <limits>

namespace vishnyakov
{
  SegmentResult traverseSegment(
    const std::string& nameA,
    int xA, int zA,
    const std::string& nameB,
    int xB, int zB,
    double startTime,
    bool isStartPoint)
  {
    SegmentResult result;
    result.totalNightTime = 0.0;
    double currentTime = startTime;
    double dx = static_cast< double >(xB - xA);
    double dz = static_cast< double >(zB - zA);
    double totalDistance = std::sqrt(dx * dx + dz * dz);
    double remainingDistance = totalDistance;
    double currentX = static_cast< double >(xA);
    double currentZ = static_cast< double >(zA);

    if (currentTime >= NIGHT_START)
    {
      RouteStop nightStop;
      nightStop.isPoint = isStartPoint;
      nightStop.isNightStop = true;
      if (isStartPoint)
      {
        nightStop.name = nameA;
      }
      else
      {
        nightStop.name = "field";
      }
      nightStop.x = static_cast< int >(currentX);
      nightStop.z = static_cast< int >(currentZ);
      nightStop.time = currentTime;
      nightStop.travelTime = 0.0;
      nightStop.distanceFromPrev = 0.0;
      result.stops.push_back(nightStop);
      result.totalNightTime += CYCLE_LENGTH - currentTime;
      currentTime = 0.0;
    }

    while (remainingDistance > 1e-6)
    {
      if (currentTime >= NIGHT_START)
      {
        RouteStop nightStop;
        nightStop.isPoint = false;
        nightStop.isNightStop = true;
        nightStop.name = "field";
        nightStop.x = static_cast< int >(currentX);
        nightStop.z = static_cast< int >(currentZ);
        nightStop.time = currentTime;
        nightStop.travelTime = 0.0;
        nightStop.distanceFromPrev = 0.0;
        result.stops.push_back(nightStop);
        result.totalNightTime += CYCLE_LENGTH - currentTime;
        currentTime = 0.0;
        continue;
      }

      double timeToNight = NIGHT_START - currentTime;
      double distanceCanGo = timeToNight * SPEED_BLOCKS_PER_MIN;

      if (distanceCanGo >= remainingDistance)
      {
        double travelTime = remainingDistance / SPEED_BLOCKS_PER_MIN;
        currentTime += travelTime;
        remainingDistance = 0.0;

        RouteStop arrival;
        arrival.isPoint = true;
        arrival.isNightStop = false;
        arrival.name = nameB;
        arrival.x = xB;
        arrival.z = zB;
        arrival.time = currentTime;
        arrival.travelTime = travelTime;
        arrival.distanceFromPrev = totalDistance;
        result.stops.push_back(arrival);
      }
      else
      {
        remainingDistance -= distanceCanGo;
        double fraction = (totalDistance - remainingDistance) / totalDistance;
        auto coords = interpolate(xA, zA, xB, zB, fraction);
        currentX = coords.first;
        currentZ = coords.second;
        double travelTime = distanceCanGo / SPEED_BLOCKS_PER_MIN;
        currentTime = NIGHT_START;

        RouteStop fieldStop;
        fieldStop.isPoint = false;
        fieldStop.isNightStop = false;
        fieldStop.name = "field";
        fieldStop.x = coords.first;
        fieldStop.z = coords.second;
        fieldStop.time = currentTime;
        fieldStop.travelTime = travelTime;
        fieldStop.distanceFromPrev = distanceCanGo;
        result.stops.push_back(fieldStop);

        RouteStop nightStop;
        nightStop.isPoint = false;
        nightStop.isNightStop = true;
        nightStop.name = "field";
        nightStop.x = coords.first;
        nightStop.z = coords.second;
        nightStop.time = currentTime;
        nightStop.travelTime = 0.0;
        nightStop.distanceFromPrev = 0.0;
        result.stops.push_back(nightStop);

        result.totalNightTime += CYCLE_LENGTH - currentTime;
        currentTime = 0.0;
      }
    }

    result.endTime = currentTime;
    return result;
  }

  RouteResult buildGreedyRoute(
    const std::vector< std::pair< std::string, Waypoint > >& points,
    int startX, int startZ,
    double startTime)
  {
    RouteResult result;
    result.totalDistance = 0.0;
    result.totalTime = 0.0;
    result.totalNightTime = 0.0;

    if (points.empty())
    {
      return result;
    }

    std::vector< bool > visited(points.size(), false);
    int currentX = startX;
    int currentZ = startZ;
    double currentTime = startTime;
    bool isAtPoint = false;
    std::string currentPointName = "start";
    std::vector< RouteStop > allStops;

    RouteStop startStop;
    startStop.isPoint = false;
    startStop.isNightStop = false;
    startStop.name = "start";
    startStop.x = startX;
    startStop.z = startZ;
    startStop.time = currentTime;
    startStop.travelTime = 0.0;
    startStop.distanceFromPrev = 0.0;
    allStops.push_back(startStop);

    int visitedCount = 0;
    while (visitedCount < static_cast< int >(points.size()))
    {
      int bestIndex = -1;
      double bestDist = std::numeric_limits< double >::max();

      for (size_t i = 0; i < points.size(); ++i)
      {
        if (!visited[i])
        {
          const auto& wp = points[i].second;
          double dx = static_cast< double >(wp.x - currentX);
          double dz = static_cast< double >(wp.z - currentZ);
          double dist = std::sqrt(dx * dx + dz * dz);

          if (dist < bestDist)
          {
            bestDist = dist;
            bestIndex = static_cast< int >(i);
          }
        }
      }

      if (bestIndex == -1)
      {
        break;
      }

      const auto& point = points[bestIndex];
      const Waypoint& wp = point.second;

      SegmentResult seg = traverseSegment(
        currentPointName, currentX, currentZ,
        point.first, wp.x, wp.z,
        currentTime,
        isAtPoint
      );

      for (const auto& stop : seg.stops)
      {
        allStops.push_back(stop);
      }

      currentTime = seg.endTime;
      currentX = wp.x;
      currentZ = wp.z;
      currentPointName = point.first;
      isAtPoint = true;
      result.totalDistance += bestDist;
      result.totalNightTime += seg.totalNightTime;
      visited[bestIndex] = true;
      ++visitedCount;
    }

    result.allStops = allStops;
    result.totalTime = currentTime + result.totalNightTime;
    result.totalHunger = result.totalDistance / 40.0;
    result.breadNeeded = static_cast< int >(std::ceil(result.totalHunger / 5.0));

    return result;
  }
}

