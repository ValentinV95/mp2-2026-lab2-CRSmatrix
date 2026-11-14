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
#include <type_traits>

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
      if (dense.empty())
          throw out_of_range("Matrix must not be empty");

      for (size_t i = 0; i < dense.size()-1; i++) {
          if(dense[i].size() != dense[i+1].size())
              throw out_of_range("Matrix rows must have equal length");
      }

      rows = dense.size();
      cols = dense[0].size();
    
      if (rows > MAX_MATRIX_SIZE || cols == 0 || cols > MAX_MATRIX_SIZE || rows == 0)
          throw out_of_range("Invalid matrix argument in constructur");

      size_t count_for_values = 0;
      
      for (size_t i = 0; i < rows; i++) {

          row_ptr.push_back(count_for_values);
          
          for (size_t j = 0; j < cols; j++) {
              
              T current_tmp = dense[i][j];

              if (!isZero(current_tmp)) {
                 
                  values.push_back(current_tmp);
                  col_index.push_back(j);
                  count_for_values++;
              
              }
          }
      }
      row_ptr.push_back(count_for_values);
  }


  // Конструктор напрямую из массивов CRS
  TCRSMatrix(size_t r, size_t c, const vector<T>& vals, const vector<size_t>& c_idx, const vector<size_t>& r_ptr)
  {
      if (r == 0 || c == 0 || r > MAX_MATRIX_SIZE || c > MAX_MATRIX_SIZE || vals.size() > MAX_VECTOR_SIZE || c_idx.size() > MAX_VECTOR_SIZE || r_ptr.size() > MAX_VECTOR_SIZE)
          throw out_of_range("Invalid matrix argument in constructur");

      if (vals.size() != c_idx.size())
          throw invalid_argument("Values and column must be the same size");

      if (r_ptr.size() != r+1)
          throw invalid_argument("Invalid row pointer size");

      for (size_t i = 0; i < r; i++) {
          if(r_ptr[i] > r_ptr[i+1])
              throw invalid_argument("Row pointers must be nondecreasing");
      }

      if (r_ptr.front() != 0 || r_ptr.back() != vals.size())
          throw invalid_argument("Invalid row pointer");

      for (size_t col : c_idx) {
          if (col >= c)
              throw out_of_range("Column index is out of range");
      }

      for (size_t i = 0; i < r; i++) {
          for (size_t k = r_ptr[i]; k < r_ptr[i + 1]; k++) {
              if (k > r_ptr[i] && c_idx[k - 1] >= c_idx[k])
                  throw invalid_argument(
                      "Column indices must be strictly increasing within a row");
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
      vector<vector<T>> tmp(rows, vector<T>(cols, T(0)));

      for (size_t i = 0; i < row_ptr.size() - 1; i++) {

        

          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              tmp[i][col_index[j]] = values[j];

          }
      }
      return tmp;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {

      if (i >= rows || j >= cols)
          throw out_of_range("incorect index in at");

      size_t count_tmp_in_row = row_ptr[i + 1] - row_ptr[i];
      if (count_tmp_in_row == 0) return 0;

      for (size_t k = row_ptr[i + 1] - count_tmp_in_row; k < row_ptr[i + 1]; k++) {
          if (j == col_index[k]) return values[k];

      }
      return 0;
    }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
      if (m.cols == cols && m.rows == rows && m.col_index == col_index && m.values == values && m.row_ptr == row_ptr) {
          return 1;
      }
      else return 0;
  }
  bool operator!=(const TCRSMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // Транспонирование матрицы (возвращает новую матрицу в формате CRS)
  TCRSMatrix transpose() const
  {
      vector<size_t> count_value_colum(cols); // количество чисел в столбце до транспонирования
      vector<size_t> tmp_row_ind(cols + 1, 0);
      vector<T> tmp_values(values.size());
      vector<size_t> tmp_col_index(values.size());

      for (size_t i = 0; i < col_index.size(); i++) {
          count_value_colum[col_index[i]]++;
      }
        
      for (size_t i = 1; i < cols + 1; i++) {
          tmp_row_ind[i] = tmp_row_ind[i - 1] + count_value_colum[i - 1];
          
      }
      vector<size_t> next = tmp_row_ind;
      
      for (size_t i = 0; i < rows; i++) {
          for (size_t k = row_ptr[i]; k < row_ptr[i + 1]; k++) {
              
              size_t j = col_index[k];
              size_t pos = next[j]++;

              tmp_values[pos] = values[k];
              tmp_col_index[pos] = i;
          }
      }
      return TCRSMatrix(cols,rows,tmp_values,tmp_col_index,tmp_row_ind);
  }
  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {

      if (v.size() != cols)
          throw out_of_range("vector leght must be the same with number of colums");

      vector<T> tmp(rows, T(0));
      for (size_t i = 0; i < rows; i++) {
          for (size_t k = row_ptr[i]; k < row_ptr[i + 1]; k++) {
              tmp[i] += values[k] * v[col_index[k]];
          }
      }
      return tmp;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {

      if (cols != m.rows)
          throw invalid_argument("Two matrix must have same cols count and rows count argument");

      TCRSMatrix tmp(rows, m.cols);


      for (size_t i = 0; i < rows; i++) {

          vector<T>acamulation(m.cols, T(0));

          for (size_t k = row_ptr[i]; k < row_ptr[i + 1]; k++) {
              
              size_t m_row = col_index[k];

              for (size_t j = m.row_ptr[m_row]; j < m.row_ptr[m_row + 1]; j++) {

                  acamulation[m.col_index[j]] += values[k] * m.values[j];

              }
          }

          for (size_t col = 0; col < m.cols; col++) {
              if (!isZero(acamulation[col])) {
                  tmp.values.push_back(acamulation[col]);
                  tmp.col_index.push_back(col);
              }
          }

          tmp.row_ptr[i + 1] = tmp.values.size();
      }
      return tmp;
  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
      size_t r, c, nnz;

      if (!(istr >> r >> c >> nnz))
          throw invalid_argument("Failed to read matrix argument in istream");


      if (r == 0 || c == 0 || r > MAX_MATRIX_SIZE || c > MAX_MATRIX_SIZE || nnz > r * c)
      {
          throw out_of_range("Invalid matrix argument in istram");

      }

      vector<T> vals(nnz);
      vector<size_t> c_idx(nnz);
      vector<size_t> r_ptr(r + 1);

      for (auto& value : vals)
          if (!(istr >> value))
              return istr;

      for (auto& col : c_idx)
          if (!(istr >> col))
              return istr;

      for (auto& ptr : r_ptr)
          if (!(istr >> ptr))
              return istr;

      m = TCRSMatrix(r, c, vals, c_idx, r_ptr);
     
    return istr;
  }

  friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
      ostr << "Values: ";
      for (const auto& i : m.getValues())
          ostr << i << ' ';

      ostr << "\nCol Index: ";
      for (size_t i : m.getColIndices())
          ostr << i << ' ';

      ostr << "\nRow ptr: ";
      for (size_t i : m.getRowPtr())
          ostr << i << ' ';

      ostr << '\n';
      return ostr;
  }
};

#endif
