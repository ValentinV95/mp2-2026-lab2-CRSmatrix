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
  setlocale(LC_ALL, "Russian");

  int rows, cols;

  cout << "Введите количество строк и столбцов матрицы A: ";
  cin >> rows >> cols;

  if (!cin || rows <= 0 || cols <= 0 || rows > MAX_MATRIX_SIZE || cols > MAX_MATRIX_SIZE) {
    cout << "Некорректные размеры\n";
    return 1;
  }

  TCRSMatrix<int> a(rows, cols);

  cout << "Введите элементы матрицы A:\n";
  cin >> a;

  if (!cin) {
    cout << "Некорректный ввод\n";
    return 1;
  }

  int choice;

  while (true) {
    cout << "\n";
    cout << "1. Транспонировать матрицу\n";
    cout << "2. Умножить матрицу на вектор\n";
    cout << "3. Умножить матрицу на матрицу\n";
    cout << "4. Показать матрицу и CRS-массивы\n";
    cout << "5. Выход\n";
    cout << "Выберите действие: ";

    cin >> choice;

    if (!cin) {
      cout << "Ошибка ввода\n";
      return 1;
    }

    switch (choice) {
      case 1: {
        TCRSMatrix<int> transposed = a.transpose();

        cout << "Транспонированная матрица A:\n";
        cout << transposed << "\n";

        printCRSInternals(transposed, "A^T");

        break;
      }

      case 2: {
        vector<int> v(cols);

        cout << "Введите элементы вектора (" << cols << " штук)\n";

        for (int i = 0; i < cols; i++) {
          cin >> v[i];

          if (!cin) {
            cout << "Некорректный ввод\n";
            return 1;
          }
        }

        vector<int> result = a * v;

        cout << "Результат:\n";

        for (int i = 0; i < result.size(); i++) {
          cout << result[i] << " ";
        }

        cout << "\n";

        break;
      }

      case 3: {
        int rowsB, colsB;

        cout << "Введите количество строк и столбцов матрицы B: ";
        cin >> rowsB >> colsB;

        if (!cin || rowsB <= 0 || colsB <= 0 || rowsB > MAX_MATRIX_SIZE || colsB > MAX_MATRIX_SIZE) {
          cout << "Некорректные размеры\n";
          return 1;
        }

        if (cols != rowsB) {
          cout << "Некорректные размеры\n";
          break;
        }

        TCRSMatrix<int> b(rowsB, colsB);

        cout << "Введите элементы матрицы B:\n";
        cin >> b;

        if (!cin) {
          cout << "Некорректный ввод\n";
          return 1;
        }

        TCRSMatrix<int> result = a * b;

        cout << "Результат:\n";
        cout << result << "\n";

        printCRSInternals(result, "A * B");

        break;
      }

      case 4: {
        cout << "Матрица A:\n";
        cout << a << "\n";

        printCRSInternals(a, "A");

        break;
      }

      case 5: {
        return 0;
      }

      default: {
        cout << "Некорректный ввод\n";
        break;
      }
    }
  }
}