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
    if (dense.empty() || dense[0].empty())
      throw invalid_argument("error");
    if (dense.size() > MAX_MATRIX_SIZE || dense[0].size() > MAX_MATRIX_SIZE)
      throw out_of_range("error");

    rows = dense.size();
    cols = dense[0].size();
    row_ptr.assign(rows + 1, 0);

    for (size_t i = 0; i < rows; i++)
    {
      if (dense[i].size() != cols)
        throw invalid_argument("error");
        for (size_t j = 0; j < cols; j++)
        {
          if (!isZero(dense[i][j]))
          {
            values.push_back(dense[i][j]);
            cols_index.push_back(j);
          }
        }
        row_ptr[i + 1] = values.size();
    }
  }

  // Конструктор напрямую из массивов CRS
  TCRSMatrix(size_t r, size_t c, const vector<T>& vals, const vector<size_t>& c_idx, const vector<size_t>& r_ptr)
  {
    if (r == 0 || r > MAX_MATRIX_SIZE || c == 0 || c > MAX_MATRIX_SIZE)
      throw out_of_range("error");
    if (c_idx.size() != vals.size() || r_ptr.size() != r + 1 || r_ptr[0] != 0 || r_ptr[r] != vals.size())
      throw invalid_argument("error");
    for (size_t i = 0; i < r; i++)
    {
      if (r_ptr[i] > r_ptr[i + 1])
        throw invalid_argument("error");
      for (size_t k = r_ptr[i]; k < r_ptr[i + 1]; k++)
      {
        if (c_idx[k] >= c)
          throw invalid_argument("error");
        if (k > r_ptr[i] && c_idx[k] <= c_idx[k - 1])
          throw invalid_argument("error");
        if (isZero(vals[k]))
          throw invalid_argument("error");
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
    for (size_t i = 0; i < rows; i++)
      for (size_t k = row_ptr[i]; k < row_ptr[i + 1]; k++)
        dense[i][col_index[k]] = values[k];
    return dense;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
    if (i >= rows || j >= cols)
      throw out_of_range("error");
    for (int k = row_ptr[i]; k < row_ptr[i + 1]; k++){
      if (col_index[k] == j) {
        return values[k];
      }
    }
    return T(0);
  }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
    if (this == &m) return true;
    if (rows != m.rows || cols != m.cols || row_ptr != m.row_ptr || col_index != m.col_index)
      return false;
    for (size_t k = 0; k < values.size(); k++)
      if (!isZero(values[k] - m.values[k]))
        return false;
    return true;
  }
  bool operator!=(const TCRSMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // Транспонирование матрицы (возвращает новую матрицу в формате CRS)
  TCRSMatrix transpose() const
  {
    TCRSMatrix t(cols, rows);
    const size_t nnz = values.size();
    t.values.resize(nnz);
    t.col_index.resize(nnz);

    for (size_t k = 0; k < nnz; k++)
      t.row_ptr[col_index[k] + 1]++;
    for (size_t j = 0; j < cols; j++)
      t.row_ptr[j + 1] += t.row_ptr[j];
    vector<size_t> pos(t.row_ptr.begin(), t.row_ptr.end() - 1);
    for (size_t i = 0; i < rows; i++)
    {
      for (size_t k = row_ptr[i]; k < row_ptr[i + 1]; k++)
      {
        size_t p = pos[col_index[k]]++;
        t.values[p] = values[k];
        t.col_index[p] = i;
      }
    }
    return t;
  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
    if (v.size() != cols)
      throw invalid_argument("Vector size should be equal to the number of matrix columns");
    vector<T> res (rows, T(0));
    for (size_t i = 0; i < rows; i++)
    {
      Tsum = T(0);
      for (size_t k = row_ptr[i]; k < row_ptr[i + 1]; k++)
        sum += values[k] * v[col_index[k]];
      res[i] = sum;
    }
    return res;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
    if (cols != m.rows)
      throw invalid_argument("error");
      
    TCRSMatrix res(rows, m.cols);
    const size_t none = static_cast<size_t>(-1);
    vector<T> acc(m.cols, T(0));
    vector<size_t> mark(m.cols, none);
    vector<size_t> used;

    for (size_t i = 0; i < rows; i++)
    {
      used.clear();
      for(size_t k = row_ptr[i]; k < row_ptr[i + 1]; k++)
      {
        const T a = values[k];
        const size_t ja = col_index[k];
        for (size_t p = m.row_ptr[ja]; p < m.row_ptr[ja + 1]; p++)
        {
          const size_t j = m.col_index[p];
          if (mark[j] != i)
          {
            mark[j] = i;
            acc[j] = a * m.values[p];
            used.push_back(j);
          }
          else
            acc[j] += a * m.values[p];
        }
      }
    sort(used.begin(), used.end());
      for (size_t j : used)
      {
        if (!isZero(acc[j]))
        {
          res.values.push_back(acc[j]);
          res.col_index.push_back(j);
        }
      }
      res.row_ptr[i + 1] = res.values.size();
    }
    return res;
  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
    size_t r, c;
    if (!(istr >> r >> c))
      return istr;
    if (r == 0 || r > MAX_MATRIX_SIZE || c == 0 || c > MAX_MATRIX_SIZE)
    {
      istr.setstate(ios::failbit);
      return istr;
    }

    TCRSMatrix tmp(r, c);
    for (size_t i = 0; i < r; i++)
    {
      for (size_t j = 0; j < c; j++)
      {
        T x;
        if (!(istr >> x))
          return istr; 
        if (!isZero(x))
        {
          tmp.values.push_back(x);
          tmp.col_index.push_back(j);
        }
      }
      tmp.row_ptr[i + 1] = tmp.values.size();
    }
    m = std::move(tmp);
    return istr;
  }

friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
    for (size_t i = 0; i < m.rows; i++)
    {
      size_t k = m.row_ptr[i];
      for (size_t j = 0; j < m.cols; j++)
      {
        if (k < m.row_ptr[i + 1] && m.col_index[k] == j)
          ostr << m.values[k++];
        else
          ostr << T(0);
        ostr << ' ';
      }
      ostr << '\n';
    }
    return ostr;
  }
};

#endif
