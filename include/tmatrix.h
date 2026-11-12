// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Волокитин В.Д.
//
//

#ifndef __TMatrix_H__
#define __TMatrix_H__

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <cmath>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Разреженная матрица в формате CRS (Compressed Row Storage)
template<typename T>
class TCRSMatrix
{
protected:
    size_t rows;               // количество строк
    size_t cols;               // количество столбцов
    vector<T> values;          // ненулевые элементы (размер NNZ)
    vector<size_t> col_index;  // столбцовые индексы ненулевых элементов (размер NNZ)
    vector<size_t> row_ptr;    // индексы начала строк в массивах values и col_index (размер rows + 1)

    static bool isZero(const T& val)
    {
        if constexpr (std::is_floating_point_v<T>)
            return std::abs(val) < 1e-12;
        else
            return val == T(0);
    }

public:
    TCRSMatrix(size_t r = 1, size_t c = 0)
    {
        if (c == 0) c = r; // если передан 1 аргумент, создаем квадратную матрицу r x r
        if (r == 0 || r > MAX_MATRIX_SIZE || c == 0 || c > MAX_MATRIX_SIZE)
            throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");

        rows = r;
        cols = c;
        row_ptr.assign(rows + 1, 0);
    }

    // Конструктор конвертации из плотной матрицы
    TCRSMatrix(const vector<vector<T>>& dense)
    {
        size_t row_ind = 0;
        if (dense.empty())
            throw invalid_argument("Dense matrix is empty");
        cols = dense.at(0).size();
        rows = dense.size();
        if (rows > MAX_MATRIX_SIZE || cols > MAX_MATRIX_SIZE || rows == 0 || cols == 0) {
            throw out_of_range("Incorrect data in constructor");
        }
        for (size_t i = 0; i < rows; i++) {
            if (dense[i].size() != cols)
                throw invalid_argument("Array has strings of different lengths");
        }
        row_ptr.push_back(0);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                if (!isZero(dense[i][j])) {
                    values.push_back(dense[i][j]);
                    col_index.push_back(j);
                    row_ind++;
                }
            }
            row_ptr.push_back(row_ind);
        }
    }

    // Конструктор напрямую из массивов CRS
    TCRSMatrix(size_t r, size_t c, const vector<T>& vals, const vector<size_t>& c_idx, const vector<size_t>& r_ptr)
    {
        if (r > MAX_MATRIX_SIZE || c > MAX_MATRIX_SIZE || vals.size() > MAX_VECTOR_SIZE || c_idx.size() > MAX_VECTOR_SIZE || r_ptr.size() > MAX_VECTOR_SIZE || c_idx.size() != vals.size() || r_ptr.size() != r + 1) {
            throw out_of_range("Invalid data in constructor");
        }
        for (size_t i = 0; i < r_ptr.size() - 1; i++) {
            if (r_ptr[i] > r_ptr[i + 1]) {
                throw invalid_argument("Incorrectly entered row indices");
            }
        }
        if (r_ptr.front() != 0 || r_ptr.back() != vals.size()) {
            throw invalid_argument("Invalid rows value");
        }
        for (size_t i = 0; i < c_idx.size(); i++) {
            if (c_idx[i] >= c) {
                throw out_of_range("The row index is outside the matrix boundaries.");
            }
        }
        rows = r;
        cols = c;
        values = vals;
        col_index = c_idx;
        row_ptr = r_ptr;
    }

    TCRSMatrix(const TCRSMatrix& m) = default;
    TCRSMatrix(TCRSMatrix&& m) noexcept = default;
    ~TCRSMatrix() = default;
    TCRSMatrix& operator=(const TCRSMatrix& m) = default;
    TCRSMatrix& operator=(TCRSMatrix&& m) noexcept = default;

    size_t getRows() const noexcept { return rows; }
    size_t getCols() const noexcept { return cols; }
    size_t getNNZ() const noexcept { return values.size(); } // Number of Non-Zeros

    const vector<T>& getValues() const noexcept { return values; }
    const vector<size_t>& getColIndices() const noexcept { return col_index; }
    const vector<size_t>& getRowPtr() const noexcept { return row_ptr; }

    // Конвертация обратно в плотную матрицу (для проверки корректности операций)
    vector<vector<T>> toDense() const
    {
        vector<vector<T>> res(rows, vector<T>(cols, 0));
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++){
                res[i][col_index[j]] = values[j];
            }
        }
    return res;
    }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
      if (i >= rows || j >= cols) {
          throw invalid_argument("Incorrect index");
      }
      size_t el_in_row = row_ptr[i + 1] - row_ptr[i];
      if (el_in_row == 0) {
          return T(0);
      }
      for (size_t t = row_ptr[i]; t < row_ptr[i + 1]; t++) {
          if (j == col_index[t]) {
              return values[t];
          }
      }
      return T(0);
  }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
      return (m.cols == cols && m.rows == rows && m.col_index == col_index && m.values == values && m.row_ptr == row_ptr);
  }
  bool operator!=(const TCRSMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // Транспонирование матрицы (возвращает новую матрицу в формате CRS)
  TCRSMatrix transpose() const
  {
      TCRSMatrix res(cols, rows);
      res.row_ptr.assign(cols + 1, 0);
      res.values.clear();
      res.col_index.clear();
      vector<vector<size_t>> t_col(cols);
      vector<vector<T>> t_val(cols);
      for (size_t i = 0; i < rows; ++i) {
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              size_t main_col = col_index[j];
              t_col[main_col].push_back(i);
              t_val[main_col].push_back(values[j]);
          }
      }
      for (size_t i = 0; i < cols; i++) {
          for (size_t j = 0; j < t_col[i].size(); j++) {
              res.col_index.push_back(t_col[i][j]);
              res.values.push_back(t_val[i][j]);
          }
          res.row_ptr[i + 1] = static_cast<size_t>(res.values.size());
      }
      return res;
  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
      if (v.size() != cols)
          throw invalid_argument("Inconsistent dimensions of the matrix and the vector");

      vector<T> res(rows, 0);
      for (size_t i = 0; i < rows; i++) {
          T sum = 0;
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++)
              sum += values[j] * v[col_index[j]];
          res[i] = sum;
      }

      return res;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
      if (cols != m.rows){
          throw invalid_argument("Inconsistent matrix dimensions");
      }
      TCRSMatrix res(rows, m.cols);
      res.values.reserve(values.size());
      res.col_index.reserve(col_index.size());
      res.row_ptr.assign(rows + 1, size_t{ 0 });
      size_t x_back, y_back, x_col;
      vector<T> row(m.cols, 0);

      for (size_t i = 0; i < rows; i++) {
          x_back = row_ptr[i + 1];

          for (size_t x = row_ptr[i]; x < x_back; x++) {
              x_col = col_index[x];
              const T& val_x = values[x];

              y_back = m.row_ptr[x_col + 1];
              for (size_t y = m.row_ptr[x_col]; y < y_back; y++) {
                  row[m.col_index[y]] += val_x * m.values[y];
              }
          }
          for (size_t j = 0; j < m.cols; j++) {
              if (!isZero(row[j])) {
                  res.values.push_back(row[j]);
                  res.col_index.push_back(j);
              }
              row[j] = 0;
          }
          res.row_ptr[i + 1] = res.values.size();
      }
      return res;
  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
      size_t is_row, is_col, is_nnz;
      if (!(istr >> is_row >> is_col >> is_nnz)) {
          throw invalid_argument("Failed to read matrix argument in istream");
      }
      if (is_row == 0 || is_col == 0 || is_row > MAX_MATRIX_SIZE || is_col > MAX_MATRIX_SIZE || is_nnz > is_row * is_col) {
          throw out_of_range("Invalid matrix argument in istream");
      }
      vector<T> is_vals(is_nnz);
      vector<size_t> is_col_idx(is_nnz);
      vector<size_t> is_row_ptr(is_row + 1);
      for (size_t i = 0; i < is_nnz; i++) {
          if (!(istr >> is_vals[i])) {
              return istr;
          }
      }
      for (size_t i = 0; i < is_nnz; i++) {
          if (!(istr >> is_col_idx[i])) {
              return istr;
          }
      }
      for (size_t i = 0; i < is_row + 1; i++) {
          if (!(istr >> is_row_ptr[i])) {
              return istr;
          }
      }
      m = TCRSMatrix(is_row, is_col, is_vals, is_col_idx, is_row_ptr);
      return istr;
  }

friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
    ostr << "Values: ";
    for (const auto& i : m.getValues()) {
        ostr << i << ' ';
    }
    ostr << "\nCol Index: ";
    for (size_t i : m.getColIndices()) {
        ostr << i << ' ';
    }
    ostr << "\nRow ptr: ";
    for (size_t i : m.getRowPtr()) {
        ostr << i << ' ';
    }
    ostr << '\n';
    return ostr;
  }
};

#endif
