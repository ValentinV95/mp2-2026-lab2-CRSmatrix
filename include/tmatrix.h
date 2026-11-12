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
      if (dense.empty() || dense[0].empty()) {
          throw invalid_argument("Dense matrix cannot be empty");
      }
      rows = dense.size();
      cols = dense[0].size();
      if (rows > MAX_MATRIX_SIZE || cols > MAX_MATRIX_SIZE)
          throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");

      row_ptr.assign(rows + 1, 0);
      for (size_t i = 0; i < rows; i++) {
          if (dense[i].size() != cols) {
              throw invalid_argument("Rows have different length");
          }
          for (size_t j = 0; j < cols; j++) {
              if (!isZero(dense[i][j])) {
                  values.push_back(dense[i][j]);
                  col_index.push_back(j);
              }
          }
          row_ptr[i + 1] = values.size();
      }
  }

  // Конструктор напрямую из массивов CRS
  TCRSMatrix(size_t r, size_t c, const vector<T>& vals, const vector<size_t>& c_idx, const vector<size_t>& r_ptr)
  {
      if (r == 0 || r > MAX_MATRIX_SIZE || c == 0 || c > MAX_MATRIX_SIZE)
          throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");
      if (vals.size() != c_idx.size()) {
          throw invalid_argument("Values and col_index have different size");
      }
      if (r_ptr.size() != r + 1) {
          throw invalid_argument("Row_ptr must have size rows + 1");
      }
      if (r_ptr[0] != 0) {
          throw invalid_argument("Row_ptr[0] must be 0");
      }
      if (r_ptr[r] != vals.size()) {
          throw invalid_argument("Row_ptr[r] must equal vals.size()");
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
      vector<vector<T>> res(rows, vector<T>(cols, T(0)));
      for (size_t i = 0; i < rows; i++) {
          for (size_t k = row_ptr[i]; k < row_ptr[i+1]; k++) {
              res[i][col_index[k]] = values[k];
          }
      }
      return res;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
      if (i >= rows || j >= cols) {
          throw out_of_range("Index out of range");
      }
      for (size_t k = row_ptr[i]; k < row_ptr[i + 1]; k++) {
          if (col_index[k] == j) {
                  return values[k];
              }
      }
      return T(0);
    }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
      return (rows == m.rows && cols == m.cols && row_ptr == m.row_ptr && col_index == m.col_index && values == m.values);
  }
  bool operator!=(const TCRSMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // Транспонирование матрицы (возвращает новую матрицу в формате CRS)
  TCRSMatrix transpose() const
  {
      vector<vector<T>> val(cols);
      vector<vector<size_t>> row(cols);

      for (size_t i = 0; i < rows; i++) {
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              size_t c = col_index[j];
              val[c].push_back(values[j]);
              row[c].push_back(i);
          }
      }

      TCRSMatrix tmatrix_CRS(cols, rows);
      for (size_t i = 0; i < cols; i++) {
          for (size_t j = 0; j < val[i].size(); j++) {
              tmatrix_CRS.values.push_back(val[i][j]);
              tmatrix_CRS.col_index.push_back(row[i][j]);
          }
          tmatrix_CRS.row_ptr[i + 1] = tmatrix_CRS.values.size();
      }
      return tmatrix_CRS;
  }
  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
      if (v.size() != cols) {
          throw invalid_argument("The size of the vector must be equal to the number of columns");
      }
      vector<T> res(rows, T(0));
      for (size_t i = 0; i < rows; i++) {
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              res[i] += values[j] * v[col_index[j]];
          }
      }
      return res;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
      if (cols != m.rows) {
          throw invalid_argument("The number of columns in the left matrix must be equal to the number of rows in the right matrix");
      }
      vector<T> res_values;
      vector<size_t> res_col_index;
      vector<size_t> res_row_ptr;
      res_row_ptr.push_back(0);

      vector<T> row_tmp(m.cols, T(0));

      for (size_t i = 0; i < rows; i++) {
          for (size_t m1 = row_ptr[i]; m1 < row_ptr[i + 1]; m1++) {
              size_t col_m1 = col_index[m1];
              T values_m1 = values[m1];

              for (size_t m2 = m.row_ptr[col_m1]; m2 < m.row_ptr[col_m1 + 1]; m2++) {
                  size_t col_m2 = m.col_index[m2];
                  T values_m2 = m.values[m2];

                  row_tmp[col_m2] += values_m1 * values_m2;
              }
          }
          for (size_t j = 0; j < m.cols; j++) {
              if (!isZero(row_tmp[j])) {
                  res_values.push_back(row_tmp[j]);
                  res_col_index.push_back(j);
              }
              row_tmp[j] = T(0);
          }
          res_row_ptr.push_back(res_values.size());
      }
      return TCRSMatrix(rows, m.cols, res_values, res_col_index, res_row_ptr);
  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
      size_t r, c;
      if (!(istr >> r >> c)) {
          return istr;
      }
      if (r == 0 || r > MAX_MATRIX_SIZE || c == 0 || c > MAX_MATRIX_SIZE)
          throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");
      vector<vector<T>> matrix(r, vector<T>(c, T(0)));
      for (size_t i = 0; i < r; i++) {
          for (size_t j = 0; j < c; j++) {
              istr >> matrix[i][j];
          }
      }
      m = TCRSMatrix(matrix);
      return istr;
  }

  friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
   
      vector<vector<T>> matrix = m.toDense();
      for (size_t i = 0; i < m.rows; i++) {
          for (size_t j = 0; j < m.cols; j++) {
              ostr << matrix[i][j];
              if (j + 1 < m.cols) {
                  ostr << " ";
              }
          }
          ostr << "\n";
      }
      return ostr;
  }
};

#endif
