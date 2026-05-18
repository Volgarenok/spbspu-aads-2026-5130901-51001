#include "commands.hpp"
#include <algorithm>
#include <cstddef>

namespace shevchenko
{

static void sortStrings(std::string* arr, size_t size)
{
  for (size_t i = 0; i + 1 < size; ++i)
  {
    for (size_t j = 0; j + 1 < size - i; ++j)
    {
      if (arr[j] > arr[j + 1])
      {
        std::string tmp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = tmp;
      }
    }
  }
}

static void sortWeights(size_t* arr, size_t size)
{
  for (size_t i = 0; i + 1 < size; ++i)
  {
    for (size_t j = 0; j + 1 < size - i; ++j)
    {
      if (arr[j] > arr[j + 1])
      {
        size_t tmp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = tmp;
      }
    }
  }
}


void printInvalid(std::ostream& out)
{
  out << "<INVALID COMMAND>\n";
}

void graphsCommand(const GraphTable& graphs, std::ostream& out)
{
  std::string* names = new std::string[graphs.size()];
  size_t index = 0;
  
  for (auto it = graphs.cbegin(); it != graphs.cend(); ++it)
  {
    names[index] = (*it).first;
    ++index;
  }
  
  sortStrings(names, graphs.size());
  
  for (size_t i = 0; i < graphs.size(); ++i)
  {
    out << names[i] << '\n';
  }
  
  delete[] names;
}

void vertexesCommand(const GraphTable& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  in >> graph_name;
  
  try
  {
    const Graph& graph = graphs.at(graph_name);
    
    size_t vertex_count = 0;
    for (auto it = graph.data().cbegin(); it != graph.data().cend(); ++it)
    {
      ++vertex_count;
    }
    
    std::string* vertexes = new std::string[vertex_count];
    size_t index = 0;
    
    for (auto it = graph.data().cbegin(); it != graph.data().cend(); ++it)
    {
      vertexes[index] = (*it).first;
      ++index;
    }
    
    sortStrings(vertexes, vertex_count);
    
    for (size_t i = 0; i < vertex_count; ++i)
    {
      out << vertexes[i] << '\n';
    }
    
    delete[] vertexes;
  }
  catch (const std::exception&)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void outboundCommand(const GraphTable& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  std::string vertex;
  in >> graph_name >> vertex;
  
  try
  {
    const Graph& graph = graphs.at(graph_name);
    
    if (!graph.hasVertex(vertex))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    
    const Graph::EdgeList& edges = graph.outbound(vertex);
    
    HashTable<std::string, List<size_t>, SHA1Hasher> targets;
    
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
      if (!targets.contains((*it).to))
      {
        targets.insert((*it).to, List<size_t>());
      }
      targets.at((*it).to).pushBack((*it).weight);
    }
    
    size_t target_count = 0;
    for (auto it = targets.cbegin(); it != targets.cend(); ++it)
    {
      ++target_count;
    }
    
    std::string* target_names = new std::string[target_count];
    size_t index = 0;
    for (auto it = targets.cbegin(); it != targets.cend(); ++it)
    {
      target_names[index] = (*it).first;
      ++index;
    }
    
    sortStrings(target_names, target_count);
    
    for (size_t i = 0; i < target_count; ++i)
    {
      const std::string& target = target_names[i];
      List<size_t> weights = targets.at(target);
      
      size_t weight_count = 0;
      for (auto w_it = weights.begin(); w_it != weights.end(); ++w_it)
      {
        ++weight_count;
      }
      
      size_t* weight_arr = new size_t[weight_count];
      size_t w_index = 0;
      for (auto w_it = weights.begin(); w_it != weights.end(); ++w_it)
      {
        weight_arr[w_index] = *w_it;
        ++w_index;
      }
      
      sortWeights(weight_arr, weight_count);
      
      out << target;
      for (size_t j = 0; j < weight_count; ++j)
      {
        out << ' ' << weight_arr[j];
      }
      out << '\n';
      
      delete[] weight_arr;
    }
    
    delete[] target_names;
  }
  catch (const std::exception&)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void inboundCommand(const GraphTable& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  std::string vertex;
  in >> graph_name >> vertex;
  
  try
  {
    const Graph& graph = graphs.at(graph_name);
    
    if (!graph.hasVertex(vertex))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    
    Graph::EdgeList edges = graph.inbound(vertex);
    
    HashTable<std::string, List<size_t>, SHA1Hasher> sources;
    
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
      if (!sources.contains((*it).to))
      {
        sources.insert((*it).to, List<size_t>());
      }
      sources.at((*it).to).pushBack((*it).weight);
    }
    
    size_t source_count = 0;
    for (auto it = sources.cbegin(); it != sources.cend(); ++it)
    {
      ++source_count;
    }
    
    std::string* source_names = new std::string[source_count];
    size_t index = 0;
    for (auto it = sources.cbegin(); it != sources.cend(); ++it)
    {
      source_names[index] = (*it).first;
      ++index;
    }
    
    sortStrings(source_names, source_count);
    
    for (size_t i = 0; i < source_count; ++i)
    {
      const std::string& source = source_names[i];
      List<size_t> weights = sources.at(source);
      
      size_t weight_count = 0;
      for (auto w_it = weights.begin(); w_it != weights.end(); ++w_it)
      {
        ++weight_count;
      }
      
      size_t* weight_arr = new size_t[weight_count];
      size_t w_index = 0;
      for (auto w_it = weights.begin(); w_it != weights.end(); ++w_it)
      {
        weight_arr[w_index] = *w_it;
        ++w_index;
      }
      
      sortWeights(weight_arr, weight_count);
      
      out << source;
      for (size_t j = 0; j < weight_count; ++j)
      {
        out << ' ' << weight_arr[j];
      }
      out << '\n';
      
      delete[] weight_arr;
    }
    
    delete[] source_names;
  }
  catch (const std::exception&)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void bindCommand(GraphTable& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  std::string from;
  std::string to;
  size_t weight;
  
  in >> graph_name >> from >> to >> weight;
  
  try
  {
    graphs.at(graph_name).bind(from, to, weight);
  }
  catch (const std::exception&)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void cutCommand(GraphTable& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  std::string from;
  std::string to;
  size_t weight;
  
  in >> graph_name >> from >> to >> weight;
  
  try
  {
    graphs.at(graph_name).cut(from, to, weight);
  }
  catch (const std::exception&)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void createCommand(GraphTable& graphs, std::istream& in, std::ostream& out)
{
  std::string graph_name;
  size_t vertex_count;
  
  in >> graph_name >> vertex_count;
  
  try
  {
    if (graphs.contains(graph_name))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    
    Graph new_graph;
    
    for (size_t i = 0; i < vertex_count; ++i)
    {
      std::string vertex;
      in >> vertex;
      new_graph.addVertex(vertex);
    }
    
    graphs.insert(graph_name, new_graph);
  }
  catch (const std::exception&)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void mergeCommand(GraphTable& graphs, std::istream& in, std::ostream& out)
{
  std::string result_name;
  std::string lhs_name;
  std::string rhs_name;
  
  in >> result_name >> lhs_name >> rhs_name;
  
  try
  {
    if (graphs.contains(result_name))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    
    if (!graphs.contains(lhs_name) || !graphs.contains(rhs_name))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    
    Graph result = graphs.at(lhs_name);
    result.merge(graphs.at(rhs_name));
    graphs.insert(result_name, result);
  }
  catch (const std::exception&)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void extractCommand(GraphTable& graphs, std::istream& in, std::ostream& out)
{
  std::string result_name;
  std::string source_name;
  size_t vertex_count;
  
  in >> result_name >> source_name >> vertex_count;
  
  try
  {
    if (graphs.contains(result_name))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    
    if (!graphs.contains(source_name))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    
    const Graph& source = graphs.at(source_name);
    Graph result;
    
    for (size_t i = 0; i < vertex_count; ++i)
    {
      std::string vertex;
      in >> vertex;
      
      if (!source.hasVertex(vertex))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }
      
      Graph subgraph = source.extract(vertex);
      result.merge(subgraph);
    }
    
    graphs.insert(result_name, result);
  }
  catch (const std::exception&)
  {
    out << "<INVALID COMMAND>\n";
  }
}

}
