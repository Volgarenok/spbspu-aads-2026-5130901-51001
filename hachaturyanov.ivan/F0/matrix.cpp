#include <limits>
#include "matrix.hpp"

namespace hachaturyanov
{
  size_t Matrix::safeCellCount_(size_t rows, size_t cols) const
  {
    if (rows != 0 && cols > std::numeric_limits< size_t >::max() / rows) {
      throw std::invalid_argument("Matrix dimensions too large");
    }
    size_t total = rows * cols;
    return total > 0 ? total : 1;
  }

  size_t Matrix::safeAdd_(size_t a, size_t b) const
  {
    if (a > std::numeric_limits< size_t >::max() - b) {
      throw std::invalid_argument("Sum overflow");
    }
    return a + b;
  }

  size_t Matrix::safeMul_(size_t a, size_t b) const
  {
    if (a != 0 && b > std::numeric_limits< size_t >::max() / a) {
      throw std::invalid_argument("Multiplication overflow");
    }
    return a * b;
  }

  Matrix::Matrix(size_t rows, size_t cols, int fill):
   rows_(rows),
   cols_(cols),
   data_(safeCellCount_(rows, cols))
  {
    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        data_.add({ i, j }, fill);
      }
    }
  }

  Matrix::Matrix():
   Matrix(1, 1, 0)
  {}

  size_t Matrix::rows() const noexcept
  {
    return rows_;
  }

  size_t Matrix::cols() const noexcept
  {
    return cols_;
  }

  void Matrix::checkBounds_(size_t i, size_t j) const
  {
    if (i >= rows_ || j >= cols_) {
      throw std::out_of_range("Matrix index out of range");
    }
  }

  int Matrix::get(size_t i, size_t j) const
  {
    checkBounds_(i, j);
    return data_.get({ i, j });
  }

  void Matrix::set(size_t i, size_t j, int value)
  {
    checkBounds_(i, j);
    data_.get({ i, j }) = value;
  }

  void Matrix::checkSameDims_(const Matrix &other) const
  {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
      throw std::logic_error("Matrix dimensions don't match");
    }
  }

  Matrix Matrix::operator+(const Matrix &other) const
  {
    checkSameDims_(other);
    Matrix result(rows_, cols_, 0);
    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
      result.set(i, j, get(i, j) + other.get(i, j));
      }
    }
    return result;
  }

  Matrix Matrix::operator-(const Matrix &other) const
  {
    checkSameDims_(other);
    Matrix result(rows_, cols_, 0);
    for(size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        result.set(i, j, get(i, j) - other.get(i, j));
      }
    }
    return result;
  }

  Matrix Matrix::operator*(const Matrix &other) const
  {
    if (cols_ != other.rows_) {
      throw std::logic_error("Matrix dimensions don't match");
    }
    Matrix result(rows_, other.cols_, 0);
    for (size_t i = 0; i < rows_; i++) {
      for (size_t k = 0; k < cols_; k++) {
        int a = get(i, k);
        if (a == 0) {
          continue;
        }
        for (size_t j = 0; j < other.cols_; j++) {
          result.set(i, j, result.get(i, j) + a * other.get(k, j));
        }
      }
    }
    return result;
  }

  Matrix Matrix::operator*(int factor) const
  {
    Matrix result(rows_, cols_, 0);
    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        result.set(i, j, factor * get(i, j));
      }
    }
    return result;
  }

  Matrix Matrix::transpose() const
  {
    Matrix result(cols_, rows_, 0);
    for(size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        result.set(j, i, get(i, j));
      }
    }
    return result;
  }

  bool Matrix::operator==(const Matrix &other) const
  {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
      return false;
    }
    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        if (get(i, j) != other.get(i, j)) {
          return false;
        }
      }
    }
    return true;
  }

  bool Matrix::operator!=(const Matrix &other) const
  {
    return !(*this == other);
  }

  void Matrix::insert_row(size_t index, const List< int > &values)
  {
    if (cols_ != values.size()) {
      throw std::logic_error("Numbers of values and cols don't match");
    }
    if (index > rows_) {
      throw std::out_of_range("Index out of range");
    }

    auto it = values.begin();
    Store temp(safeMul_(safeAdd_(rows_, 1), cols_));

    for (size_t i = 0; i < rows_ + 1; i++) {
      for (size_t j = 0; j < cols_; j++) {
        if (i < index) {
          temp.add({ i, j }, get(i, j));
        } else if (i == index) {
          temp.add({ i, j }, *it);
          ++it;
        } else {
          temp.add({ i, j }, get(i - 1, j));
        }
      }
    }
    data_ = std::move(temp);
    rows_++;
  }

  void Matrix::insert_col(size_t index, const List< int > &values)
  {
    if (rows_ != values.size()) {
      throw std::logic_error("Numbers of values and rows don't match");
    }
    if (index > cols_) {
      throw std::out_of_range("Index out of range");
    }

    auto it = values.begin();
    Store temp(safeMul_(rows_, safeAdd_(cols_, 1)));

    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_ + 1; j++) {
        if (j < index) {
          temp.add({ i, j }, get(i, j));
        } else if (j == index) {
          temp.add({ i, j }, *it);
          ++it;
        } else {
          temp.add({ i, j }, get(i, j - 1));
        }
      }
    }
    data_ = std::move(temp);
    cols_++;
  }

  void Matrix::delete_row(size_t index)
  {
    if (index >= rows_) {
      throw std::out_of_range("Index out of range");
    }
    if (rows_ == 1) {
      throw std::logic_error("Matrix contains only one row");
    }

    Store temp(safeMul_((rows_ - 1), cols_));

    for(size_t i = 0; i < rows_ - 1; i++) {
      for (size_t j = 0; j < cols_; j++) {
        if (i < index) {
          temp.add({ i, j }, get(i, j));
        } else {
          temp.add({ i, j }, get(i + 1, j));
        }
      }
    }
    data_ = std::move(temp);
    rows_--;
  }

  void Matrix::delete_col(size_t index)
  {
    if (index >= cols_) {
      throw std::out_of_range("Index out of range");
    }
    if (cols_ == 1) {
      throw std::logic_error("Matrix contains only one col");
    }

    Store temp(safeMul_(rows_, (cols_ - 1)));

    for(size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_ - 1; j++) {
        if (j < index) {
          temp.add({ i, j }, get(i, j));
        } else {
          temp.add({ i, j }, get(i, j + 1));
        }
      }
    }
    data_ = std::move(temp);
    cols_--;
  }

  void Matrix::append_rows(const Matrix &source, size_t start, size_t end)
  {
    if (source.cols_ != cols_) {
      throw std::logic_error("Numbers of cols don't match");
    }
    if (start >= source.rows_ || end >= source.rows_ || start > end) {
      throw std::out_of_range("Index out of range");
    }

    size_t n = safeAdd_(end - start, 1);
    Store temp(safeMul_(safeAdd_(rows_, n), cols_));

    for (size_t i = 0; i < rows_ + n; i++) {
      for (size_t j = 0; j < cols_; j++) {
        if (i < rows_) {
          temp.add({ i, j }, get(i, j));
        } else {
          temp.add({ i, j }, source.get(start, j));
          if (j == cols_ - 1) {
            start++;
          }
        }
      }
    }
    data_ = std::move(temp);
    rows_ += n;
  }

  Matrix Matrix::embed(size_t newRows, size_t newCols, size_t rowOffset, size_t colOffset, int fill) const
  {
    if (rowOffset + rows_ > newRows || colOffset + cols_ > newCols) {
      throw std::out_of_range("Index out of range");
    }

    Matrix result(newRows, newCols, fill);
    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < cols_; j++) {
        result.set(i + rowOffset, j + colOffset, get(i, j));
      }
    }
    return result;
  }

  Matrix Matrix::join_right(const Matrix &source, int fill) const
  {
    size_t newRows = std::max(rows_, source.rows_);
    size_t newCols = safeAdd_(cols_, source.cols_);
    Matrix result(newRows, newCols, fill);

    for (size_t i = 0; i < newRows; i++) {
      for (size_t j = 0; j < newCols; j++) {
        int value = fill;
        if (j < cols_) {
          if (i < rows_) {
            value = get(i, j);
          }
        } else {
          if (i < source.rows_) {
            value = source.get(i, j - cols_);
          }
        }
        if (value != fill) {
          result.set(i, j, value);
        }
      }
    }
    return result;
  }

   Matrix Matrix::join_bottom(const Matrix &source, int fill) const
  {
    size_t newRows = safeAdd_(rows_, source.rows_);
    size_t newCols = std::max(cols_, source.cols_);
    Matrix result(newRows, newCols, fill);

    for (size_t i = 0; i < newRows; i++) {
      for (size_t j = 0; j < newCols; j++) {
        int value = fill;
        if (i < rows_) {
          if (j < cols_) {
            value = get(i, j);
          }
        } else {
          if (j < source.cols_) {
            value = source.get(i - rows_, j);
          }
        }
        if (value != fill) {
          result.set(i, j, value);
        }
      }
    }
    return result;
  }

  Matrix Matrix::crop(size_t row, size_t col, size_t newRows, size_t newCols) const
  {
    if (newRows > rows_ || row > rows_ - newRows) {
      throw std::logic_error("Crop area not in bounds");
    }
    if (newCols > cols_ || col > cols_ - newCols) {
      throw std::logic_error("Crop area not in bounds");
    }

    Matrix result(newRows, newCols, 0);
    for (size_t i = 0; i < newRows; i++) {
      for (size_t j = 0; j < newCols; j++) {
        result.set(i, j, get(i + row, j + col));
      }
    }
    return result;
  }

  void Matrix::replace(const Matrix &source, size_t row, size_t col)
  {
    if (row > rows_ || source.rows_ > rows_ - row) {
      throw std::logic_error("Out of destination matrix's bounds");
    }
    if (col > cols_ || source.cols_ > cols_ - col) {
      throw std::logic_error("Out of destination matrix's bounds");
    }

    Store temp(data_);
    for (size_t i = 0; i < source.rows_; i++) {
      for (size_t j = 0; j < source.cols_; j++) {
        temp.get({ i + row, j + col }) = source.get(i, j);
      }
    }
    data_ = std::move(temp);
  }

  Matrix Matrix::flatten() const
  {
    Matrix result(1, safeMul_(rows_, cols_), 0);

    for (size_t i = 0; i < rows_; i++) {
      for(size_t j = 0; j < cols_; j++) {
        result.set(0, i * cols_ + j, get(i, j));
      }
    }
    return result;
  }

  Matrix Matrix::repeat(size_t tileRows, size_t tileCols) const
  {
    if (tileRows == 0 || tileCols == 0) {
      throw std::logic_error("Repeat times should be positive");
    }

    Matrix result(safeMul_(rows_, tileRows), safeMul_(cols_, tileCols), 0);

    for (size_t ti = 0; ti < tileRows; ti++) {
      for (size_t tj = 0; tj < tileCols; tj++) {
        for (size_t i = 0; i < rows_; i++) {
          for (size_t j = 0; j < cols_; j++) {
            result.set(ti * rows_ + i, tj * cols_ + j, get(i, j));
          }
        }
      }
    }
    return result;
  }
}
