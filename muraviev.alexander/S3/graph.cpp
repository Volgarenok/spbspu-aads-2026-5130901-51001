#include "graph.hpp"

muraviev::Graph::Graph(const std::string& name):
  name_(name)
{}

const std::string& muraviev::Graph::getName() const
{
  return name_;
}
