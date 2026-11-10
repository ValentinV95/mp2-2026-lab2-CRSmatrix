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
      rows = dense.size();
      if (rows != 0) {
          cols = dense[0].size();
      }
      else
      {
          cols = 0;
      }
      if (rows == 0 || rows > MAX_MATRIX_SIZE || cols == 0 || cols > MAX_MATRIX_SIZE) {
          throw out_of_range("");
      }
      row_ptr.assign(rows + 1, 0);
      for (size_t i = 0; i < rows; i++) {
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
      if ((r < 1) || (r > MAX_MATRIX_SIZE) || (c < 1) || (c > MAX_MATRIX_SIZE)) {
          throw out_of_range("");
      }
      if (vals.size() != c_idx.size()) {
          throw invalid_argument("");
      }
      if ((r_ptr.size() != r + 1)||(r_ptr[0]!=0)) {
          throw invalid_argument("");
      }
      for (size_t i = 0; i < r_ptr.size()-1;i++) {
          if (r_ptr[i] > r_ptr[i + 1]) {
              throw invalid_argument("");
          }
      }
      if (r_ptr.back() != vals.size()) {
          throw invalid_argument("");
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
      vector<vector<T>> result(rows, vector<T>(cols,T(0)));
      for (size_t i = 0; i<rows; i++) {
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1] ; j++) {
              result[i][col_index[j]] = values[j];  
          }
      }
      return result;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
      if (i >= rows || j >= cols) {
          throw out_of_range("");
      }
      for (size_t t = row_ptr[i]; t < row_ptr[i + 1]; t++) {
          if (col_index[t] == j) {
              return values[t];
          }
      }
      return T(0);
  }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
      return((rows == m.rows) && (cols == m.cols) && (values == m.values) && (col_index == m.col_index) && (row_ptr == m.row_ptr));
  }
  bool operator!=(const TCRSMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // Транспонирование матрицы (возвращает новую матрицу в формате CRS)
  TCRSMatrix transpose() const
  {
      TCRSMatrix result(cols, rows);
      if (values.size() == 0) {
          return result;
      }

      result.values.assign(values.size(), T(0));
      result.col_index.assign(col_index.size(), 0);
      vector<size_t> col_sum(cols, 0);
      for (size_t i = 0; i < col_index.size(); i++) {
          col_sum[col_index[i]]++;
      }

      for (size_t i = 0; i < cols; i++) {
          result.row_ptr[i + 1] = result.row_ptr[i] + col_sum[i];
      }
      vector<size_t> next_ptr = result.row_ptr;
      for (size_t i = 0; i < rows; i++) {
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              size_t where = next_ptr[col_index[j]]++;
              result.values[where] = values[j];
              result.col_index[where] = i;
          }
      }
      return result;
  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
      if (v.size() != cols) {
          throw invalid_argument("");
      }
      vector<T> result(rows, T(0));
      for (size_t i = 0; i < rows; i++) {
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              result[i] += values[j] * v[col_index[j]];
          }
      }
      return result;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
      if (cols != m.rows) {
          throw invalid_argument("");
      }
      size_t crows = rows;
      size_t ccols = m.cols;
      vector<T> cvalues;
      vector<size_t> ccol_index;
      vector<size_t> crow_ptr(crows+1,0);

      vector<T> row_acc(ccols, T(0));
      for (size_t i = 0; i < rows; i++) {
          for (size_t k = row_ptr[i]; k < row_ptr[i + 1]; k++) {
              T A_ik = values[k];
              size_t B_k_row = col_index[k];
              for (size_t t = m.row_ptr[B_k_row]; t < m.row_ptr[B_k_row + 1]; t++) {
                  size_t j = m.col_index[t];
                  row_acc[j] += A_ik * m.values[t];
              }
          }
          for (size_t j = 0; j < ccols; j++) {
              if (!isZero(row_acc[j])) {
                  cvalues.push_back(row_acc[j]);
                  ccol_index.push_back(j);
              }
              row_acc[j] = T(0);
          }
          crow_ptr[i + 1] = cvalues.size();
      }
      return TCRSMatrix(crows, ccols, cvalues, ccol_index, crow_ptr);
  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
      size_t r, c;
      istr >> r >> c;
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
    for (size_t i = 0; i < m.rows; i++) {
          size_t k = m.row_ptr[i];
          for (size_t j = 0; j < m.cols; j++) {
              if (k < m.row_ptr[i + 1] && m.col_index[k] == j) {
                  ostr << m.values[k] << " ";
                  k++;
              }
              else {
                  ostr << T(0) << " ";
              }
          }
          ostr << "\n";
      }
      return ostr;
  }
};

#endif
