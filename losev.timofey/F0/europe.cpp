#include "europe.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <climits>

namespace losev {

Graph europeGraph("Europe");
bool graphLoaded = false;

struct Route
{
  std::vector<std::string> cities;
  std::vector<int> distances;
  int totalDistance;

  int difference(int target) const
  {
    return std::abs(totalDistance - target);
  }

  void print(std::ostream& out, int target) const
  {
    out << "Ближайший маршрут к " << target << " км";
    if (cities.size() > 2)
    {
      out << " из " << cities.size() << " городов";
    }
    out << ":\n";

    for (size_t i = 0; i + 1 < cities.size(); ++i)
    {
      out << cities[i] << " -> " << cities[i + 1];
      out << " (" << distances[i] << " км)\n";
    }

    out << "Итого: " << totalDistance << " км";
    out << " (отличие " << difference(target) << " км)\n";
  }
};

void loadEuropeGraph(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    graphLoaded = false;
    return;
  }

  europeGraph = Graph("Europe");
  std::string line;

  while (std::getline(file, line))
  {
    if (line.empty() || line[0] == '#')
    {
      continue;
    }

    std::stringstream ss(line);
    std::string from, to;
    int distance;

    ss >> from >> to >> distance;

    if (ss.fail())
    {
      continue;
    }

    europeGraph.addEdge(from, to, distance);
    europeGraph.addEdge(to, from, distance);
  }

  graphLoaded = true;
  file.close();
}

static void dfsFindRoute(
    const Graph& graph,
    const std::string& current,
    int targetKm,
    int targetCityCount,
    Route& currentRoute,
    Route& bestRoute,
    int& bestDifference,
    std::vector<std::string>& visited)
{
  // Проверяем текущий маршрут (минимум 2 города)
  if (currentRoute.cities.size() >= 2)
  {
    int diff = currentRoute.difference(targetKm);
    if (diff < bestDifference)
    {
      bestDifference = diff;
      bestRoute = currentRoute;
    }
  }

  // Если достигли нужного количества городов
  if (targetCityCount != -1 &&
      static_cast<int>(currentRoute.cities.size()) == targetCityCount)
  {
    return;
  }

  // Ограничиваем максимальное количество городов (5)
  if (currentRoute.cities.size() >= 5)
  {
    return;
  }

  // Получаем всех соседей
  auto outbound = graph.getOutbound(current);

  for (const auto& neighborPair : outbound)
  {
    const std::string& nextCity = neighborPair.first;
    const std::vector<int>& weights = neighborPair.second;

    // Пропускаем уже посещённые
    if (std::find(visited.begin(), visited.end(), nextCity) != visited.end())
    {
      continue;
    }

    if (weights.empty())
    {
      continue;
    }

    int weight = weights[0];

    // Отсечение по оптимистичной оценке
    int optimisticDiff = std::abs(currentRoute.totalDistance + weight - targetKm);
    if (optimisticDiff >= bestDifference)
    {
      continue;
    }

    visited.push_back(nextCity);
    currentRoute.cities.push_back(nextCity);
    currentRoute.distances.push_back(weight);
    currentRoute.totalDistance += weight;

    dfsFindRoute(graph, nextCity, targetKm, targetCityCount,
                 currentRoute, bestRoute, bestDifference, visited);

    currentRoute.totalDistance -= weight;
    currentRoute.distances.pop_back();
    currentRoute.cities.pop_back();
    visited.pop_back();
  }
}

void findRoute(std::ostream& out, int targetKm, int cityCount)
{
  if (!graphLoaded)
  {
    out << "Граф городов не загружен. Проверьте файл europe_cities.txt\n";
    return;
  }

  if (targetKm <= 0)
  {
    out << "Дистанция должна быть положительной\n";
    return;
  }

  if (cityCount < 2 && cityCount != -1)
  {
    out << "Количество городов должно быть не менее 2\n";
    return;
  }

  Route bestRoute;
  int bestDifference = INT_MAX;

  std::vector<std::string> vertices = europeGraph.getVerticesSorted();

  for (const std::string& start : vertices)
  {
    Route currentRoute;
    std::vector<std::string> visited;

    currentRoute.cities.push_back(start);
    visited.push_back(start);
    currentRoute.totalDistance = 0;

    dfsFindRoute(europeGraph, start, targetKm, cityCount,
                 currentRoute, bestRoute, bestDifference, visited);
  }

  if (bestRoute.cities.empty())
  {
    out << "Маршрут не найден\n";
  }
  else
  {
    bestRoute.print(out, targetKm);
  }
}

}
