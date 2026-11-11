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
    if (dense.empty() || dense[0].empty()){
      throw out_of_range("Empty matrix");
    }
    rows = dense.size();
    cols = dense[0].size();

    if (rows == 0 || rows > MAX_MATRIX_SIZE || cols == 0 || cols > MAX_MATRIX_SIZE){
      throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");
    }

    for (size_t i = 0; i < rows; i++) {
      if (dense[i].size() != cols){
        throw out_of_range("The rows of the matrix have different lengths");
      }
    }
   
    row_ptr.push_back(0);

    for (size_t i = 0; i < rows;i++){
      for (size_t j = 0; j < cols;j++){

        T num = dense[i][j];
        if(!isZero(num)){
          values.push_back(num);
          col_index.push_back(j);
        }
      }
      size_t row_size = values.size();
      row_ptr.push_back(row_size);
    }
  }

  // Конструктор напрямую из массивов CRS
  TCRSMatrix(size_t r, size_t c, const vector<T>& vals, const vector<size_t>& c_idx, const vector<size_t>& r_ptr)
  {
    if (r == 0 || r > MAX_MATRIX_SIZE || c == 0 || c > MAX_MATRIX_SIZE){
      throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");
    }
    if (vals.size() != c_idx.size()){
      throw invalid_argument("The number of values and column numbers does not match");
    }
    if (r_ptr.size() != r + 1){
      throw invalid_argument("row_ptr must contain rows + 1 elements");
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
    vector<vector<T>> dense;
    dense.resize(rows, vector<T>(cols, 0));

    for (size_t i = 0; i < rows;i++){

      size_t fl = row_ptr[i + 1] - row_ptr[i]; 
      for (size_t j = 0; j < fl;j++){

        size_t k = row_ptr[i] + j;
        dense[i][col_index[k]] = values[k];
      }
    }
    return dense;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
      if (i >= rows || j >= cols){
        throw out_of_range("The index is out of the matrix bounds");
      }
    
    size_t fl = row_ptr[i + 1] - row_ptr[i]; 

    for (size_t pos = 0; pos < fl; pos++) {
        size_t idx = row_ptr[i] + pos;

        if (col_index[idx] == j) {
            return values[idx];
        }
    }

    return T(0); 
  
  }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
    if (rows != m.rows || cols != m.cols) {
        return false;
    }
    if (values != m.values) {
        return false;
    }
    if (col_index != m.col_index) {
        return false;
    }
    if (row_ptr != m.row_ptr) {
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

    vector<size_t> fl(cols + 1, 0); 

    for (size_t a_col = 0; a_col < values.size();a_col++){
      fl[col_index[a_col] + 1]++;
    }
    for (size_t a_col = 0; a_col < cols;a_col++){
      fl[a_col + 1] += fl[a_col];
    }

    res.row_ptr.resize(fl.size());
    for (size_t i = 0; i < fl.size(); i++) {
      res.row_ptr[i] = fl[i];
    }
    res.values.resize(values.size());
    res.col_index.resize(values.size());

    vector<size_t> copy_fl = fl; 

    for (size_t a_row = 0; a_row < rows; a_row++) {

      for (size_t a_pos = row_ptr[a_row]; a_pos < row_ptr[a_row + 1]; a_pos++) {

        size_t a_col = col_index[a_pos];
        size_t at_pos = copy_fl[a_col];

        res.values[at_pos] = values[a_pos];
        res.col_index[at_pos] = a_row;
        copy_fl[a_col]++;
      }
    }
    return res;
  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
      if (cols != v.size()){
        throw invalid_argument("Different matrix sizes");
      }

    vector<T> new_v(rows, T(0));
    for (size_t i = 0; i < rows;i++){
      T sum = 0;
      for (size_t j = row_ptr[i]; j < row_ptr[i + 1];j++){
        sum += values[j] * v[col_index[j]];
      }
      new_v[i] = sum;
    }
    return new_v;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
    if (cols != m.rows) {
      throw invalid_argument("Different matrix sizes");
    }
    vector<T> AB_values; 
    vector<size_t> AB_cols;
    vector<size_t> AB_row_ptr;
    AB_row_ptr.push_back(0);

    vector<T> sum_row(m.cols, T(0));
    for (size_t i = 0; i < rows;i++){

      for (size_t a_pos = row_ptr[i]; a_pos < row_ptr[i + 1]; a_pos++){

        size_t b_row = col_index[a_pos];
        T a_val = values[a_pos];

        for (size_t b_pos = m.row_ptr[b_row]; b_pos < m.row_ptr[b_row + 1];b_pos++){

          size_t b_col = m.col_index[b_pos];
          sum_row[b_col] += a_val * m.values[b_pos];
        }
      }
      for (size_t j = 0; j < m.cols;j++){

        if (!isZero(sum_row[j])){

        AB_values.push_back(sum_row[j]);
        AB_cols.push_back(j);
        }
        sum_row[j] = T(0);
      }
      size_t row_end = AB_values.size();
      AB_row_ptr.push_back(row_end);
    }

    return TCRSMatrix(rows, m.cols, AB_values, AB_cols, AB_row_ptr);
  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
    size_t new_r, new_c;
    istr >> new_r >> new_c;

    vector<vector<T>> v(new_r, vector<T>(new_c, T(0)));

    for (size_t i = 0; i < new_r; i++) {
      for (size_t j = 0; j < new_c; j++) {

          istr >> v[i][j];
      }
    }
    m = TCRSMatrix(v);

    return istr;
  }

  friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
    vector<vector<T>> v = m.toDense();

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
          ostr << v[i][j] << " ";
        }
        ostr << "\n";
    }
    return ostr;
  }
};

#endif
