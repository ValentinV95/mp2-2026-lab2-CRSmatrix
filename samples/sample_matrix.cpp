// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Волокитин В.Д.
//
// Тестирование матриц
//---------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include "tmatrix.h"
#include <limits>
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
 
  TCRSMatrix<int> tmp;
  int command;

  while (true) {
      cout << "\n1. Ввести матрицу в формате CRS\n"
          << "2. Вывести матрицу\n"
          << "3. Транспонировать матрицу\n"
          << "4. Умножить матрицу на вектор\n"
          << "5. Умножить матрицу на другую матрицу\n"
          << "0. Выход\n"
          << "Выберите действие: ";

      if (!(cin >> command))
          break;

      if (command == 0)
          break;

      try {
          switch (command) {
          case 1: {
              cout << "Введите rows, cols, NNZ, values, col_index, row_ptr:\n";

              if (!(cin >> tmp))
                  throw invalid_argument("Ошибка чтения матрицы");

              cout << "Матрица сохранена.\n";
              break;
          }

          case 2:
              cout << tmp;
              break;

          case 3:
              tmp = tmp.transpose();
              cout << "Матрица после транспонирования:\n"
                  << tmp;
              break;

          case 4: {
              vector<int> vec(tmp.getCols());

              cout << "Введите " << vec.size()<< " элементов вектора:\n";

              for (auto& value : vec) {
                  if (!(cin >> value))
                      throw invalid_argument("Ошибка чтения вектора");
              }

              vector<int> exp_res = tmp * vec;

              cout << "Результат: ";
              for (int value : exp_res)
                  cout << value << ' ';
              cout << '\n';
              break;
          }

          case 5: {
              TCRSMatrix<int> tmp2;

              cout << "Введите вторую матрицу: rows, cols, NNZ,\n"<< "затем values, col_index, row_ptr:\n";

              if (!(cin >> tmp2))
                  throw invalid_argument("Ошибка чтения матрицы");

              TCRSMatrix<int> result = tmp * tmp2;
              cout << "Результат умножения:\n" << result;
              break;
          }

          default:
              cout << "Нет такого пункта меню.\n";
          }
      }
      catch (const exception& error) {
          cout << "Ошибка: " << error.what() << '\n';

          if (cin.eof() || cin.bad())
              break;

          if (cin.fail()) {
              cin.clear();
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
          }
      }
  }
  return 0;
}
//---------------------------------------------------------------------------
