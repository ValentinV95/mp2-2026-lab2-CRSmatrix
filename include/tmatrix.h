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
      if (dense.size() == 0 || dense[0].size() == 0) {
          throw invalid_argument("error");
      }
      row_ptr.push_back(0);
      rows = dense.size();
      cols = dense[0].size();
      if (rows > MAX_MATRIX_SIZE || cols > MAX_MATRIX_SIZE)
          throw out_of_range("error");
      for (int i = 0; i < rows; i++) {
          if (dense[i].size() != cols)
              throw invalid_argument("error");
      }
      for (int i = 0; i < rows; i++) {
          for (int j = 0; j < cols; j++) {
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
      if (r == 0 || c == 0 || r_ptr.size() != r + 1)
          throw invalid_argument("error");
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
      vector<T> empty(cols, T(0));
      vector<vector<T>> dense(rows, empty);
      for (int i = 0; i < rows; i++) {
          for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              int col = col_index[j];
              dense[i][col] = values[j];
          }
      }
      return dense;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
      if (i >= rows || j >= cols)
          throw out_of_range("error");

      for (int k = row_ptr[i]; k < row_ptr[i + 1]; k++) {
          if (col_index[k] == j)
              return values[k];
      }
      return 0;
  
  }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
      if (rows != m.rows || cols != m.cols || row_ptr != m.row_ptr || col_index != m.col_index)
          return false;
      
      for (int i = 0; i < values.size(); i++) {
          if (values[i] != m.values[i])
              return false;
      }
      return true;
  }
  bool operator!=(const TCRSMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // Транспонирование матрицы (возвращает новую матрицу в формате CRS)
  TCRSMatrix transpose() const
  {
      TCRSMatrix res(cols, rows);
      vector<vector<int>> temp_c(cols);
      vector<vector<T>> temp_v(cols);
      for (int i = 0; i < rows; i++) {
          for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              int old_col = col_index[j];
              T val = values[j];
              temp_c[old_col].push_back(i);
              temp_v[old_col].push_back(val);
          }
      }
      for (int i = 0; i < cols; i++) {
          for (int j = 0; j < temp_c[i].size(); j++) {
              res.col_index.push_back(temp_c[i][j]);
              res.values.push_back(temp_v[i][j]);
          }
          res.row_ptr[i + 1] = res.values.size();
      }

      return res;
  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
      if (v.size() != cols)
          throw invalid_argument("error");
      vector<T> y(rows, T(0));
      
      for (int i = 0; i < rows; i++) {
          T res = T(0);
          for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++)
              res += values[j] * v[col_index[j]];
          y[i] = res;
      }

      return y;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
      if (cols != m.rows) {
          throw invalid_argument("error");
      }
      TCRSMatrix res(rows, m.cols);
      vector<T> row(m.cols, T(0));

      for (int i = 0; i < rows; i++) {
          for (int a = row_ptr[i]; a < row_ptr[i + 1]; a++) {
              int col_a = col_index[a];
              T val_a = values[a];

              for (int b = m.row_ptr[col_a]; b < m.row_ptr[col_a + 1]; b++) {
                  T val_b = m.values[b];
                  row[m.col_index[b]] += val_a * val_b;
              }
          }

          for (int j = 0; j < m.cols; j++) {
              if (!isZero(row[j])) {
                  res.values.push_back(row[j]);
                  res.col_index.push_back(j);
              }
              row[j] = T(0);
          }
          res.row_ptr[i + 1] = res.values.size();
      }

      return res;

  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
      vector<vector<T>> matrix(m.rows, vector<T>(m.cols));
      for (int i = 0; i < m.rows; i++) {
          for (int j = 0; j < m.cols; j++) {
              istr >> matrix[i][j];
          }
      }
      m = TCRSMatrix(matrix);
      return istr;
  }

friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
    vector<vector<T>> matrix = m.toDense();
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.cols; j++)
            ostr << matrix[i][j] << "\t";
        ostr << "\n";
    }
    return ostr;
  }
};

#endif
