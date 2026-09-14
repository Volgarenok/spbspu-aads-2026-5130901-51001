#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "matrix.hpp"

namespace hachaturyanov
{
  using MatrixTable = HashTable< std::string, Matrix, std::hash< std::string >, std::equal_to< std::string > >;

  List< std::string > tokenize(const std::string &line);
  size_t parseSizeT(const std::string &token);

  void cmdNew(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdDrop(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdShow(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdSet(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdGet(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdAdd(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdSub(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdMul(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdScale(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdTranspose(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdInsertRow(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdInsertCol(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdDeleteRow(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdDeleteCol(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdAppendRows(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdEmbed(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdJoinRight(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdJoinBottom(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdCrop(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdReplace(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdFlatten(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);
  void cmdRepeat(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out);

  void (*findHandler(const std::string &name))(const List< std::string > &, MatrixTable &, std::ostream &);
}

#endif
