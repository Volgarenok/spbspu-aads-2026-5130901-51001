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
    double currentX = static_cast< double >(xA);
    double currentZ = static_cast< double >(zA);
    double targetX = static_cast< double >(xB);
    double targetZ = static_cast< double >(zB);

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

    while (true)
    {
      double dx = targetX - currentX;
      double dz = targetZ - currentZ;
      double remainingDistance = std::sqrt(dx * dx + dz * dz);

      if (remainingDistance < 1e-6)
      {
        break;
      }

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

        RouteStop arrival;
        arrival.isPoint = true;
        arrival.isNightStop = false;
        arrival.name = nameB;
        arrival.x = xB;
        arrival.z = zB;
        arrival.time = currentTime;
        arrival.travelTime = travelTime;
        arrival.distanceFromPrev = remainingDistance;
        result.stops.push_back(arrival);

        currentX = targetX;
        currentZ = targetZ;
        break;
      }
      else
      {
        double fraction = distanceCanGo / remainingDistance;
        currentX = currentX + fraction * dx;
        currentZ = currentZ + fraction * dz;
        double travelTime = distanceCanGo / SPEED_BLOCKS_PER_MIN;
        currentTime = NIGHT_START;

        RouteStop nightStop;
        nightStop.isPoint = false;
        nightStop.isNightStop = true;
        nightStop.name = "field";
        nightStop.x = static_cast< int >(currentX);
        nightStop.z = static_cast< int >(currentZ);
        nightStop.time = currentTime;
        nightStop.travelTime = travelTime;
        nightStop.distanceFromPrev = distanceCanGo;
        result.stops.push_back(nightStop);

        result.totalNightTime += CYCLE_LENGTH - currentTime;
        currentTime = 0.0;
      }
    }

    result.endTime = currentTime;
    return result;
  }

  RouteResult buildGreedyRoute(
    const List< std::pair< std::string, Waypoint > >& points,
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

    List< bool > visited;
    for (size_t i = 0; i < points.size(); ++i)
    {
      visited.push_back(false);
    }

    int currentX = startX;
    int currentZ = startZ;
    double currentTime = startTime;
    bool isAtPoint = false;
    std::string currentPointName = "start";
    List< RouteStop > allStops;

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

      int idx = 0;
      for (auto it = points.cbegin(); it != points.cend(); ++it, ++idx)
      {
        int vIdx = 0;
        for (auto vIt = visited.cbegin(); vIt != visited.cend(); ++vIt, ++vIdx)
        {
          if (vIdx == idx)
          {
            if (*vIt)
            {
              break;
            }
            const auto& wp = it->second;
            double dx = static_cast< double >(wp.x - currentX);
            double dz = static_cast< double >(wp.z - currentZ);
            double dist = std::sqrt(dx * dx + dz * dz);

            if (dist < bestDist)
            {
              bestDist = dist;
              bestIndex = idx;
            }
            break;
          }
        }
      }

      if (bestIndex == -1)
      {
        break;
      }

      int pointIdx = 0;
      const std::pair< std::string, Waypoint >* selectedPoint = nullptr;
      for (auto it = points.cbegin(); it != points.cend(); ++it, ++pointIdx)
      {
        if (pointIdx == bestIndex)
        {
          selectedPoint = &(*it);
          break;
        }
      }

      if (!selectedPoint)
      {
        break;
      }

      const auto& point = *selectedPoint;
      const Waypoint& wp = point.second;

      SegmentResult seg = traverseSegment(
        currentPointName, currentX, currentZ,
        point.first, wp.x, wp.z,
        currentTime,
        isAtPoint
      );

      double segmentDistance = 0.0;
      for (const auto& stop : seg.stops)
      {
        allStops.push_back(stop);
        segmentDistance += stop.distanceFromPrev;
      }

      result.totalDistance += segmentDistance;
      currentTime = seg.endTime;
      currentX = wp.x;
      currentZ = wp.z;
      currentPointName = point.first;
      isAtPoint = true;
      result.totalNightTime += seg.totalNightTime;

      int vIdx = 0;
      for (auto vIt = visited.begin(); vIt != visited.end(); ++vIt, ++vIdx)
      {
        if (vIdx == bestIndex)
        {
          *vIt = true;
          break;
        }
      }
      ++visitedCount;
    }

    result.allStops = allStops;
    result.totalTime = currentTime + result.totalNightTime;
    result.totalHunger = result.totalDistance / 40.0;
    result.breadNeeded = static_cast< int >(std::ceil(result.totalHunger / 5.0));

    return result;
  }

  double distanceBetween(const Waypoint& a, const Waypoint& b)
  {
    double dx = static_cast< double >(a.x - b.x);
    double dz = static_cast< double >(a.z - b.z);
    return std::sqrt(dx * dx + dz * dz);
  }

  RouteResult improve2Opt(
    const List< std::pair< std::string, Waypoint > >& points,
    int startX, int startZ,
    double startTime)
  {
    if (points.size() < 2)
    {
      return buildGreedyRoute(points, startX, startZ, startTime);
    }

    List< std::pair< std::string, Waypoint > > pointList = points;
    bool improved = true;

    while (improved)
    {
      improved = false;

      int idx1 = 0;
      for (auto it1 = pointList.cbegin(); it1 != pointList.cend(); ++it1, ++idx1)
      {
        int idx2 = 0;
        for (auto it2 = pointList.cbegin(); it2 != pointList.cend(); ++it2, ++idx2)
        {
          if (idx2 <= idx1 + 1)
          {
            continue;
          }

          auto it1Next = it1;
          ++it1Next;

          auto it2Next = it2;
          ++it2Next;

          if (it1Next == pointList.cend() || it2Next == pointList.cend())
          {
            continue;
          }

          const Waypoint& a = it1->second;
          const Waypoint& b = it1Next->second;
          const Waypoint& c = it2->second;
          const Waypoint& d = it2Next->second;

          double currentDist = distanceBetween(a, b) + distanceBetween(c, d);
          double newDist = distanceBetween(a, c) + distanceBetween(b, d);

          if (newDist < currentDist - 1e-6)
          {
            List< std::pair< std::string, Waypoint > > newList;
            int pos = 0;
            for (auto it = pointList.cbegin(); it != pointList.cend(); ++it, ++pos)
            {
              if (pos <= idx1 || pos > idx2)
              {
                newList.push_back(*it);
              }
              else
              {
                List< std::pair< std::string, Waypoint > > reversed;
                for (int r = idx2; r > idx1; --r)
                {
                  int rpos = 0;
                  for (auto rit = pointList.cbegin(); rit != pointList.cend(); ++rit, ++rpos)
                  {
                    if (rpos == r)
                    {
                      reversed.push_back(*rit);
                      break;
                    }
                  }
                }
                for (const auto& rev : reversed)
                {
                  newList.push_back(rev);
                }
                pos = idx2;
              }
            }
            pointList = newList;
            improved = true;
            break;
          }
        }
        if (improved)
        {
          break;
        }
      }
    }

    return buildGreedyRoute(pointList, startX, startZ, startTime);
  }
}

