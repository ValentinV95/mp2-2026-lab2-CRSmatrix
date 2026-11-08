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
#include <utility>

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
    if (dense.empty()) {
      throw out_of_range("Матрица нулевого размера");
    }
    rows = dense.size();
    if (rows <= 0 || rows > MAX_MATRIX_SIZE) {
      throw out_of_range("матрица невозможных размеров");
    }
    cols = dense[0].size();
    row_ptr.push_back(0);
    if (cols <= 0 || cols > MAX_MATRIX_SIZE) {
      throw out_of_range("матрица невозможных размеров");
    }
    for (size_t i = 0; i < rows; i++) {
      size_t cols1 = dense[i].size();
      if (cols != cols1) {
        throw out_of_range("кол-во столбцов не равно другим");
      }
      for (size_t j = 0; j < cols; j++){
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
    rows = r;
    cols = c;
    if (rows == 0 || rows > MAX_MATRIX_SIZE){
      throw out_of_range("матрица неправильных размеров");
    }
    if (cols == 0 || cols > MAX_MATRIX_SIZE) {
      throw out_of_range("матрица неправильных размеров");
    }
    if (r_ptr.size() != r + 1) {
      throw out_of_range("массив индексов строк указан неправильно");
    }
    if (r_ptr[0] != 0 || r_ptr[r] != vals.size()) {
      throw out_of_range("массив индексов начала/конца строки указан неверно");
    }
    for (size_t i = 1; i < r+1; i++){
      if (r_ptr[i] < r_ptr[i-1] || r_ptr[i] > vals.size()) {
        throw out_of_range("массив индексов строки указан неверно");
      }
    }
    if (vals.size() != c_idx.size()) {
      throw out_of_range("Размеры не совпадают");
    }
    for (size_t i = 0; i < vals.size(); i++) {
      if (c_idx[i] >= cols) {
        throw out_of_range("Несуществующий индекс колонки");
      }
    }
    col_index = c_idx;
    row_ptr = r_ptr;
    values = vals;
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
    int t = 0;
    for (size_t i = 1; i < row_ptr.size(); i++){
      if (row_ptr[i] != row_ptr[i-1]) {
        for (size_t j = row_ptr[i-1]; j < row_ptr[i]; j++){
          res[t][col_index[j]] = values[j];
        }
      }
      t++;
    }
    return res;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
    if (i >= rows || j >= cols) {
      throw out_of_range("обращение к несуществующему эл-ту");
    }
    for (size_t k = row_ptr[i]; k < row_ptr[i+1]; k++){
      if (col_index[k] == j) {
        return values[k];
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
    else if (col_index != m.col_index || row_ptr != m.row_ptr) {
      return false;
    }
    else {
      for (size_t i = 0; i < values.size(); i++){
        if (values[i] != m.values[i]) {
          return false;
        }
      }
      return true;
    }
  }
  bool operator!=(const TCRSMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // Транспонирование матрицы (возвращает новую матрицу в формате CRS)
  TCRSMatrix transpose() const
  {
    vector<size_t> col_id;
    vector<size_t> row_id;
    row_id.push_back(0);
    vector<T> res;
    vector<vector<pair<T, size_t>>> prom(this->cols);
    for (size_t i = 0; i < this->rows; i++){
      for (size_t j = this->row_ptr[i]; j < this->row_ptr[i+1]; j++){
        
        size_t new_row = this->col_index[j];
        size_t new_col = i;
        prom[new_row].push_back({this->values[j], new_col});
      }
    }
    for (size_t i = 0; i < this->cols; i++) {
      for (size_t j = 0; j < prom[i].size(); j++){
        res.push_back(prom[i][j].first);
        col_id.push_back(prom[i][j].second);
      }
      row_id.push_back(res.size());
    }
    return TCRSMatrix(this->cols, this->rows, res, col_id, row_id);
  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
    if (v.size() != cols) {
      throw invalid_argument("Несовместимые размеры");
    }
    vector<T> res(rows, T(0));

    for (size_t i = 0; i < rows; i++) {
      T sum = T(0);
      for (size_t j = row_ptr[i]; j < row_ptr[i+1]; j++) {
        sum += values[j] * v[col_index[j]];
      }
      res[i] = sum;
    }
    return res;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {

    if (cols != m.rows){
      throw invalid_argument("матрицы неправильных размеров");
    }

    vector<T> res;
    vector<size_t> col_i;
    vector<size_t> row_i = {0};
    for (size_t i = 0; i < rows; i++) {
      vector<T> sum(m.cols, T(0));
      for (size_t j = row_ptr[i]; j < row_ptr[i+1]; j++) {
        size_t k = col_index[j];
        for (size_t t = m.row_ptr[k]; t < m.row_ptr[k+1]; t++){
          size_t col = m.col_index[t];
          sum[col] += values[j] * m.values[t];
        }
      }
      for (size_t j = 0; j < m.cols; j++) {
        if (!isZero(sum[j])) {
          res.push_back(sum[j]);
          col_i.push_back(j);
        }
      }
      row_i.push_back(res.size());
    }
    return TCRSMatrix(rows, m.cols, res, col_i, row_i);
  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
    size_t r, c;
    if (!(istr >> r >> c)) return istr;
    if (r == 0 || r > MAX_MATRIX_SIZE || c == 0 || c > MAX_MATRIX_SIZE) {
      return istr;
    }
    vector<vector<T>> dense(r, vector<T>(c));
    for (size_t i = 0; i < r; i++) {
      for (size_t j = 0; j < c; j++) {
        istr >> dense[i][j];
      }
    }
    if (istr) {
      m = TCRSMatrix(dense);
    }
    return istr;
  }

  friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
    vector<vector<T>> dense = m.toDense();
    for (size_t i = 0; i < m.rows; i++) {
      for (size_t j = 0; j < m.cols; j++) {
        ostr << dense[i][j] << '\t';
      }
      ostr << '\n';
    }
    return ostr;
  }
};

#endif
