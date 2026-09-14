#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include "../common/hash_table_robin.hpp"

namespace hachaturyanov
{
  struct PairHash {
    size_t operator()(const std::pair< size_t, size_t > &p) const noexcept
    {
      size_t h1 = std::hash< size_t >{}(p.first);
      size_t h2 = std::hash< size_t >{}(p.second);
      return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
  };

  struct PairEqual {
    bool operator()(const std::pair< size_t, size_t > &a,
        const std::pair< size_t, size_t > &b) const noexcept
    {
      return a.first == b.first && a.second == b.second;
    }
  };

  class Matrix {
   public:
    using Key = std::pair< size_t, size_t >;
    using Store = HashTable< Key, int, PairHash, PairEqual >;

    Matrix(size_t rows, size_t cols, int fill);
    Matrix();

    size_t rows() const noexcept;
    size_t cols() const noexcept;

    int get(size_t i, size_t j) const;
    void set(size_t i, size_t j, int value);

    Matrix operator+(const Matrix &other) const;
    Matrix operator-(const Matrix &other) const;
    Matrix operator*(const Matrix &other) const;

    Matrix operator*(int factor) const;
    Matrix transpose() const;

    bool operator==(const Matrix &other) const;
    bool operator!=(const Matrix &other) const;

    void insert_row(size_t index, const List< int > &values);
    void insert_col(size_t index, const List< int > &values);
    void delete_row(size_t index);
    void delete_col(size_t index);

    void append_rows(const Matrix &source, size_t start, size_t end);
    Matrix embed(size_t newRows, size_t newCols, size_t rowOffset, size_t colOffset, int fill) const;

    Matrix join_right(const Matrix &source, int fill) const;
    Matrix join_bottom(const Matrix &source, int fill) const;
    Matrix crop(size_t row, size_t col, size_t newRows, size_t newCols) const;
    void replace(const Matrix &source, size_t row, size_t col);
    Matrix flatten() const;
    Matrix repeat(size_t tileRows, size_t tileCols) const;
   private:
    size_t rows_;
    size_t cols_;
    Store data_;

    void checkBounds_(size_t i, size_t j) const;
    void checkSameDims_(const Matrix &other) const;
    size_t safeCellCount_(size_t rows, size_t cols) const;

    size_t safeAdd_(size_t a, size_t b) const;
    size_t safeMul_(size_t a, size_t b) const;
  };
}

#endif
