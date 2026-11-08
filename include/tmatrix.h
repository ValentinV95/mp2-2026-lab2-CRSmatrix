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
      if (dense.empty() || dense[0].empty() || dense.size() > MAX_MATRIX_SIZE || dense[0].size() > MAX_MATRIX_SIZE)
          throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");

      rows = dense.size();
      cols = dense[0].size();
      row_ptr.assign(rows + 1, 0);

      for (size_t i = 0; i < rows; i++)
      {
          if (dense[i].size() != cols)    // проверка на то, что все строки одной длины
              throw invalid_argument("All rows must have the same lenght");

          for (size_t j = 0; j < cols; j++)
          {
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
      if (r == 0 || c == 0 || r > MAX_MATRIX_SIZE || c > MAX_MATRIX_SIZE)
          throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");

      if (vals.size() != c_idx.size())
          throw invalid_argument("Size of values and columns index vector must be same");

      if (r_ptr.size() != r + 1)
          throw invalid_argument("Row pointer vector must be rows+1 size");

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
          size_t start_ptr = row_ptr[i];
          size_t end_ptr = row_ptr[i + 1];
          for (size_t k = start_ptr; k < end_ptr; k++) {
              res[i][col_index[k]] = values[k];
          }
      }

      return res;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
      if (i >= rows || j >= cols)    //если передать отрицательное число, то будет неявное пеобразование в большое положительное
          throw out_of_range("Indexes are out of matrix");

      size_t row_start = row_ptr[i];
      size_t row_end = row_ptr[i + 1];

      for (size_t k = row_start; k < row_end; k++) {
          if (col_index[k] == j)
              return values[k];
          if (col_index[k] > j)    //так как столбцы уже отсортированны, то если был пройден j, то он не будет найден и далее
              return T(0);
      }
      return T(0);
  }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
      if (rows != m.rows() || cols != m.cols() || values.size() != m.values.size())
          return false;

      if (row_ptr != m.row_ptr())
          return false;

      if (col_index != m.col_index())
          return false;

      for (size_t i = 0; i < values.size(); i++) {
          if (!isZero(values[i] - m.values[i]))
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
      if (values.size() == 0)
          return TCRSMatrix(cols, rows);

      vector<size_t> count_col(cols, 0);    //эл-тов в каждой строке транспонированной матрицы
      for (size_t i = 0; i < values.size(); i++)
      {
          count_col[col_index[i]]++;
      }

      vector<size_t> transpose_row_ptr = vector<size_t>(cols + 1, 0);    //row_ptr в транспонированной матрице
      for (size_t j = 0; j < cols; j++) {
          transpose_row_ptr[j + 1] = transpose_row_ptr[j] + count_col[j];
      }

      vector<T> transpose_values(values.size());
      vector<size_t> transpose_col_index(values.size());

      vector<size_t> current_ptr = transpose_row_ptr;
      for (size_t i = 0; i < rows; i++) {
          for (size_t k = row_ptr[i]; k < row_ptr[i + 1]; k++) {
              size_t j = col_index[k];    //Столбец в изначальной матрице станет строкой в транспонированной 
              size_t free_pose = current_ptr[j];

              transpose_values[free_pose] = values[k];
              transpose_col_index[free_pose] = i;
              current_ptr[j]++;
          }
      }

      return TCRSMatrix(cols, rows, transpose_values, transpose_col_index, transpose_row_ptr);
  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
      if (v.size() != cols)
          throw invalid_argument("Matrix and vector must be the same size");

      vector<T> res(rows, 0);
      for (size_t i = 0; i < rows; i++) {
          T sum = 0;
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              sum += values[j] * v[col_index[j]];
          }
          res[i] = sum;
      }
      return res;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
      if (cols != m.rows)
          throw invalid_argument("Matrices can not be multiplied");

      TCRSMatrix trans_m(m.transpose());
      TCRSMatrix res(rows, m.cols);

      for (size_t i = 0; i < rows; i++)   //цикл по строкам матрицы A
      {
          for (size_t j = 0; j < trans_m.rows; j++)     //цикл по столбцам матрицы B
          {
              //указатели на начало строк в матрице A и B^T
              size_t ptr_A = row_ptr[i];
              size_t ptr_B_transp = trans_m.row_ptr[j];

              T sum = 0;

              //пока не дошли до последних элементов в строках матриц A и B^T
              while ((ptr_A < row_ptr[i + 1]) && (ptr_B_transp < trans_m.row_ptr[j + 1]))
              {
                  size_t col_ptr1 = col_index[ptr_A];
                  size_t col_ptr2 = trans_m.col_index[ptr_B_transp];

                  if (col_ptr1 < col_ptr2) { ptr_A++; }  //эл-т в A есть, в B^T нет

                  else if (col_ptr2 < col_ptr1) { ptr_B_transp++; }    //эл-т в B^T есть, в A нет

                  else if (col_ptr1 == col_ptr2)
                  {
                      sum += values[ptr_A] * trans_m.values[ptr_B_transp];
                      ptr_A++;
                      ptr_B_transp++;
                  }
              }

              if (!isZero(sum))
              {
                  res.values.push_back(sum);
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
      size_t row, col;
      istr >> row >> col;
      
      if (!istr)
          throw invalid_argument("Failed to read matrix dimensions");
      
      if (row == 0 || row > MAX_MATRIX_SIZE || col == 0 || col > MAX_MATRIX_SIZE)
          throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");

      m.rows = row;
      m.cols = col;
      m.values.clear();
      m.col_index.clear();
      m.row_ptr.assign(row+1, 0);

      for (size_t i = 0; i < m.rows; i++) 
      {
          for (size_t j = 0; j < m.cols; j++)
          {
              T val;
              if (!(istr >> val))
                  throw invalid_argument("Premature end of stream");


              if (!m.isZero(val)) 
              {
                  m.values.push_back(val);
                  m.col_index.push_back(j);
              }
          }

          m.row_ptr[i + 1] = m.values.size();
      }

      return istr;
  }

  friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
    for (size_t i = 0; i < m.rows; i++) 
    {
        size_t start_ptr = m.row_ptr[i];
        size_t end_ptr = m.row_ptr[i + 1];

        for (size_t j = 0; j < m.cols; j++) 
        {
            if ((start_ptr < end_ptr) && (m.col_index[start_ptr] == j))
            {
                ostr << m.values[start_ptr] << " ";
                start_ptr++;
            }
            else { ostr << T(0) << " "; }
        }

		ostr << endl;
    }

    return ostr;
  }
};

#endif
