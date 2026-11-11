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
    if (dense.empty()) 
        cols = 0;
    else 
        cols = dense[0].size();
    if (rows == 0 || rows > MAX_MATRIX_SIZE || cols == 0 || cols > MAX_MATRIX_SIZE)
      throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");
    row_ptr.assign(rows + 1, 0);

    for (size_t i = 0; i < rows; i++){
      if (dense[i].size() != cols)
        throw invalid_argument("Matrix dimension isnt correct");

      for (size_t j = 0; j < cols; j++){
        if (!isZero(dense[i][j])){
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
    if (vals.size() != c_idx.size())
      throw invalid_argument("Size of values and col_index should be equal - size NNZ");
    if (r_ptr.size() != r + 1)
      throw invalid_argument("Size of row_ptr should be count of rows plus one");
    if (r_ptr[0] != 0) 
      throw invalid_argument("First argument of row_ptr should be zero");
    if (r_ptr[r_ptr.size() - 1] != vals.size())
      throw invalid_argument("Last argument of row_ptr should be size of values");
    for (size_t i = 0; i < r_ptr.size() - 1; i++) {
      if (r_ptr[i] > r_ptr[i + 1])
        throw invalid_argument("Row_ptr should be sorted in increasing order");
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
    for (size_t i = 0; i < rows; i++)
      for (size_t nz = row_ptr[i]; nz < row_ptr[i+1]; nz++)
        dense[i][col_index[nz]] = values[nz];
    return dense;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
    if (i >= rows || j >= cols)
      throw out_of_range("Index out of range");
    for (size_t nz = row_ptr[i]; nz < row_ptr[i + 1]; nz++)
      if (col_index[nz] == j)
        return values[nz];
    return T(0);
  }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
    if (rows != m.rows || cols != m.cols) return false;
    if (col_index != m.col_index) return false;
    if (values != m.values) return false;
    if (row_ptr != m.row_ptr) return false;
    return true;
  }
  bool operator!=(const TCRSMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // Транспонирование матрицы (возвращает новую матрицу в формате CRS)
  TCRSMatrix transpose() const
  {
    vector<vector<T>> valueVectors(cols);
    vector<vector<size_t>> indexVectors(cols);

    for (size_t i = 0; i < rows; i++) {
        for (size_t nz = row_ptr[i]; nz < row_ptr[i + 1]; nz++) {
            size_t j = col_index[nz];
            T v = values[nz];

            indexVectors[j].push_back(i);
            valueVectors[j].push_back(v);
        }
    }

    TCRSMatrix res(cols, rows);
    res.row_ptr[0] = 0;

    for (size_t j = 0; j < cols; j++) {
        for (size_t k = 0; k < valueVectors[j].size(); k++) {
            res.values.push_back(valueVectors[j][k]);
            res.col_index.push_back(indexVectors[j][k]);
        }
        res.row_ptr[j + 1] = res.values.size();
    }

    return res;
  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
    if (v.size() != cols)
      throw invalid_argument("Vector size should be equal to count of matrix columns");
    vector<T> res(rows, T(0));
    for (size_t i = 0; i < rows; i++)
      for (size_t nz = row_ptr[i]; nz < row_ptr[i + 1]; nz++)
        res[i]+= values[nz] * v[col_index[nz]];
    return res;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
    if (cols != m.rows) 
      throw invalid_argument("Matrix dimensions incorrect for multiplication");

    TCRSMatrix res(rows, m.cols);
    vector<T> temp(m.cols, T(0));

    for (size_t i = 0; i < rows; i++) {
      
      for (size_t nza = row_ptr[i]; nza < row_ptr[i + 1]; nza++) {
        size_t a_col = col_index[nza];
        T a_val = values[nza];

        for (size_t n = m.row_ptr[a_col]; n < m.row_ptr[a_col + 1]; n++) {
          size_t b_col = m.col_index[n];
          T b_val = m.values[n];

          temp[b_col] += a_val * b_val;
        }
      }

      for (size_t j = 0; j < m.cols; j++) {
        if (!isZero(temp[j])) {
          res.values.push_back(temp[j]);
          res.col_index.push_back(j);
        }
        temp[j] = T(0);
      }
      res.row_ptr[i + 1] = res.values.size();
    }
    return res;
  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
    size_t r, c;
    istr >> r >> c;
    
    if (r == 0 || r > MAX_MATRIX_SIZE || c == 0 || c > MAX_MATRIX_SIZE)
      throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");

    vector<vector<T>> dense(r, vector<T>(c));
    for (size_t i = 0; i < r; i++)
      for (size_t j = 0; j < c; j++)
        istr >> dense[i][j];
    m = TCRSMatrix<T>(dense);
    return istr;
  }

friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
    vector<vector<T>> dense = m.toDense();
    for (size_t i = 0; i < m.rows; i++){
      for (size_t j = 0; j < m.cols; j++){
        ostr << dense[i][j] << " ";
      }
      ostr << endl;
    }
    return ostr;
  }
};

#endif
