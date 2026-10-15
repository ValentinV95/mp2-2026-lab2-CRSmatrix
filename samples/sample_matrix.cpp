// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Волокитин В.Д.
//
// Тестирование матриц
//---------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include "tmatrix.h"
//---------------------------------------------------------------------------

// Вспомогательная функция для наглядного вывода внутренней структуры CRS
template<typename T>
void printCRSInternals(const TCRSMatrix<T>& m, const string& name)
{
  cout << "--- Внутренняя структура CRS матрицы " << name << " ---" << endl;
  cout << "Размер: " << m.getRows() << "x" << m.getCols() 
       << ", ненулевых элементов (NNZ): " << m.getNNZ() << endl;

  cout << "values:    [ ";
  for (const auto& val : m.getValues()) cout << val << " ";
  cout << "]" << endl;

  cout << "col_index: [ ";
  for (const auto& col : m.getColIndices()) cout << col << " ";
  cout << "]" << endl;

  cout << "row_ptr:   [ ";
  for (const auto& ptr : m.getRowPtr()) cout << ptr << " ";
  cout << "]" << endl << endl;
}

int main()
{
  const size_t N = 5;
  vector<vector<int>> dense_a(N, vector<int>(N, 0));
  vector<vector<int>> dense_b(N, vector<int>(N, 0));
  vector<int> v(N, 1);

  setlocale(LC_ALL, "Russian");
  cout << "Тестирование класса работы с разреженными матрицами (CRS)" << endl << endl;

  // Заполняем исходные плотные матрицы
  for (size_t i = 0; i < N; i++)
  {
    dense_b[i][i] = (i + 1) * 10; // Матрица B — диагональная
    for (size_t j = i; j < N; j++)
    {
      dense_a[i][j] = i * 10 + j + 1; // Матрица A — верхнетреугольная
    }
  }

  // Конвертируем в формат CRS
  TCRSMatrix<int> a(dense_a);
  TCRSMatrix<int> b(dense_b);

  cout << "Matrix a = " << endl << a << endl;
  printCRSInternals(a, "a");

  cout << "Matrix b = " << endl << b << endl;

  // 1. Тестирование транспонирования
  TCRSMatrix<int> a_t = a.transpose();
  cout << "Matrix a^T (транспонированная a) = " << endl << a_t << endl;
  printCRSInternals(a_t, "a^T");

  // 2. Тестирование умножения матрицы на вектор (SpMV)
  vector<int> res_vec = a * v;
  cout << "Vector v = [ ";
  for (int x : v) cout << x << " ";
  cout << "]" << endl;

  cout << "Result a * v = [ ";
  for (int x : res_vec) cout << x << " ";
  cout << "]" << endl << endl;

  // 3. Тестирование умножения матриц (SpGEMM)
  TCRSMatrix<int> c = a * b;
  cout << "Matrix c = a * b" << endl << c << endl;
  printCRSInternals(c, "c");

  return 0;
}
//---------------------------------------------------------------------------
