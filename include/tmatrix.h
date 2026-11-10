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
          throw out_of_range("dense matrix cannot be empty");
      }

      rows = dense.size();
      cols = dense[0].size();

      if (rows > MAX_MATRIX_SIZE || cols > MAX_MATRIX_SIZE) {
          throw out_of_range("the matrix dimensions exceed the MAX_MATRIX_SIZE");
      }

      row_ptr.push_back(0);

      for (size_t i = 0; i < rows; i++) {
          if (dense[i].size() != cols) {
              throw invalid_argument("rows must have the same number of col`s");
          }

          for (size_t j = 0; j < cols; j++) {
              if (!isZero(dense[i][j])) {
                  values.push_back(dense[i][j]);
                  col_index.push_back(j);
              }
          }
          row_ptr.push_back(values.size());
      }
  }

  // Конструктор напрямую из массивов CRS
  TCRSMatrix(size_t r, size_t c, const vector<T>& vals, const vector<size_t>& c_idx, const vector<size_t>& r_ptr)
  {
      if (r == 0 || r > MAX_MATRIX_SIZE || c == 0 || c > MAX_MATRIX_SIZE) {
          throw out_of_range("matrix dimensions should be between 1 and MAX_MATRIX_SIZE");
      }

      if (r_ptr.size() != r + 1) {
          throw invalid_argument("row_ptr size must be rows+1");
      }

      if (r_ptr[0] != 0 || r_ptr[r] != vals.size()) {
          throw invalid_argument("invalid row_ptr values");
      }

      if (vals.size() != c_idx.size()) {
          throw invalid_argument("values and col_index must have the same size");
      }

      for (size_t i = 0; i < r; i++) {
          if (r_ptr[i] > r_ptr[i + 1]) {
              throw invalid_argument("row_ptr must be non-decreasing");
          }
      }

      for (size_t i = 0; i < c_idx.size(); i++) {
          if (c_idx[i] >= c) {
              throw invalid_argument("column index out of range");
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
      vector<vector<T>> dense(rows, vector<T>(cols, T(0)));

      for (size_t i = 0; i < rows; i++) {
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              dense[i][col_index[j]] = values[j];
          }
      }

      return dense;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
      if (i >= rows || j >= cols) {
          throw out_of_range("index out of range");
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
      int isBool = true;

      if (rows != m.rows || cols != m.cols) {
          isBool = false;
      }

      if (values.size() != m.values.size()) {
          isBool = false;
      }

      for (size_t i = 0; i < values.size(); i++) {
          if (values[i] != m.values[i] || col_index[i] != m.col_index[i]) {
              isBool = false;
          }
      }

      for (size_t i = 0; i < row_ptr.size(); i++) {
          if (row_ptr[i] != m.row_ptr[i]) {
              isBool = false;
          }
      }

      return isBool;
  }
  bool operator!=(const TCRSMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // Транспонирование матрицы (возвращает новую матрицу в формате CRS)
  TCRSMatrix transpose() const
  {
      TCRSMatrix result(cols, rows);

      if (values.empty()) {
          return result;
      }

      vector<size_t> counts(cols, 0);
      for (size_t t = 0; t < values.size(); t++) {
          counts[col_index[t]]++;
      }

      for (size_t t = 0; t < cols; t++) {
          result.row_ptr[t + 1] = result.row_ptr[t] + counts[t];
      }

      result.values.assign(values.size(), T(0));
      result.col_index.assign(values.size(), 0);

	  vector<size_t> cursor = result.row_ptr;
      for (size_t i = 0; i < rows; i++) {
          for (size_t t = row_ptr[i]; t < row_ptr[i + 1]; t++) {
              size_t j = col_index[t];
              size_t pos = cursor[j]++;
              result.values[pos] = values[t];
              result.col_index[pos] = i;
          }
      }
	  return result;
  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
      if (v.size() != cols) {
          throw invalid_argument("vector size must match matrix col`s");
      }

      vector<T> res(rows, T(0));

      for (size_t i = 0; i < rows; i++) {
          T sumi = T(0);
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              sumi += (values[j] * v[col_index[j]]);
          }
          res[i] = sumi;
      }

      return res;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
      vector<T> new_values;
      vector<size_t> new_col_index;
      vector<size_t> new_row_ptr(rows + 1, 0);

      for (size_t i = 0; i < rows; i++) {
          vector<T> row_vals(m.cols, T(0));
          vector<bool> row_used(m.cols, false);

          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              size_t col_a = col_index[j];
              T val_a = values[j];

              for (size_t k = m.row_ptr[col_a]; k < m.row_ptr[col_a + 1]; k++) {
                  size_t col_b = m.col_index[k];
                  row_vals[col_b] += (val_a * m.values[k]);
                  row_used[col_b] = true;
              }
          }

          for(size_t t = 0; t < m.cols; t++) {
              if (row_used[t]) {
                  new_values.push_back(row_vals[t]);
                  new_col_index.push_back(t);
              }
		  }

		  new_row_ptr[i + 1] = new_values.size();
      }

      return TCRSMatrix(rows, m.cols, new_values, new_col_index, new_row_ptr);
  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
      size_t r = 0, c = 0;
      if (!(istr >> r >> c)) {
          return istr;
      }

      vector<vector<T>> dense(r, vector<T>(c));
      for (size_t i = 0; i < r; i++) {
          for (size_t j = 0; j < c; j++) {
              istr >> dense[i][j];
          }
      }

	  m = TCRSMatrix(dense);
      return istr;
  }

friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
    auto dense = m.toDense();
    for (size_t i = 0; i < m.getRows(); i++) {
        for (size_t j = 0; j < m.getCols(); j++) {
            ostr << dense[i][j];
            if (j + 1 < m.getCols()) {
                ostr << " ";
            }
        }
        ostr << "\n";
    }
    return ostr;
  }
};

#endif
