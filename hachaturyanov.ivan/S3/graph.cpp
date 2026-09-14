#include "graph.hpp"

namespace hachaturyanov
{
  Graph::Graph():
   links_(),
   vertices_(new List< std::string >()),
   links_count_(0),
   vertices_count_(0)
  {}

  Graph::Graph(const Graph &other):
   links_(link_table(other.links_)),
   vertices_(new List< std::string >(*other.vertices_)),
   links_count_(other.links_count_),
   vertices_count_(other.vertices_count_)
  {}

  Graph::Graph(Graph &&other) noexcept:
   links_(std::move(other.links_)),
   vertices_(other.vertices_),
   links_count_(other.links_count_),
   vertices_count_(other.vertices_count_)
  {
    other.vertices_ = nullptr;
    other.links_count_ = 0;
    other.vertices_count_ = 0;
  }

  void Graph::swap_(Graph &other) noexcept
  {
    std::swap(links_, other.links_);
    std::swap(vertices_, other.vertices_);
    std::swap(links_count_, other.links_count_);
    std::swap(vertices_count_, other.vertices_count_);
  }

  Graph &Graph::operator=(const Graph &other)
  {
    if (this != &other) {
      Graph copy(other);
      swap_(copy);
    }
    return *this;
  }

  Graph &Graph::operator=(Graph &&other) noexcept
  {
    if (this != &other) {
      swap_(other);
    }
    return *this;
  }

  Graph::Graph(size_t links_count, new_links_list* links_list):
   links_(),
   vertices_(nullptr),
   links_count_(links_count),
   vertices_count_(0)
  {
    if (links_list) {
      vertices_ = new List< std::string >();
      auto it = links_list->begin();
      do {
        std::pair< std::pair< std::string, std::string >, size_t > data = *it;
        if (links_.has(data.first)) {
          links_[data.first].insertSorted(data.second);
        } else {
          if (links_.size() * 2 > links_.capacity()) {
            links_.rehash(links_.capacity() * 2);
          }
          links_.add(data.first, List< size_t >{data.second});
        }
        if (!vertices_->has(data.first.first)) {
          vertices_->insertSorted(data.first.first);
          vertices_count_++;
        }
        if (!vertices_->has(data.first.second)) {
          vertices_->insertSorted(data.first.second);
          vertices_count_++;
        }
        ++it;
      } while (it != links_list->begin());
    }
  }

  const List< std::string >* Graph::getVertices() const noexcept
  {
    return vertices_;
  }

  oneway_links_list Graph::getLinks(const std::string& vertex, bool outbound) const
  {
    if (vertices_->has(vertex)) {
      oneway_links_list result;
      auto it = vertices_->begin();
      do {
        std::string otherVertex = *it;
        vertices_pair verticesPair = {otherVertex, vertex};
        if (outbound) {
          verticesPair = {vertex, otherVertex};
        }
        if (links_.has(verticesPair)) {
          result.addEnd({otherVertex, links_.get(verticesPair)});
        }
        ++it;
      } while (it != vertices_->begin());
      return result;
    } else {
      throw std::logic_error("Vertex not found");
    }
  }

  void Graph::bind(const std::string &vertex1, const std::string &vertex2, size_t weight)
  {
    Graph temp(*this);
    vertices_pair verticesPair = {vertex1, vertex2};
    if (temp.links_.has(verticesPair)) {
      temp.links_[verticesPair].insertSorted(weight);
    } else {
      if (temp.links_.size() * 2 > temp.links_.capacity()) {
        temp.links_.rehash(temp.links_.capacity() * 2);
      }
      temp.links_.add(verticesPair, List< size_t >{weight});
    }
    if (!temp.vertices_->has(vertex1)) {
      temp.vertices_->insertSorted(vertex1);
      temp.vertices_count_++;
    }
    if (!temp.vertices_->has(vertex2)) {
      temp.vertices_->insertSorted(vertex2);
      temp.vertices_count_++;
    }
    temp.links_count_++;

    swap_(temp);
  }

  void Graph::cut(const std::string &vertex1, const std::string &vertex2, size_t weight)
  {
    Graph temp(*this);
    vertices_pair verticesPair = {vertex1, vertex2};
    if (!temp.vertices_->has(vertex1) || !temp.vertices_->has(vertex2)) {
      throw std::logic_error("Vertex not found");
    }
    if (!temp.links_.has(verticesPair) || !temp.links_.get(verticesPair).has(weight)) {
      throw std::logic_error("Link not found");
    }
    List< size_t  > &weights = temp.links_.get(verticesPair);
    weights.erase(weights.find(weight));
    if (weights.isEmpty()) {
      temp.links_.drop(verticesPair);
    }
    temp.links_count_--;

    swap_(temp);
  }

  Graph::Graph(size_t vertices_count, const List< std::string >* vertices):
   links_(),
   vertices_(new List< std::string >()),
   links_count_(0),
   vertices_count_(vertices_count)
  {
    if (!vertices->isEmpty()) {
      auto it = vertices->begin();
      do {
        vertices_->insertSorted(*it);
        ++it;
      } while (it != vertices->begin());
    }
  }

  Graph::Graph(const Graph &graph1, const Graph &graph2):
   Graph(graph1)
  {
    if (!graph2.vertices_->isEmpty()) {
      auto start = graph2.vertices_->begin();
      auto it = start;
      do {
        std::string vertex1 = *it;
        if (!vertices_->has(vertex1)) {
          vertices_->insertSorted(vertex1);
          vertices_count_++;
        }
        oneway_links_list links = graph2.getLinks(vertex1, true);
        if (!links.isEmpty()) {
          auto linksIt = links.begin();
          do {
            std::string vertex2 = linksIt->first;
            List< size_t > weights = linksIt->second;
            if (!weights.isEmpty()) {
              auto weightsIt = weights.begin();
              do {
                size_t weight = *weightsIt;
                bind(vertex1, vertex2, weight);
                ++weightsIt;
              } while (weightsIt != weights.begin());
            }
            ++linksIt;
          } while (linksIt != links.begin());
        }
        ++it;
      } while (it != start);
    }
  }

  Graph::Graph(const Graph &other, size_t vertices_count, const List< std::string >* vertices):
   Graph(vertices_count, vertices)
  {
    auto it = vertices->begin();
    do {
      std::string vertex1 = *it;
      if (other.vertices_->has(vertex1)) {
        oneway_links_list links = other.getLinks(vertex1, true);
        if (!links.isEmpty()) {
          auto linksIt = links.begin();
          do {
            std::string vertex2 = linksIt->first;
            if (vertices_->has(vertex2)) {
              List< size_t > weights = linksIt->second;
              if (!weights.isEmpty()) {
                auto weightsIt = weights.begin();
                do {
                  size_t weight = *weightsIt;
                  bind(vertex1, vertex2, weight);
                  ++weightsIt;
                } while (weightsIt != weights.begin());
              }
            }
            ++linksIt;
          } while (linksIt != links.begin());
        }
      } else {
        throw std::logic_error("Vertex not found");
      }
      ++it;
    } while (it != vertices->begin());
  }

  Graph::~Graph()
  {
    delete vertices_;
  }
}
