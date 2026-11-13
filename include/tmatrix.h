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
          throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");

      rows = dense.size();
      cols = dense[0].size();

      if (rows > MAX_MATRIX_SIZE || cols > MAX_MATRIX_SIZE)
          throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");

      row_ptr.assign(rows + 1, 0);

      for (size_t i = 0;i < rows; i++) {
          if (dense[i].size() != cols)
              throw invalid_argument("All columns must be same size");

          for (size_t j = 0;j < cols; j++) {
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
      if (r == 0 || c == 0)
          throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");

      if (r > MAX_MATRIX_SIZE || c > MAX_MATRIX_SIZE)
          throw out_of_range("Matrix dimensions should be between 1 and MAX_MATRIX_SIZE");

      if (vals.size() != c_idx.size())
          throw invalid_argument("Size of values and columns index vector must be same");

      if (r_ptr.size() != r + 1)
          throw invalid_argument("Row pointer vector must be rows+1 size");

      for (size_t i = 0;i < c_idx.size();i++) {
          if (c_idx[i] + 1 > c)
              throw out_of_range("Column indexes out of range");
      }

      if (r_ptr[0] != 0)
          throw invalid_argument("First row pointer must be 0");

      if (r_ptr[r] != values.size())
          throw invalid_argument("Last row pointer element must equal amount of values");

      for (size_t i = 0;i < r_ptr.size()-1;i++) {
          if (r_ptr[i] > r_ptr[i+1])
              throw out_of_range("Row pointers must be increasing");
      }

      for (size_t i = 0;i < r_ptr.size()-1;i++) {
          for (size_t j = r_ptr[i]; j+1 < r_ptr[i + 1];j++) {
              if (c_idx[j] >= c_idx[j + 1])
                  throw invalid_argument("Column indexes inside one row must increase");
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
      vector<vector<T>> result(rows,vector<T>(cols, T(0) ));

      for (size_t r_ptr_pointer = 0;r_ptr_pointer < rows;r_ptr_pointer++) {
          for (size_t values_pointer = row_ptr[r_ptr_pointer];values_pointer < row_ptr[r_ptr_pointer + 1];values_pointer++) {
              result[r_ptr_pointer][col_index[values_pointer]] = values[values_pointer];
          }
      }

      return result;
  }

  // Получение элемента по индексам (i, j)
  T at(size_t i, size_t j) const
  {
      //i - row | j - column
      if (i <0 || j<0 || i > rows - 1 || j>cols - 1)
          throw invalid_argument("Indexes are out of matrix");

      size_t row_start_pointer = row_ptr[i];
      size_t row_end_pointer = row_ptr[i+1];
      if (row_start_pointer == row_end_pointer)
          return T(0);

      for (int k = row_start_pointer; k < row_end_pointer;k++) {
          if (col_index[k] == j)
              return values[k];
      }
      return T(0);
  }

  // Сравнение матриц
  bool operator==(const TCRSMatrix& m) const noexcept
  {
      return (rows == m.rows && cols == m.cols && values == m.values && col_index == m.col_index && row_ptr == m.row_ptr);
  }
  bool operator!=(const TCRSMatrix& m) const noexcept
  {
    return !(*this == m);
  }

  // Транспонирование матрицы (возвращает новую матрицу в формате CRS)
  TCRSMatrix transpose() const
  {
      TCRSMatrix result(cols, rows);

      size_t nnz = values.size();
      if (nnz == 0)
          return result;

      result.values.assign(nnz, T(0));
      result.col_index.assign(nnz, T(0));

      vector<size_t> col_count(cols, 0);
      //эл-тов в каждом столбце
      for (size_t i = 0; i < nnz; i++) {
          col_count[col_index[i]]++;
      }

      //row_ptr в новой матрице
      result.row_ptr[0] = 0;
      for (size_t i = 0; i < cols; i++) {
          result.row_ptr[i + 1] = result.row_ptr[i] + col_count[i];
      }

      //перенос значений
      vector<size_t> clone_row_ptr = result.row_ptr; // копия, не ссылка
      for (size_t i = 0; i < rows; ++i) {
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              size_t place_pos = clone_row_ptr[col_index[j]]++;
              result.values[place_pos] = values[j];
              result.col_index[place_pos] = i;
          }
      }
      return result;
  }

  // Умножение CRS матрицы на плотный вектор (SpMV)
  vector<T> operator*(const vector<T>& v) const
  {
      if (v.size() != cols)
          throw invalid_argument("Vector size must equals to number of columns");

      vector<T> result(rows, T(0));
      
      for (size_t i = 0;i < rows;i++) {
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              result[i] += values[j] * v[col_index[j]];
          }
      }
      return result;
  }

  // Умножение двух CRS матриц (SpGEMM) -> результат тоже в формате CRS
  TCRSMatrix operator*(const TCRSMatrix& m) const
  {
      if (cols != m.getRows())
          throw invalid_argument("Matrix columns and rows must match for multiplying");

      // изначальная A , вторая B, резултат Result

      size_t result_rows = rows;
      size_t result_cols = m.getCols();
      //размеров не знаем поэтому задаём все компоненты класса отдельно
      vector<T> result_values;
      vector<size_t> result_col_index;
      vector<size_t> result_row_ptr;

      result_row_ptr.push_back(0);

      vector<T> temp_result_row(result_cols, T(0));
      vector<bool> temp_col_visited(result_cols, false);
      vector<size_t> temp_col_index;

      for (size_t i = 0; i < result_rows; i++) {
          temp_col_index.clear();

          // проход по строке A
          for (size_t j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
              T value_a = values[j];
              size_t col_a = col_index[j];

              // проход по соответствующей столбцу из A строке из B и умножение
              for (size_t k = m.row_ptr[col_a]; k < m.row_ptr[col_a + 1]; k++) {
                  T value_b = m.values[k];
                  size_t col_b = m.col_index[k];

                  if (!temp_col_visited[col_b]) {
                      temp_col_visited[col_b] = true;
                      temp_col_index.push_back(col_b);
                  }
                  temp_result_row[col_b] += value_a * value_b;
              }
          }
          // сорт сохранённых номеров столбцов чтобы они были по возрастанию при записи
          sort(temp_col_index.begin(), temp_col_index.end());


          // записываем в результат всё что не ноль
          for (size_t temp_col_pointer = 0; temp_col_pointer < temp_col_index.size();temp_col_pointer++) {
              size_t col_b_pointer = temp_col_index[temp_col_pointer];
              if (!isZero(temp_result_row[col_b_pointer])) {
                  result_values.push_back(temp_result_row[col_b_pointer]);
                  result_col_index.push_back(col_b_pointer);
              }
              // убираем из временных переменных то что уже добавили
              temp_result_row[col_b_pointer] = T(0);
              temp_col_visited[col_b_pointer] = false;
          }
          //след строка результата
          result_row_ptr.push_back(result_values.size());
      }
      return TCRSMatrix(result_rows, result_cols, result_values, result_col_index, result_row_ptr); // собираем результат
  }

  // Ввод/вывод (в плотном виде или в виде CRS-структуры)
  // плотный вид выбран
  friend istream& operator>>(istream& istr, TCRSMatrix& m)
  {
      size_t i_rows, i_cols;
      istr >> i_rows >> i_cols;

      vector<vector<T>> dense(i_rows, vector<T>(i_cols));

      for (size_t i = 0; i < i_rows; ++i) {
          for (size_t j = 0; j < i_cols; ++j) {
              istr >> dense[i][j];
          }
      }
      m = TCRSMatrix(dense);

      return istr;
  }

friend ostream& operator<<(ostream& ostr, const TCRSMatrix& m)
  {
      ostr << m.rows << " " << m.cols << "\n";

      vector<vector<T>> dense = m.toDense();

      for (size_t i = 0; i < m.rows; ++i) {
          for (size_t j = 0; j < m.cols; ++j) {
              ostr << dense[i][j] << "\t";
          }
          ostr << "\n";
      }
      return ostr;
  }
};

#endif
