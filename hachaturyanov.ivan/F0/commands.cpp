#include "commands.hpp"

namespace hachaturyanov
{
  List< std::string > tokenize(const std::string &line)
  {
    List< std::string > tokens;
    size_t pos = 0;

    while (pos < line.size()) {
      while (pos < line.size() && std::isspace(line[pos])) {
        pos++;
      }
      if (pos >= line.size()) {
        break;
      }
      size_t start = pos;
      while (pos < line.size() && !std::isspace(line[pos])) {
        pos++;
      }
      tokens.addEnd(line.substr(start, pos - start));
    }
    return tokens;
  }

  size_t parseSizeT(const std::string &token)
  {
    if (token.empty() || token[0] == '-') {
      throw std::invalid_argument("Invalid size_t value");
    }
    return std::stoul(token);
  }

  void cmdNew(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 5) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string name = *it;
    ++it;
    size_t rows = parseSizeT(*it);
    ++it;
    size_t cols = parseSizeT(*it);
    ++it;
    int fill = std::stoi(*it);

    if (!matrices.has(name) && rows && cols) {
      matrices.add(name, Matrix(rows, cols, fill));
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdDrop(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 2) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string name = *it;

    if (matrices.has(name)) {
      matrices.drop(name);
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdShow(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 2) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string name = *it;

    if (matrices.has(name)) {
      const Matrix &matrix = matrices.get(name);
      out << "<ROWS: " << matrix.rows() << ", COLS: " << matrix.cols() << ", DATA: [";
      for (size_t i = 0; i < matrix.rows(); i++) {
        out << "[";
        for (size_t j = 0; j < matrix.cols(); j++) {
          out << matrix.get(i, j);
          if (j < matrix.cols() - 1) {
            out << ", ";
          }
        }
        out << "]";
        if (i < matrix.rows() - 1) {
          out << ", ";
        }
      }
      out << "]>" << '\n';
    } else {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdSet(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 5) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string name = *it;
    ++it;
    size_t row = parseSizeT(*it);
    ++it;
    size_t col = parseSizeT(*it);
    ++it;
    int value = std::stoi(*it);

    try {
      matrices.get(name).set(row, col, value);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdGet(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 4) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string name = *it;
    ++it;
    size_t row = parseSizeT(*it);
    ++it;
    size_t col = parseSizeT(*it);

    try {
      int result = matrices.get(name).get(row, col);
      out << result << '\n';
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdAdd(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 4) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string res = *it;
    ++it;
    std::string matrix1 = *it;
    ++it;
    std::string matrix2 = *it;

    try {
      if (matrices.has(res)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &lhs = matrices.get(matrix1);
      const Matrix &rhs = matrices.get(matrix2);
      Matrix result = lhs + rhs;

      matrices.add(res, result);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdSub(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 4) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string res = *it;
    ++it;
    std::string matrix1 = *it;
    ++it;
    std::string matrix2 = *it;

    try {
      if (matrices.has(res)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &lhs = matrices.get(matrix1);
      const Matrix &rhs = matrices.get(matrix2);
      Matrix result = lhs - rhs;

      matrices.add(res, result);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdMul(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 4) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string res = *it;
    ++it;
    std::string matrix1 = *it;
    ++it;
    std::string matrix2 = *it;

    try {
      if (matrices.has(res)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &lhs = matrices.get(matrix1);
      const Matrix &rhs = matrices.get(matrix2);
      Matrix result = lhs * rhs;

      matrices.add(res, result);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdScale(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 4) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string res = *it;
    ++it;
    std::string name = *it;
    ++it;
    int scalar = std::stoi(*it);

    try {
      if (matrices.has(res)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &matrix = matrices.get(name);
      Matrix result = matrix * scalar;

      matrices.add(res, result);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdTranspose(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 3) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string res = *it;
    ++it;
    std::string name = *it;

    try {
      if (matrices.has(res)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &matrix = matrices.get(name);
      Matrix result = matrix.transpose();

      matrices.add(res, result);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdInsertRow(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() < 4) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string name = *it;
    ++it;
    size_t rowIndex = parseSizeT(*it);
    ++it;
    List< int > values;
    for (; it != tokens.end(); ++it) {
      values.addEnd(std::stoi(*it));
    }

    try {
      Matrix &matrix = matrices.get(name);
      matrix.insert_row(rowIndex, values);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdInsertCol(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() < 4) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string name = *it;
    ++it;
    size_t colIndex = parseSizeT(*it);
    ++it;
    List< int > values;
    for (; it != tokens.end(); ++it) {
      values.addEnd(std::stoi(*it));
    }

    try {
      Matrix &matrix = matrices.get(name);
      matrix.insert_col(colIndex, values);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdDeleteRow(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 3) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string name = *it;
    ++it;
    size_t rowIndex = parseSizeT(*it);

    try {
      Matrix &matrix = matrices.get(name);
      matrix.delete_row(rowIndex);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdDeleteCol(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 3) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string name = *it;
    ++it;
    size_t colIndex = parseSizeT(*it);

    try {
      Matrix &matrix = matrices.get(name);
      matrix.delete_col(colIndex);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdAppendRows(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 5) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string dest = *it;
    ++it;
    std::string source = *it;
    ++it;
    size_t rowStart = parseSizeT(*it);
    ++it;
    size_t rowEnd = parseSizeT(*it);

    try {
      Matrix &destMatrix = matrices.get(dest);
      const Matrix &sourceMatrix = matrices.get(source);

      destMatrix.append_rows(sourceMatrix, rowStart, rowEnd);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdEmbed(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 8) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string result = *it;
    ++it;
    std::string source = *it;
    ++it;
    size_t rows = parseSizeT(*it);
    ++it;
    size_t cols = parseSizeT(*it);
    ++it;
    size_t rowOffset = parseSizeT(*it);
    ++it;
    size_t colOffset = parseSizeT(*it);
    ++it;
    int fill = std::stoi(*it);

    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = sourceMatrix.embed(rows, cols, rowOffset, colOffset, fill);

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdJoinRight(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 5) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string result = *it;
    ++it;
    std::string dest = *it;
    ++it;
    std::string source = *it;
    ++it;
    int fill = std::stoi(*it);

    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &destMatrix = matrices.get(dest);
      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = destMatrix.join_right(sourceMatrix, fill);

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdJoinBottom(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 5) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string result = *it;
    ++it;
    std::string dest = *it;
    ++it;
    std::string source = *it;
    ++it;
    int fill = std::stoi(*it);

    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &destMatrix = matrices.get(dest);
      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = destMatrix.join_bottom(sourceMatrix, fill);

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdCrop(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 7) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string result = *it;
    ++it;
    std::string source = *it;
    ++it;
    size_t row = parseSizeT(*it);
    ++it;
    size_t col = parseSizeT(*it);
    ++it;
    size_t rows = parseSizeT(*it);
    ++it;
    size_t cols = parseSizeT(*it);

    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = sourceMatrix.crop(row, col, rows, cols);

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdReplace(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 5) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string dest = *it;
    ++it;
    std::string source = *it;
    ++it;
    size_t row = parseSizeT(*it);
    ++it;
    size_t col = parseSizeT(*it);

    try {
      Matrix &destMatrix = matrices.get(dest);
      const Matrix &sourceMatrix = matrices.get(source);

      destMatrix.replace(sourceMatrix, row, col);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdFlatten(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 3) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string result = *it;
    ++it;
    std::string source = *it;

    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = sourceMatrix.flatten();

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }

  void cmdRepeat(const List< std::string > &tokens, MatrixTable &matrices, std::ostream &out)
  {
    if (tokens.size() != 5) {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }
    auto it = tokens.begin();
    ++it;
    std::string result = *it;
    ++it;
    std::string source = *it;
    ++it;
    size_t tileRows = parseSizeT(*it);
    ++it;
    size_t tileCols = parseSizeT(*it);

    try {
      if (matrices.has(result)) {
        throw std::logic_error("Result matrix already exists");
      }

      const Matrix &sourceMatrix = matrices.get(source);
      Matrix resultMatrix = sourceMatrix.repeat(tileRows, tileCols);

      matrices.add(result, resultMatrix);
    } catch (const std::logic_error &) {
      out << "<INVALID COMMAND>" << '\n';
    }
  }


  struct CommandEntry {
    std::string name;
    void (*handler)(const List< std::string > &, MatrixTable &, std::ostream &);
  };

  const CommandEntry COMMAND_TABLE[] = {
    { "new", cmdNew },
    { "drop", cmdDrop },
    { "show", cmdShow },
    { "set", cmdSet },
    { "get", cmdGet },
    { "add", cmdAdd },
    { "sub", cmdSub },
    { "mul", cmdMul },
    { "scale", cmdScale },
    { "transpose", cmdTranspose },
    { "insert-row", cmdInsertRow },
    { "insert-col", cmdInsertCol },
    { "delete-row", cmdDeleteRow },
    { "delete-col", cmdDeleteCol },
    { "append-rows", cmdAppendRows },
    { "embed", cmdEmbed },
    { "join-right", cmdJoinRight },
    { "join-bottom", cmdJoinBottom },
    { "crop", cmdCrop },
    { "replace", cmdReplace },
    { "flatten", cmdFlatten },
    { "repeat", cmdRepeat }
  };

  const size_t COMMAND_TABLE_SIZE = sizeof(COMMAND_TABLE) / sizeof(COMMAND_TABLE[0]);

  void (*findHandler(const std::string &name))(const List< std::string > &, MatrixTable &, std::ostream &)
  {
    for (size_t i = 0; i < COMMAND_TABLE_SIZE; ++i) {
      if (COMMAND_TABLE[i].name == name) {
        return COMMAND_TABLE[i].handler;
      }
    }
    return nullptr;
  }
}
