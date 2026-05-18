#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>

#include "graph.hpp"
#include "hash_table.hpp"
#include "sha1.hpp"
#include "common/list.hpp"

namespace shevchenko
{

using GraphTable = HashTable<std::string, Graph, SHA1Hasher>;

void printInvalid(std::ostream& out);
void graphsCommand(const GraphTable& graphs, std::ostream& out);
void vertexesCommand(const GraphTable& graphs, std::istream& in, std::ostream& out);
void outboundCommand(const GraphTable& graphs, std::istream& in, std::ostream& out);
void inboundCommand(const GraphTable& graphs, std::istream& in, std::ostream& out);
void bindCommand(GraphTable& graphs, std::istream& in, std::ostream& out);
void cutCommand(GraphTable& graphs, std::istream& in, std::ostream& out);
void createCommand(GraphTable& graphs, std::istream& in, std::ostream& out);
void mergeCommand(GraphTable& graphs, std::istream& in, std::ostream& out);
void extractCommand(GraphTable& graphs, std::istream& in, std::ostream& out);

}

#endif
