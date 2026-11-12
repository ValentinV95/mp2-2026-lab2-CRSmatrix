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

TCRSMatrix<int> inputM() {
    size_t r, c;
    cout << "Введите количество строк и столбцов: ";
    cin >> r >> c;
    vector<vector<int>> dense(r, vector<int>(c, 0));
    cout << "Введите элементы матрицы: " << endl;
    for (size_t i = 0; i < r; i++) {
        for (size_t j = 0; j < c; j++) {
            cin >> dense[i][j];
        }
    }
    return TCRSMatrix<int>(dense);
}

int main()
{
  setlocale(LC_ALL, "Russian");
  TCRSMatrix<int> matrixA;
  TCRSMatrix<int> matrixB;

  int choice = 0;

  while (true) {

    cout << "1. Ввести текущую матрицу" << endl;
    cout << "2. Показать текущую матрицу" << endl;
    cout << "3. Показать CRS матрицу для текущей" << endl;
    cout << "4. Транспонировать текущую матрицу" << endl;
    cout << "5. Умножить текущую матрицу на вектор" << endl;
    cout << "6. Умножить текущую матрицу на другую" << endl;

    cout << "Ваш выбор: ";

    cin >> choice;
    try{
      switch (choice) {
        case 1: {
          cout << "\nВведите текущую матрицу" << endl;
          matrixA = inputM();
          break;
        }
        case 2: {
          cout << "\nТекущая матрица:" << endl;
          cout << matrixA;
          break;
        }
        case 3: {
          printCRSInternals(matrixA, "A");
          break;
        }
        case 4: {
          matrixA = matrixA.transpose();
          cout << "\nРезультат транспонирования" << endl;
          cout << matrixA;
          break;
        }
        case 5: {
          vector<int> vec(matrixA.getCols());
          
          cout << "Введите " << vec.size() << " элементов вектора:" << endl;
          for (size_t i = 0; i < vec.size(); i++) {
              cin >> vec[i];
          }
          
          vector<int> res = matrixA * vec;
          
          cout << "Результат: ";
          for (size_t i = 0; i < res.size(); i++) {
              cout << res[i] << " ";
          }
          cout << endl;
          break;
        }
        
        case 6: { 
          size_t r, c;
          cout << "Введите количество строк и столбцов второй матрицы: ";
          cin >> r >> c;
          
          vector<vector<int>> dense(r, vector<int>(c, 0));
          cout << "Введите элементы матрицы: " << endl;

          for (size_t i = 0; i < r; i++) {
              for (size_t j = 0; j < c; j++) {
                  cin >> dense[i][j];
              }
          }
          
          matrixB = TCRSMatrix<int>(dense);
          
          TCRSMatrix<int> res = matrixA * matrixB;
          
          cout << "\nРезультат умножения матриц :" << endl;
          cout << res;
          break;
        }
        default: {
          cout << "Нет такого пункта в меню" << endl;
          break;
      }
    }
  }
  catch (const exception& err) {
      cout << "\nОШИБКА: " << err.what() << endl;
      break;
  }
}
  return 0;
}
//---------------------------------------------------------------------------

