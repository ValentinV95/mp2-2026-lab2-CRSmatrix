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

  cout << "Тестовое приложение" << endl;
  
  bool flag = true;
  while (flag){
    cout << "----------------------------------------" << endl;
    cout << "1. Ввести матрицу" << endl;
    cout << "2. Транспонировать матрицу" << endl;
    cout << "3. Умножить матрицу на вектор" << endl;
    cout << "4. Умножить матрицу на матрицу" << endl;
    cout << "5. Показать внутреннюю структуру CRS матрицы" << endl;
    cout << "----------------------------------------" << endl;
    cout << "0. Выход" << endl;
    int choice;
    cout << " " << endl;
    cout << "Выберите действие: ";
    cin >> choice;
    cout << " " << endl;
    switch(choice){
          case 0:
                flag = false;
                cout << "Выход из программы" << endl;
                break;
          case 1:
                cout << "Введите сначала размеры матрицы, затем элементы:" << endl;
                cin >> a;
                cout << "Матрица успешно введена:" << endl << a << endl;
                break;
          case 2:
                a_t = a.transpose();
                cout << "Транспонированная матрица:"<< endl << a_t << endl;
                printCRSInternals(a_t, "A^T");
                break;
          case 3:
                size_t size_v;
                cout << "Введите сначала длину вектора, затем элементы:" << endl;
                cin >> size_v;
                if (size_v != a.getCols()) {
                    cout << "Ошибка: длина вектора должна быть равна " << a.getCols() << endl;
                    break;
                }
                v.resize(size_v);
                for (size_t i = 0; i < size_v; i++) 
                  cin >> v[i];
                res_vec = a * v;
                cout << "Результат умножения матрицы на вектор:" << endl;
                cout << "[ ";
                for (size_t i = 0; i < res_vec.size(); i++) 
                  cout << res_vec[i] << " ";
                cout << "]" << endl;
                break;
          case 4:
                cout << "Введите сначала размеры матрицы на которую умножаем, затем элементы:" << endl;
                cin >> b;
                if (a.getCols() != b.getRows()) {
                    cout << "Ошибка: размеры матриц несовместимы для умножения!" << endl;
                    break;
                }
                c = a * b;
                cout << "Результат умножения матриц:" << endl << c << endl;
                break;
          case 5:
                printCRSInternals(a, "A");
                break;
          default:
                cout << "Неверный выбор. Попробуйте снова." << endl;
                break;
    }
  }
  return 0;
}
//---------------------------------------------------------------------------
