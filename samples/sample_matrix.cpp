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

/*int main()
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
*/

int main() 
{
    setlocale(LC_ALL, "Russian");
    TCRSMatrix<int> mA;
    TCRSMatrix<int> mB;
    TCRSMatrix<int> lastM;
    bool saveA = false;
    bool saveB = false;
    bool saveLastM = false;
    int choice = 1;

    while (choice != 0)
    {
        cout << "\n1 - Ввести матрицу A" << endl;
        cout << "2 - Ввести матрицу B" << endl;
        cout << "3 - Вывести матрицы" << endl;
        cout << "4 - Транспонировать матрицу" << endl;
        cout << "5 - Умножить A на вектор" << endl;
        cout << "6 - Умножить A на B" << endl;
        cout << "7 - Заменить A на последний результат умножения" << endl;
        cout << "8 - Заменить B на последний результат умножения" << endl;
        cout << "0 - Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
        {
            try {
                int r, c;
                cout << "Введите строки и столбцы для A: ";
                cin >> r >> c;

                vector<vector<int>> dense(r, vector<int>(c));
                cout << "Введите элементы построчно:" << endl;
                for (int i = 0; i < r; i++)
                    for (int j = 0; j < c; j++)
                        cin >> dense[i][j];

                mA = TCRSMatrix<int>(dense);
                saveA = true;
                cout << "Матрица A создана" << endl;
            }
            catch (const exception& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
            break;
        }
        case 2:
        {
            try {
                int r, c;
                cout << "Введите строки и столбцы для B: ";
                cin >> r >> c;

                vector<vector<int>> dense(r, vector<int>(c));
                cout << "Введите элементы построчно:" << endl;
                for (int i = 0; i < r; i++)
                    for (int j = 0; j < c; j++)
                        cin >> dense[i][j];

                mB = TCRSMatrix<int>(dense);
                saveB = true;
                cout << "Матрица B создана" << endl;
            }
            catch (const exception& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
            break;
        }
        case 3:
        {
            if (saveA) {
                cout << "Матрица A:\n" << mA << endl;
                printCRSInternals(mA, "A");
            }
            else cout << "A не введена\n";

            if (saveB) {
                cout << "Матрица B:\n" << mB << endl;
                printCRSInternals(mB, "B");
            }
            else cout << "B не введена\n";
            break;
        }
        case 4:
        {
            try {
                char which;
                cout << "Какую транспонировать? (A или B): ";
                cin >> which;

                if (which == 'A' && saveA) {
                    mA = mA.transpose();
                    cout << "A транспонирована\n";
                }
                else if (which == 'B' && saveB) {
                    mB = mB.transpose();
                    cout << "B транспонирована\n";
                }
                else cout << "Ошибка выбора или матрица не введена\n";
            }
            catch (const exception& e) {
                cout << "Ошибка: " << e.what() << endl;
            }
            break;
        }
        case 5:
        {
            if (!saveA) {
                cout << "Сначала введите матрицу A\n";
            }
            else {
                try {
                    int size = mA.getCols();
                    vector<int> vec(size);
                    cout << "Введите вектор из " << size << " чисел: ";
                    for (int i = 0; i < size; i++) cin >> vec[i];

                    vector<int> res = mA * vec;
                    cout << "Результат: [ ";
                    for (int i = 0; i < res.size(); i++) cout << res[i] << " ";
                    cout << "]" << endl;
                }
                catch (const exception& e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
            }
            break;
        }
        case 6:
        {
            if (!saveA || !saveB) {
                cout << "Не все из матриц введены\n";
            }
            else {
                try {
                    lastM = mA * mB;
                    saveLastM = true;
                    cout << "Результат A * B:\n" << lastM << endl;
                    printCRSInternals(lastM, "Res");
                    cout << "Результат сохранён. Используйте пункты 7 или 8.\n";
                }
                catch (const exception& e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
            }
            break;
        }
        case 7:
        {
            if (!saveLastM) {
                cout << "Сначала выполните пункт 6\n";
            }
            else {
                mA = lastM;
                saveA = true;
                cout << "Матрица A заменена на последний результат\n";
            }
            break;
        }
        case 8:
        {
            if (!saveLastM) {
                cout << "Сначала выполните пункт 6\n";
            }
            else {
                mB = lastM;
                saveB = true;
                cout << "Матрица B заменена на последний результат\n";
            }
            break;
        }
        case 0:
        {
            cout << "Выход\n";
            break;
        }

        default:
        {
            cout << "Неверный пункт!\n";
            break;
        }
        }
    }
    return 0;
}