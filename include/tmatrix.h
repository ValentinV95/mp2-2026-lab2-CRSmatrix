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
    if(dense.empty() || dense[0].empty())
      throw out_of_range("Размеры матрицы должны быть от 1 до MAX_MATRIX_SIZE ");
    rows = dense.size();
    cols = dense[0].size();

    if(rows > MAX_MATRIX_SIZE || cols > MAX_MATRIX_SIZE)
      throw out_of_range("Размеры матрицы должны быть от 1 до MAX_MATRIX_SIZE");
    row_ptr.assign(rows +1, 0);

    for(size_t i = 0;i < rows;i++){
      if (dense[i].size() != cols)
        throw invalid_argument("Все строки плотной матрицы должны иметь одинаковую длину");

      for (size_t j = 0; j < cols; ++j){
        if (!isZero(dense[i][j]))         
        {
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
    if(r == 0 || r > MAX_MATRIX_SIZE || c == 0 || c > MAX_MATRIX_SIZE)
      throw out_of_range("Размеры матрицы должны быть от 1 до MAX_MATRIX_SIZE");
    if(r_ptr.size() != r + 1)
      throw invalid_argument("Размер row_ptr должен быть равен числу строк + 1");
    if(vals.size() != c_idx.size())
      throw invalid_argument("Массивы values и col_index должны быть одного размера");
    if(r_ptr[0] != 0 || r_ptr[r] != vals.size())
      throw invalid_argument("Должно быть row_ptr[0] == 0 и row_ptr[rows] == NNZ");
    
    for(size_t i = 0; i < r; i++){
      if(r_ptr[i] > r_ptr[i + 1])
        throw invalid_argument("row_ptr не должен убывать");
      for(size_t j = r_ptr[i];j < r_ptr[i+1];j++){
        if(c_idx[j] >=c)
          throw out_of_range("Индекс столбца выходит за границы матрицы");
        if(j > r_ptr[i] && c_idx[j] <= c_idx[j - 1])
          throw invalid_argument("Индексы столбцов в строке должны возрастать");
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
    for(size_t i = 0; i < rows; i++){
      for(size_t j = row_ptr[i]; j < row_ptr[i+1];j++){
        dense[i][col_index[j]] = values[j];
      }
    }
    return dense;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
    if(i >= rows || j >= cols)
      throw out_of_range("Индекс элемента выходит за границы матрицы");
    for(size_t k = row_ptr[i]; k < row_ptr[i+1];k++){
      if(col_index[k] == j)
        return values[k];
    }
    return T(0);
    }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
    if (rows != m.rows) 
      return false;           
    if (cols != m.cols)
      return false;           
    if (row_ptr != m.row_ptr)
      return false;     
    if (col_index != m.col_index)
      return false; 
    if (values != m.values)
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
    TCRSMatrix res(cols, rows);
    const size_t nnz = values.size();

    for(size_t j = 0;j < nnz;j++)
      res.row_ptr[col_index[j]+1]++;
    for(size_t c = 0; c < cols; ++c)
      res.row_ptr[c + 1] += res.row_ptr[c];

    res.values.resize(nnz);
    res.col_index.resize(nnz);
    vector<size_t> pos(res.row_ptr.begin(), res.row_ptr.end()-1);

    for(size_t i = 0;i < rows; i++){
      for(size_t j = row_ptr[i];j < row_ptr[i+1];j++){
        size_t c = col_index[j];
        size_t p = pos[c];
        pos[c]++;
        res.values[p] = values[j];
        res.col_index[p] = i;
      }
    }
    return res;

  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
    if(v.size() != cols)
      throw invalid_argument("Размер вектора должен быть равен числу столбцов матрицы");
    vector<T> res(rows,T(0));
  
    for(size_t i = 0;i < rows;i++)
      for(size_t j = row_ptr[i]; j < row_ptr[i+1];j++)
        res[i] += values[j] * v[col_index[j]];
    
    return res;

  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
    if(cols != m.rows)
      throw invalid_argument("Размеры матриц не подходят для умножения");
    TCRSMatrix res(rows, m.cols);
    
    for(size_t i = 0;i < rows; i++){
      vector<T> acc(m.cols,T(0));
      for(size_t k = row_ptr[i];k < row_ptr[i+1];k++){
        size_t b_row = col_index[k];
        
        for(size_t j = m.row_ptr[b_row]; j < m.row_ptr[b_row+1];j++){
          acc[m.col_index[j]] += values[k] * m.values[j];
        }
      }
      for(size_t c = 0;c < m.cols;c++){
        if(!isZero(acc[c])){
          res.values.push_back(acc[c]);
          res.col_index.push_back(c);
        }
      }
      res.row_ptr[i+1] = res.values.size();
    }
    return res;
  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
    size_t r,c;
    istr >> r >> c;

    if(r == 0 || r > MAX_MATRIX_SIZE || c == 0 || c > MAX_MATRIX_SIZE)
      throw out_of_range("Размеры матрицы должны быть от 1 до MAX_MATRIX_SIZE");
    vector<vector<T>> dense(r, vector<T>(c));
    for(size_t i = 0; i < r; ++i)
      for(size_t j = 0; j < c; ++j)
      istr >> dense[i][j];
    m = TCRSMatrix(dense);

    return istr;
  }

friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
    vector<vector<T>> dense = m.toDense();
    for(size_t i = 0;i < m.rows;i++){
      for(size_t j = 0;j < m.cols;j++){
        if(j > 0)
          ostr << ' ';
        ostr << dense[i][j];
      }
      ostr << '\n';
    }
    return ostr;
  }
};

#endif
