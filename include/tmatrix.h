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
    if (rows == 0 || rows > MAX_MATRIX_SIZE) {
      throw out_of_range("Wrong number of rows");
    }

    cols = dense[0].size();
    if (cols == 0 || cols > MAX_MATRIX_SIZE) {
      throw out_of_range("Wrong number of cols");
    }

    row_ptr.push_back(0);
    for (int i = 0; i < rows; i++) {
      if (dense[i].size() != cols)
      {
        throw invalid_argument("Rows have different sizes");
      }
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
    if (r == 0 || c == 0 || r > MAX_MATRIX_SIZE || c > MAX_MATRIX_SIZE)
      throw out_of_range("Wrong matrix size");

    if (vals.size() != c_idx.size())
      throw invalid_argument("Wrong size of vals or c_ind");

    if (r_ptr.size() != r + 1)
      throw invalid_argument("Wrong r_ptr size");

    if (r_ptr[0] != 0 || r_ptr[r] != vals.size()) {
      throw invalid_argument("Wrong r_ptr boundaries");
    }

    for (int i = 0; i < r; i++) {
      if (r_ptr[i] > r_ptr[i + 1]) {
        throw invalid_argument("Wrong r_ptr order");
      }
    }

    for (int i = 0; i < c_idx.size(); i++) {
      if (c_idx[i] >= c) {
        throw out_of_range("Wrong column index");
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
    for (int i = 0; i < rows; i++) {
      for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
        dense[i][col_index[j]] = values[j];
      }
    }
    return dense;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
    if (i >= rows || j >= cols)
      throw out_of_range("Index out of range");
    for (int k = row_ptr[i]; k < row_ptr[i + 1]; k++) {
      if (col_index[k] == j) {
        return values[k];
      }
    }
    return T(0);
  }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
    if (rows != m.rows || cols != m.cols)
      return false;
    return (values == m.values && col_index == m.col_index && row_ptr == m.row_ptr);
  }
  bool operator!=(const TCRSMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // Транспонирование матрицы (возвращает новую матрицу в формате CRS)
  TCRSMatrix transpose() const
  {
    TCRSMatrix tr_matrix(cols, rows);
    tr_matrix.values.resize(values.size());
    tr_matrix.col_index.resize(col_index.size());

    vector<int> count(cols, 0);
    for (int i = 0; i < col_index.size(); i++) {
      count[col_index[i]]++;
    }

    for (int i = 0; i < cols; i++)
      tr_matrix.row_ptr[i + 1] = tr_matrix.row_ptr[i] + count[i];

    vector<int> index(cols, 0);
    for (int i = 0; i < cols; i++) {
      index[i] = tr_matrix.row_ptr[i];
    }

    for (int i = 0; i < rows; i++) {
      for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
        tr_matrix.values[index[col_index[j]]] = values[j];
        tr_matrix.col_index[index[col_index[j]]] = i;
        index[col_index[j]]++;
      }
    }

    return tr_matrix;
  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
    if (cols != v.size())
      throw invalid_argument("Wrong vector size");
    vector<T> result(rows, 0);
    for (int i = 0; i < rows; i++) {
      for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
        result[i] += (values[j] * v[col_index[j]]);
      }
    }
    return result;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
    if (cols != m.rows)
      throw invalid_argument("Wrong matrix size");
    TCRSMatrix result(rows, m.cols);

    for (int i = 0; i < rows; i++) {
      vector<T> sum(m.cols, T(0));
      for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
        T value = values[j];
        for (int k = m.row_ptr[col_index[j]]; k < m.row_ptr[col_index[j] + 1]; k++) {
          sum[m.col_index[k]] += value * m.values[k];
        }
      }
      int count = 0;
      for (int s = 0; s < sum.size(); s++) {
        if (!isZero(sum[s])) {
          result.values.push_back(sum[s]);
          result.col_index.push_back(s);
          count++;
        }
      }
      result.row_ptr[i + 1] = result.row_ptr[i] + count;
    }
    return result;

  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
    vector<vector<T>> dense(m.rows, vector<T>(m.cols, T(0)));
    for (int i = 0; i < m.rows; i++) {
      for (int j = 0; j < m.cols; j++) {
        istr >> dense[i][j];

        if (!istr) {
            return istr;
        }
      }
    }
    m = TCRSMatrix(dense);

    return istr;
  }

friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
    vector<vector<T>> dense = m.toDense();
    for (int i = 0; i < m.rows; i++) {
      for (int j = 0; j < m.cols; j++) {
        ostr << dense[i][j] << "\t";
      }
      ostr << "\n";
    }
    return ostr;
  }
};

#endif
