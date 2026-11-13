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

template<typename T>
TCRSMatrix<T> inputDenseMatrix()
{
    TCRSMatrix<T> m;

    cout << "\nВведите количество строк, столбцов, элементы построчно:" << endl;
    cin >> m;

    return m;
}

template<typename T>
vector<T> inputVector(size_t size)
{
    vector<T> vec(size);
    cout << "\nВведите элементы вектора длиной " << size << endl;
    for (size_t i = 0; i < size; i++) {
        cin >> vec[i];
    }
    return vec;
}

int main()
{
  setlocale(LC_ALL, "Russian");
  cout << "Тестирование класса работы с разреженными матрицами (CRS)" << endl << endl;

  //const size_t N = 5;
  //vector<vector<int>> dense_a(N, vector<int>(N, 0));
  //vector<vector<int>> dense_b(N, vector<int>(N, 0));
  //vector<int> v(N, 1);

  //// Заполняем исходные плотные матрицы
  //for (size_t i = 0; i < N; i++)
  //{
  //  dense_b[i][i] = (i + 1) * 10; // Матрица B — диагональная
  //  for (size_t j = i; j < N; j++)
  //  {
  //    dense_a[i][j] = i * 10 + j + 1; // Матрица A — верхнетреугольная
  //  }
  //}

  //// Конвертируем в формат CRS
  //TCRSMatrix<int> a(dense_a);
  //TCRSMatrix<int> b(dense_b);

  //cout << "Matrix a = " << endl << a << endl;
  //printCRSInternals(a, "a");

  //cout << "Matrix b = " << endl << b << endl;

  //// 1. Тестирование транспонирования
  //TCRSMatrix<int> a_t = a.transpose();
  //cout << "Matrix a^T (транспонированная a) = " << endl << a_t << endl;
  //printCRSInternals(a_t, "a^T");

  //// 2. Тестирование умножения матрицы на вектор (SpMV)
  //vector<int> res_vec = a * v;
  //cout << "Vector v = [ ";
  //for (int x : v) cout << x << " ";
  //cout << "]" << endl;

  //cout << "Result a * v = [ ";
  //for (int x : res_vec) cout << x << " ";
  //cout << "]" << endl << endl;

  //// 3. Тестирование умножения матриц (SpGEMM)
  //TCRSMatrix<int> c = a * b;
  //cout << "Matrix c = a * b" << endl << c << endl;
  //printCRSInternals(c, "c");

  //=====МЕНЮ==================================================

  TCRSMatrix<int> matrixA;
  TCRSMatrix<int> matrixB;
  bool hasA = false;
  bool hasB = false;

  int choice = 0;
  do
  {
      cout << "\n================ МЕНЮ ================" << endl;

      cout << "1. Задать матрицу A (плотный формат)" << endl;
      cout << "2. Задать матрицу B (плотный формат)" << endl;
      cout << "3. Загрузить стандартный пример (базовый тест)" << endl;
      cout << "4. Вывести текущие матрицы A и B" << endl;
      cout << "5. Транспонирование матрицы (на выбор)" << endl;
      cout << "6. Умножение матрицы A на вектор (с вводом вектора)" << endl;
      cout << "7. Умножение матриц A * B" << endl;
      cout << "0. Выход" << endl;

      cout << "Выберите действие: ";
      cin >> choice;

      switch (choice) {
          case 1: {
              try {
                  cout << "\n===== Ввод матрицы A =====" << endl;
                  matrixA = inputDenseMatrix<int>();
                  hasA = true;
                  cout << "Матрица введена" << endl;
              }
              catch (const exception& e) {
                  cout << "Ошибка: " << e.what() << endl;
              }
              break;
          }
          case 2: {
              try {
                  cout << "\n===== Ввод матрицы B =====" << endl;
                  matrixB = inputDenseMatrix<int>();
                  hasB = true;
                  cout << "Матрица введена" << endl;
              }
              catch (const exception& e) {
                  cout << "Ошибка: " << e.what() << endl;
              }
              break;
          }
          case 3: {
              size_t N = 5;
              vector<vector<int>> dense_a(N, vector<int>(N, 0));
              vector<vector<int>> dense_b(N, vector<int>(N, 0));

              for (size_t i = 0; i < N; i++)
              {
                  dense_b[i][i] = (i + 1) * 10; // Матрица B — диагональная
                  for (size_t j = i; j < N; j++)
                  {
                      dense_a[i][j] = i * 10 + j + 1; // Матрица A — верхнетреугольная
                  }
              }

              matrixA = TCRSMatrix<int>(dense_a);
              matrixB = TCRSMatrix<int>(dense_b);

              hasA = true;
              hasB = true;

              cout << "\nМатрицы базового (исходного) теста введёны" << endl;
              break;
          }
          case 4: {
              if (hasA) {
                  cout << "\nМатрица A =\n" << matrixA << endl;
                  printCRSInternals(matrixA, "A");
              }
              else {
                  cout << "\nМатрица A не введена" << endl;
              }

              if (hasB) {
                  cout << "\nМатрица B =\n" << matrixB << endl;
                  printCRSInternals(matrixB, "B");
              }
              else {
                  cout << "\nМатрица B не введена" << endl;
              }
              break;
          }
          case 5: {
              if (!hasA && !hasB) {
                  cout << "\nОбе матрицы не заданы" << endl;
                  break;
              }
              int trans_choice;
              cout << "Выбор матрицы ( 1-A , 2-B ): ";
              cin >> trans_choice;

              if (trans_choice == 1) {
                  if (hasA) {
                      matrixA = matrixA.transpose();
                      cout << "\nТранспонированная матрица A =\n" << matrixA << endl;
                      printCRSInternals(matrixA, "A^T");
                  }
                  else {
                      cout << "Матрица не задана" << endl;
                  }
              }
              else if (trans_choice == 2) {
                  if (hasB) {
                      matrixB = matrixB.transpose();
                      cout << "\nТранспонированная матрица B =\n" << matrixB << endl;
                      printCRSInternals(matrixB, "B^T");
                  }
                  else {
                      cout << "Матрица не задана" << endl;
                  }
              }
              else {
                  cout << "Неправильный номер" << endl;
              }
              break;
          }
          case 6: {
              if (!hasA) {
                  cout << "\nМатрица A не задана" << endl;
                  break;
              }
              try {
                  cout << "\nУмножение матрицы A на вектор:" << endl;

                  vector<int> vec = inputVector<int>(matrixA.getCols());
                  vector<int> result_vec = matrixA * vec;

                  cout << "\nВектор = [ ";
                  for (size_t i = 0; i < vec.size();i++)
                      cout << vec[i] << " ";
                  cout << "]" << endl;

                  cout << "Результат = [ ";
                  for (size_t i = 0; i < result_vec.size();i++)
                      cout << result_vec[i] << " ";
                  cout << "]" << endl;
              }
              catch (const exception& e) {
                  cout << "Ошибка: " << e.what() << endl;
              }
              break;
          }
          case 7: {
              if (!hasA || !hasB) {
                  cout << "\nЗаданы не обе матрицы" << endl;
                  break;
              }
              try {
                  TCRSMatrix<int> result = matrixA * matrixB;
                  cout << "\nРезультат (A * B) =\n" << result << endl;
                  printCRSInternals(result, "Result");
              }
              catch (const exception& e) {
                  cout << "Ошибка: " << e.what() << endl;
              }
              break;
          }
          case 0: {
              cout << "\nВыход из программы\n" << endl;
              break;
          }
          default: {
              cout << "\nНеправильный номер\n" << endl;
              break;
          }
      }
  } while (choice != 0);

  return 0;
}
//---------------------------------------------------------------------------
